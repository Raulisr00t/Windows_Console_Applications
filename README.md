# Battery Status Monitor
This is a simple C program that retrieves the current battery status of a Windows system and prompts the user with various actions based on the battery percentage.

## Features
Displays the current battery percentage in a message box.
Prompts the user to shut down the computer if the battery level is below 10%.
Provides a status message for battery levels above 10%.

### Prerequisites
Windows operating system.
MinGW or any other C compiler that supports Windows API functions.

### Building the Program
Clone the repository or download the source code.
Open a terminal or command prompt.
Navigate to the directory containing the source code.

Compile the program using a C compiler. For example, with MinGW:
```sh
gcc -o BatteryStatusMonitor BatteryStatusMonitor.c -lgdi32
```
Running the Program
Run the compiled executable:
```sh
BatteryStatusMonitor.exe
```
The program will display a message box with the current battery percentage.

If the battery level is below 10%, it will prompt you in the console to shut down the computer.

If you choose 'Y', the program will attempt to abort any ongoing shutdowns (for demonstration purposes). If you choose 'N', the program will exit.

For battery levels above 10%, it will provide a status message and exit.

### Code Explanation
The main logic of the program is as follows:

Retrieve the battery status using the GetSystemPowerStatus function.
Display the battery percentage in a message box.
If the battery percentage is less than 10%, prompt the user to shut down the computer.
If the user inputs 'Y', attempt to abort the system shutdown.
If the user inputs 'N', simply exit the program.
If the battery percentage is above 50%, display a message encouraging the user to continue.
For battery percentages between 10% and 50%, display the battery percentage.

### Error Handling
If the program fails to retrieve the battery status, it will display an error message in a message box and exit with a failure code.
### Known Issues
The program does not handle localization for different languages.
The program assumes a simple 'Y/N' input for the shutdown prompt and does not handle other inputs robustly.
The shutdown logic is a placeholder and does not actually perform a shutdown for safety reasons.

# RunAs Utility
This RunAs utility allows you to run a command as a different user by providing the username, domain (optional), and password.

### Usage
runas <[domain\username]> <command>

### Example
runas mydomain\myuser "C:\Path\To\Command.exe"

### Features
Prompts the user to enter a password securely.
Supports specifying the domain and username in the format domain\username.
Attempts to start the specified command with the provided credentials.

### Code Overview

#### Main Function
The main function handles command-line arguments and prompts the user for a password. It then attempts to run the specified command with the provided credentials.
```cpp
int wmain(int argc, wchar_t* argv[]) {
    if (argc < 3) {
        wcerr << L"[-] Usage is: runas <[domain\\username]> <command>" << endl;
        return 1;
    }

    wcout << L"Enter the password for " << argv[1] << ": ";

    string password;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == 8) {  // Handle backspace
            if (!password.empty()) {
                wcout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password += ch;
        }
    }
    wcout << endl;  // Move to the next line after password input

    // Convert password to wstring
    wstring wpassword(password.begin(), password.end());
    wstring wusername(argv[1]);

    // Parse domain and username
    wstring domain = L".";
    size_t backslashPos = wusername.find(L'\\');
    if (backslashPos != wstring::npos) {
        domain = wusername.substr(0, backslashPos);
        wusername = wusername.substr(backslashPos + 1);
    }
    wcout << L"Attempting to start " << argv[2] << " as user " << wusername << "..." << endl;

    // Initialize structures
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Create process with the provided credentials
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

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
```
### Key Functions
_getch(): Captures user input without displaying it on the screen, ensuring password security.
CreateProcessWithLogonW: Creates a new process and logon session for the specified user.

### Error Handling
The program checks for the correct number of arguments and provides usage instructions if arguments are missing.
It handles incorrect passwords by checking the ERROR_LOGON_FAILURE error code and displays an appropriate message.
### Compilation
To compile this program, use a C++ compiler that supports Windows API functions. For example, using Microsoft Visual C++:
```cmd
cl runas.cpp /link user32.lib advapi32.lib
```
## License
This project is licensed under the MIT License. See the LICENSE file for more details.

###Contributing
Contributions are welcome! Please feel free to submit a pull request or report issues.

## Author Raulisr00t
