#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <string.h>

DWORD GetPID(LPCSTR procname) {
    DWORD pid = 0; 
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("[-] Error: %lu\n", GetLastError());
        return 0; 
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (strcmp(pe.szExeFile, procname) == 0) {
                pid = pe.th32ProcessID;
                CloseHandle(hSnapshot);
                return pid;
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);

    return pid; 
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("[!] USAGE: pidof.exe <processname>\n");
        return -1;
    }

    LPCSTR processname = argv[1];
    DWORD pid = GetPID(processname);

    if (pid == 0) {
        printf("[-] Process not found\n");
        return 1;
    }

    printf("PID: %lu\n", pid);
    return 0;
}
