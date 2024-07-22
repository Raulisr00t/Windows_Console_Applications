#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

void FindStrings(const unsigned char* address, DWORD size, int WordCount) {
    auto p = address;
    char word[64];

    while (size > 0) {
        int i = 0;
        while (size > 0 && isprint(*p)) {
            if (i < sizeof(word) - 1) {
                word[i++] = *p;
            }
            p++;
            size--;
        }

        if (i >= WordCount) {
            word[i] = '\0';
            cout << "[+] 0x" << hex << uintptr_t(p - address - strlen(word)) << ": " << word << endl;
        }

        if (size == 0)
            break;

        while (size > 0 && !isprint(*p)) {
            p++;
            size--;
        }
    }
    return;
}

int wmain(int argc, const wchar_t* argv[]) {
    HANDLE hFile;
    HANDLE hMemMap;
    LPCWSTR filename = argv[1];
    DWORD size;

    if (argc < 2) {
        cerr << "[!] USAGE: strings.exe <filename>" << endl;
        return 1;
    }

    hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        cerr << "[!] Error in Opening File" << endl;
        cerr << "[!] Error: " << GetLastError() << endl;
        return 1;
    }

    size = GetFileSize(hFile, nullptr);

    hMemMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!hMemMap) {
        cerr << "[!] Error in File Mapping" << endl;
        cerr << "[!] Error: " << GetLastError() << endl;
        CloseHandle(hFile);
        return 1;
    }

    void* p = MapViewOfFile(hMemMap, FILE_MAP_READ, 0, 0, 0);
    if (!p) {
        cerr << "[!] Error Failed to Map View" << endl;
        cerr << "[!] Error: " << GetLastError() << endl;
        CloseHandle(hMemMap);
        CloseHandle(hFile);
        return 1;
    }

    FindStrings((const unsigned char*)p, size, 5);

    UnmapViewOfFile(p);
    CloseHandle(hMemMap);
    CloseHandle(hFile);

    return 0;
}
