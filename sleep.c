#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>
#include <time.h>

void Delay(unsigned int seconds){
    unsigned int count = 0;
    clock_t start = clock();

    while(count < seconds){
        if (((clock() - start) / CLOCKS_PER_SEC) >= 1){
            count ++;
            start = clock();
        }
    }
}

int main(int argc, char* argv[]){
    int seconds;

    if (argc != 2){
        printf("[!] USAGE: sleep.exe <seconds>\n");
        return 1;
    }

    seconds = atoi(argv[1]);  

    Delay(seconds);
    
    return 0;
}
