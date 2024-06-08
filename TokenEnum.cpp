#include <windows.h>
#include <iostream>
#include <sddl.h> 
#include <lm.h>

using namespace std;

LONGLONG LuidToNumber(LUID& luid) {
    return *(ULONGLONG*)&luid;
}

void DisplayTokenInfo(HANDLE hToken) {
    TOKEN_STATISTICS stats;
    DWORD len;
    if (!GetTokenInformation(hToken, TokenStatistics, &stats, sizeof(stats), &len)) {
        cerr << "[-] Failed to get token statistics. Error: " << GetLastError() << endl;
        return;
    }

    cout << "[!] Token type [!] " << (stats.TokenType == TokenPrimary ? "Primary" : "Impersonation") << endl;
    cout << "[!] Token ID [!] " << LuidToNumber(stats.TokenId) << endl;
    cout << "[!] Token Groups [!] " << stats.GroupCount << endl;
    cout << "[!] Privileges [!] " << stats.PrivilegeCount << endl;

    BYTE buffer[1 << 12];

    if (!GetTokenInformation(hToken, TokenUser, buffer, sizeof(buffer), &len)) {
        cerr << "[-] Failed to get token user. Error: " << GetLastError() << endl;
        return;
    }

    auto user = reinterpret_cast<TOKEN_USER*>(buffer);
    LPWSTR strSid = nullptr;
    if (ConvertSidToStringSidW(user->User.Sid, &strSid)) {
        wcout << "[!] USER SID [!] " << strSid << endl;
        LocalFree(strSid); 
    }
    else {
        cerr << "[-] Failed to convert SID to string. Error: " << GetLastError() << endl;
    }

    WCHAR username[256], domain[256];
    DWORD usernameLen = sizeof(username) / sizeof(WCHAR), domainLen = sizeof(domain) / sizeof(WCHAR);
    SID_NAME_USE sidType;
    if (LookupAccountSidW(nullptr, user->User.Sid, username, &usernameLen, domain, &domainLen, &sidType)) {
        wcout << "[!] USER [!] " << domain << L"\\" << username << endl;
    }
    else {
        cerr << "[-] Failed to look up account SID. Error: " << GetLastError() << endl;
    }

    if (!GetTokenInformation(hToken, TokenGroups, buffer, sizeof(buffer), &len)) {
        cerr << "[-] Failed to get token groups. Error: " << GetLastError() << endl;
        return;
    }

    auto groups = reinterpret_cast<TOKEN_GROUPS*>(buffer);
    wcout << "[!] Token Groups [!]" << endl;
    for (DWORD i = 0; i < groups->GroupCount; i++) {
        if (ConvertSidToStringSidW(groups->Groups[i].Sid, &strSid)) {
            wcout << "  Group " << i + 1 << " SID: " << strSid << endl;
            LocalFree(strSid);
        }
        else {
            cerr << "[-] Failed to convert group SID to string. Error: " << GetLastError() << endl;
        }

        DWORD groupNameLen = sizeof(username) / sizeof(WCHAR), domainLen = sizeof(domain) / sizeof(WCHAR);
        if (LookupAccountSidW(nullptr, groups->Groups[i].Sid, username, &groupNameLen, domain, &domainLen, &sidType)) {
            wcout << "  Group " << i + 1 << ": " << domain << L"\\" << username << endl;
        }
        else {
            cerr << "[-] Failed to look up group SID. Error: " << GetLastError() << endl;
        }
    }

    if (!GetTokenInformation(hToken, TokenPrivileges, buffer, sizeof(buffer), &len)) {
        cerr << "[-] Failed to get token privileges. Error: " << GetLastError() << endl;
        return;
    }
    auto privileges = reinterpret_cast<TOKEN_PRIVILEGES*>(buffer);
    wcout << "[!] Token Privileges [!]" << endl;
    for (DWORD i = 0; i < privileges->PrivilegeCount; i++) {
        WCHAR privilegeName[256];
        DWORD privilegeNameLen = sizeof(privilegeName) / sizeof(WCHAR);
        if (LookupPrivilegeNameW(nullptr, &privileges->Privileges[i].Luid, privilegeName, &privilegeNameLen)) {
            wcout << "  Privilege " << i + 1 << ": " << privilegeName;
            if (privileges->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED) {
                wcout << " (Enabled)" << endl;
            }
            else {
                wcout << " (Disabled)" << endl;
            }
        }
        else {
            cerr << "[-] Failed to look up privilege name. Error: " << GetLastError() << endl;
        }
    }
}
int main(int argc, char* argv[]) {
    DWORD PID = 0, TID = 0;
    HANDLE hObject = nullptr;
    HANDLE hToken = nullptr;

    if (argc == 1) {
        cout << "[!] Usage: Token | PID [!]" << endl;
        cerr << "[!] Using current process [!]" << endl;
        PID = GetCurrentProcessId();
        hObject = GetCurrentProcess();
    }
    else {
        PID = atoi(argv[1]);
        if (PID == 0) {
            cerr << "[-] Invalid PID. Error: " << GetLastError() << endl;
            return EXIT_FAILURE;
        }
        hObject = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);
        if (!hObject && GetLastError() != ERROR_ACCESS_DENIED) {
            TID = PID;
            PID = 0;
            hObject = OpenThread(THREAD_QUERY_INFORMATION, FALSE, TID);
            if (!hObject) {
                cerr << "[-] Error opening thread. Error: " << GetLastError() << endl;
                return EXIT_FAILURE;
            }
        }

        if (!hObject) {
            cerr << "[-] Error opening process/thread. Error: " << GetLastError() << endl;
            return EXIT_FAILURE;
        }

        if (TID) {
            cout << "[+] TID is: " << TID << endl;
            if (!OpenThreadToken(hObject, TOKEN_QUERY, TRUE, &hToken)) {
                if (GetLastError() == ERROR_ACCESS_DENIED) {
                    TID = 0;
                    PID = GetProcessIdOfThread(hObject);
                    CloseHandle(hObject);
                    cout << "[!] Thread is not impersonating. Using the process " << PID << endl;
                    hObject = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);
                }
                else {
                    cerr << "[-] Error opening thread token. Error: " << GetLastError() << endl;
                    CloseHandle(hObject);
                    return EXIT_FAILURE;
                }
            }
        }

        if (PID) {
            cout << "[+] PROCESS is: " << PID << endl;
            if (!OpenProcessToken(hObject, TOKEN_QUERY, &hToken)) {
                cerr << "[-] Error opening process token. Error: " << GetLastError() << endl;
                CloseHandle(hObject);
                return EXIT_FAILURE;
            }
        }
    }

    if (!hToken) {
        cerr << "[-] Error opening token. Error: " << GetLastError() << endl;
        if (hObject) CloseHandle(hObject);
        return EXIT_FAILURE;
    }

    cout << "[+] Token opened successfully [+]" << endl;
    DisplayTokenInfo(hToken);
    CloseHandle(hToken);

    if (hObject) CloseHandle(hObject);

    return EXIT_SUCCESS;
}
