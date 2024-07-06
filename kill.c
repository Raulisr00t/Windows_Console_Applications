#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <ntsecapi.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

int main(int argc, char *argv[]) {
    HANDLE processHandle;
    NTSTATUS status;
    OBJECT_ATTRIBUTES ObjectAttributes;
    CLIENT_ID clientId;
    DWORD processId;

    if (argc < 2) {
        printf("Usage: kill.exe <Process ID>\n");
        return 1;
    }

    processId = atoi(argv[1]);

    clientId.UniqueProcess = (HANDLE)processId;
    clientId.UniqueThread = 0;

    InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL);

    status = NtOpenProcess(&processHandle, PROCESS_TERMINATE, &ObjectAttributes, &clientId);

    if (status != 0) {
        printf("Failed to open process: %d\n", status);
        return 1;
    }

    status = NtTerminateProcess(processHandle, 0);

    if (status != 0) {
        printf("Failed to terminate process: %d\n", status);
        return 1;
    }

    printf("Process terminated successfully\n");

    return 0;
}
