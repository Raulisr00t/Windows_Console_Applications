#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    SYSTEM_POWER_STATUS status;
    
    if (GetSystemPowerStatus(&status)) {
        if (status.BatteryFlag != 255) {
            wchar_t message[100];
            swprintf(message, L"Windows Actual Battery percent %d%%", status.BatteryLifePercent);
            MessageBoxW(NULL, message, L"Power Manager", MB_ICONINFORMATION | MB_OK);
            
            if (status.BatteryLifePercent < 10) {
                char answer[10];
                printf("Do you want to shutdown? (Y/N): ");
                scanf("%s", answer);
                LPWSTR computer = "localhost";
                
                if (strcmp(answer, "Y") == 0) {
                    AbortSystemShutdown(computer);
                    return EXIT_SUCCESS;
                    
                } else {
                    return EXIT_SUCCESS;
                }
            } else if (status.BatteryLifePercent > 50) {
                printf("[+] Please continue [+]");
                return EXIT_SUCCESS;

            } else {
                printf("[!] Your power percent:%s",status.BatteryLifePercent);
                return EXIT_SUCCESS;
            }
        } else {
            MessageBoxW(NULL, L"Failed to retrieve battery information.", L"Battery Manager", MB_ICONINFORMATION | MB_OK);
            return EXIT_FAILURE;
        }
    } else {
        MessageBoxW(NULL, L"Failed to retrieve battery information.", L"Battery Manager", MB_ICONINFORMATION | MB_OK);
        return -1;
    }
}
