#include <windows.h>
#include <stdio.h>
#include <string.h>

void RestartComputer() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        printf("[!] Failed to acquire admin privileges! Error code: %lu\n", GetLastError());
        return;
    }

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0)) {
        printf("[!] Failed to adjust privileges! Error code: %lu\n", GetLastError());
        return;
    }

    if (GetLastError() != ERROR_SUCCESS) {
        printf("[!] Error while adjusting privileges. Error code: %lu\n", GetLastError());
        return;
    }

    if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) {
        printf("[!] Failed to restart the computer! Error code: %lu\n", GetLastError());
    } else {
        printf("[*] Restarting the computer...\n");
    }
}

void ShutdownComputer() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        printf("[!] Failed to acquire admin privileges! Error code: %lu\n", GetLastError());
        return;
    }

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0)) {
        printf("[!] Failed to adjust privileges! Error code: %lu\n", GetLastError());
        return;
    }

    if (GetLastError() != ERROR_SUCCESS) {
        printf("[!] Error while adjusting privileges. Error code: %lu\n", GetLastError());
        return;
    }

    if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) {
        printf("[!] Failed to shut down the computer! Error code: %lu\n", GetLastError());
    } 
    else {
        printf("[*] Shutting down the computer...\n");
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("[!] Usage: init.exe <option> [0,6]\n");
        return -1;
    }

    if (strcmp(argv[1], "0") == 0) {
        ShutdownComputer();
    } 
    
    else if (strcmp(argv[1], "6") == 0) {
        RestartComputer();
    }
    
     else {
        printf("[!] Invalid option! Usage: init.exe <option> [0,6]\n");
        return -1;
    }

    return 0;
}
