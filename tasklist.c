#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>
#include <tchar.h>

void ListAllProcesses() {
    DWORD processes[1024], needed, numProcesses;
    HANDLE hProcess;
    HMODULE hMod;
    TCHAR processName[MAX_PATH] = TEXT("<unknown>");

    if (!EnumProcesses(processes, sizeof(processes), &needed)) {
        printf("[!] EnumProcesses Failed With Error: %lu\n", GetLastError());
        return;
    }

    numProcesses = needed / sizeof(DWORD);
    printf("[i] Number of Processes Detected: %lu\n", numProcesses);

    for (unsigned int i = 0; i < numProcesses; i++) {
        if (processes[i] != 0) {
            hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

            if (hProcess) {
                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &needed)) {
                    if (GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(TCHAR))) {
                        _tprintf(TEXT("[%03u] Process \"%s\" - Of PID: %lu\n"), i, processName, processes[i]);
                    } else {
                        printf("[!] GetModuleBaseName Failed [ At PID: %lu ] With Error: %lu\n", processes[i], GetLastError());
                    }
                } else {
                    printf("[!] EnumProcessModules Failed [ At PID: %lu ] With Error: %lu\n", processes[i], GetLastError());
                }
                CloseHandle(hProcess);
            } else {
                printf("[!] OpenProcess Failed [ At PID: %lu ] With Error: %lu\n", processes[i], GetLastError());
            }
        }
    }
}

int main() {
    ListAllProcesses();
    return 0;
}
