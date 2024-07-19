#include <iostream>
#include <windows.h>
#include <Psapi.h>
#include <stdexcept>

using namespace std;

COORD GetPos() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        throw runtime_error("Failed to get console screen buffer info");
    }
    return info.dwCursorPosition;
}

void SetPos(COORD pos) {
    if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos)) {
        throw runtime_error("Failed to set console cursor position");
    }
}

int main(int argc, char* argv[]) {
    PERFORMANCE_INFORMATION pi;
    SYSTEM_INFO si;
    
    GetSystemInfo(&si);
    cout << "Page Size: " << si.dwPageSize << " bytes\n";
    cout << "Minimum Address: " << si.lpMinimumApplicationAddress << "\n";
    cout << "Maximum Address: " << si.lpMaximumApplicationAddress << "\n";

    GetPerformanceInfo(&pi, sizeof(pi));
    cout << "RAM Total: " << (pi.PhysicalTotal >> 8) << " MB\n";

    if (argc == 1) {
        while (GetPerformanceInfo(&pi, sizeof(pi))) {
            Sleep(1000);
            cout << "RAM Available: " << (pi.PhysicalAvailable >> 8) << " MB\n";
            COORD pos = GetPos();
            SetPos(pos);
        }
    } else {
        DWORD pid = stoi(argv[1]);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

        if (hProcess == NULL) {
            cerr << "[!] Error in Opening Process: " << GetLastError() << "\n";
            return -1;
        }

        PROCESS_MEMORY_COUNTERS_EX pmc;
        SIZE_T cb = pmc.cb = sizeof(pmc);

        while (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, cb)) {
            COORD pos = GetPos();
            cout << "*Private Commit: " << (pmc.PrivateUsage >> 10) << " KB\n";
            cout << "Working Set: " << pmc.WorkingSetSize << " KB\n";
            Sleep(1000);
            SetPos(pos);
        }

        CloseHandle(hProcess);
    }

    return 0;
}
