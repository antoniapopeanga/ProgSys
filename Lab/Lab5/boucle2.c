#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


volatile int first_sigint_received = 0;

void sigint_handler(int signum) {
    if (!first_sigint_received) {
        printf("Ctrl-C received. Starting the loop...\n");
        first_sigint_received = 1;
    } else {
        printf("Ignoring additional Ctrl-C signals...\n");
    }
}

int main() {
    signal(SIGINT, sigint_handler);

    while (1) {
        if(first_sigint_received==1)
          printf("Working...\n");
        sleep(1);
    }

    return 0;
}
