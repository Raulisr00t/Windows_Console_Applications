###Battery Status Monitor
This is a simple C program that retrieves the current battery status of a Windows system and prompts the user with various actions based on the battery percentage.

###Features
Displays the current battery percentage in a message box.
Prompts the user to shut down the computer if the battery level is below 10%.
Provides a status message for battery levels above 10%.

###Prerequisites
Windows operating system.
MinGW or any other C compiler that supports Windows API functions.

###Building the Program
Clone the repository or download the source code.
Open a terminal or command prompt.
Navigate to the directory containing the source code.

Compile the program using a C compiler. For example, with MinGW:

sh
Copy code
gcc -o BatteryStatusMonitor BatteryStatusMonitor.c -lgdi32
Running the Program
Run the compiled executable:

sh
Copy code
BatteryStatusMonitor.exe
The program will display a message box with the current battery percentage.

If the battery level is below 10%, it will prompt you in the console to shut down the computer.

If you choose 'Y', the program will attempt to abort any ongoing shutdowns (for demonstration purposes). If you choose 'N', the program will exit.

For battery levels above 10%, it will provide a status message and exit.

###Code Explanation
The main logic of the program is as follows:

Retrieve the battery status using the GetSystemPowerStatus function.
Display the battery percentage in a message box.
If the battery percentage is less than 10%, prompt the user to shut down the computer.
If the user inputs 'Y', attempt to abort the system shutdown.
If the user inputs 'N', simply exit the program.
If the battery percentage is above 50%, display a message encouraging the user to continue.
For battery percentages between 10% and 50%, display the battery percentage.
###Error Handling
If the program fails to retrieve the battery status, it will display an error message in a message box and exit with a failure code.
###Known Issues
The program does not handle localization for different languages.
The program assumes a simple 'Y/N' input for the shutdown prompt and does not handle other inputs robustly.
The shutdown logic is a placeholder and does not actually perform a shutdown for safety reasons.
###License
This project is licensed under the MIT License.

###Contributing
Contributions are welcome! Please feel free to submit a pull request or report issues.

###Author
RaulisR00t
