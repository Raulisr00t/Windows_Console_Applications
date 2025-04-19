#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>

#pragma comment(lib, "psapi.lib")

BOOL IsNumeric(const char* str) {
    while (*str) {
        if (!isdigit((unsigned char)*str)) return FALSE;
        str++;
    }

    return TRUE;
}

void DisplayExePathFromPID(DWORD pid) {
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (!hProc) {
        DWORD errcode;
        errcode = GetLastError();

        if (errcode == ERROR_ACCESS_DENIED){
            printf("[!] You need Administrartor rights [!]\n");
            exit(1);
        }

        else
            printf("[!] Failed to open process %lu. Error: %lu\n", pid, GetLastError());
        return;
    }

    char path[MAX_PATH];

    if (GetModuleFileNameExA(hProc, NULL, path, MAX_PATH)) 
        printf("[+] PID %lu [+] Exe Path: %s\n", pid, path);
    
    else 
        printf("[!] Failed to get path for PID %lu. Error: %lu\n", pid, GetLastError());

    CloseHandle(hProc);
}

void DisplayExePathByName(LPCSTR processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("[!] Snapshot error: %lu\n", GetLastError());
        return;
    }

    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (_stricmp(pe.szExeFile, processName) == 0) {
                DisplayExePathFromPID(pe.th32ProcessID);
                break;
            }
        } 
        while (Process32Next(hSnapshot, &pe));
    } 
    
    else 
        printf("[!] Failed to get first process.\n");

    CloseHandle(hSnapshot);
}


int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("[!] USAGE: takepath <pid|processname>\n");
        return 1;
    }

    if (IsNumeric(argv[1])) {
        DWORD pid = strtoul(argv[1], NULL, 10);

        DisplayExePathFromPID(pid);
    } 
    
    else 
        DisplayExePathByName(argv[1]);

    return 0;
}
