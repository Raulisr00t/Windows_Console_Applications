#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 3) {
        wcerr << L"[-] Usage is: runas <[domain\\username]> <command>" << endl;
        return 1;
    }

    wcout << L"Enter the password for " << argv[1] << ": ";

    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == 8) {  
            if (!password.empty()) {
                wcout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password += ch;
            wcout << "";
        }
    }
    wcout << endl;  

    wstring wpassword(password.begin(), password.end());
    wstring wusername(argv[1]);

    wstring domain = L".";
    size_t backslashPos = wusername.find(L'\\');
    if (backslashPos != wstring::npos) {
        domain = wusername.substr(0, backslashPos);
        wusername = wusername.substr(backslashPos + 1);
    }
    wcout << L"Attempting to start " << argv[2] << " as user " << wusername << "..." << endl;

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    DWORD lastError;
    if (!CreateProcessWithLogonW(wusername.c_str(), domain.c_str(), wpassword.c_str(), LOGON_WITH_PROFILE, nullptr, argv[2], 0, nullptr, nullptr, &si, &pi)) {
        lastError = GetLastError();
        wcerr << L"[-] Error is: " << lastError << " [-]" << endl;
        if (lastError == ERROR_LOGON_FAILURE) {
            cerr << "[-] PASSWORD is not Correct [-]" << endl;
            return 1;
        }
        return 1;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
