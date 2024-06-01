#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile int ctrl_c_count = 0;

void sigint_handler(int signum) {
    if (ctrl_c_count>0) {
        printf("The program is terminating...\n");
        exit(0);
    }
    ctrl_c_count++;
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }
    while (1) {
        if(ctrl_c_count>=1)
         printf("Ctrl-C\n");
        //sleep(1);
    }
    return 0;
}
