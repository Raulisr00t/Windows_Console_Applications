#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess != NULL)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
        {
            if (GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR)))
            {
                _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
            }
            else
            {
                _tprintf(TEXT("GetModuleBaseName failed for PID: %u. Error: %u\n"), processID, GetLastError());
            }
        }
        else
        {
            _tprintf(TEXT("EnumProcessModules failed for PID: %u. Error: %u\n"), processID, GetLastError());
        }

        CloseHandle(hProcess);
    }
    else
    {
        _tprintf(TEXT("OpenProcess failed for PID: %u. Error: %u\n"), processID, GetLastError());
    }
}

int main(void)
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        _tprintf(TEXT("EnumProcesses failed. Error: %u\n"), GetLastError());
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);
    _tprintf(TEXT("Number of Processes Detected: %u\n"), cProcesses);

    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessNameAndID(aProcesses[i]);
        }
    }

    return 0;
}
