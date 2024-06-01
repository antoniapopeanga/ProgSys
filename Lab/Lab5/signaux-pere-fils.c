#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NSIGNALS 32

volatile int signal_count = 0;
int target_signals = 10;

void signal_handler(int signum) {
    signal_count++;
    printf("Received signal %d (%d times)\n", signum, signal_count);

    if (signal_count >= target_signals) {
        printf("Received enough signals. Exiting...\n");
        exit(0);
    }
}

void ack_handler(int signum) {
    // Acknowledgment handler for SIGUSR1
}

int emetteur(int pere, int argc, char *argv[]) {
    int k = atoi(argv[1]);

    struct sigaction ack_sa;
    ack_sa.sa_handler = ack_handler;
    ack_sa.sa_flags = 0;
    sigemptyset(&ack_sa.sa_mask);

    // Set up acknowledgment handler for SIGUSR1
    sigaction(SIGUSR1, &ack_sa, NULL);

    for (int i = 0; i < k; i++) {
        // Send signals to the parent
        for (int j = 2; j < argc; j++) {
            kill(pere, atoi(argv[j]));
        }

        // Wait for acknowledgment before sending the next batch
        pause();
    }

    // Send SIGKILL to terminate the parent
    kill(pere, SIGKILL);

    return 0;
}

int recepteur(int fils) {
    printf("récepteur : %d\n", getpid());

    // Block all signals during setup
    sigset_t all_signals;
    sigfillset(&all_signals);
    sigprocmask(SIG_BLOCK, &all_signals, NULL);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // Install the signal handler for all signals
    for (int sig = 0; sig < NSIGNALS; sig++) {
        sigaction(sig, &sa, NULL);
    }

    // Unblock signals for normal processing
    sigprocmask(SIG_UNBLOCK, &all_signals, NULL);

    while (1)
        pause();

    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == 0)
        emetteur(getppid(), argc, argv);
    else
        recepteur(pid);
}


/*#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NSIGNALS 32


volatile int signal_count = 0;
int target_signals = 10; 

void signal_handler(int signum) {
    signal_count++;
    printf("Received signal %d (%d times)\n", signum, signal_count);

    if (signal_count >= target_signals) {
        printf("Received enough signals. Exiting...\n");
        exit(0);
    }
}

int emetteur(int pere, int argc, char *argv[]) {
    int k = atoi(argv[1]);
    //sleep(1);
    sigaction(SIGUSR1,&k,NULL);
    sigprocmask(SIG_UNBLOCK, sigprocmask(SIG_UNBLOCK,));

    for (int i = 0; i < k; i++) {
        for (int j = 2; j < argc; j++) {
            kill(pere, atoi(argv[j]));
        }
    }
sleep(1);
kill(pere,9);
    return 0;
}

int recepteur(int fils) {
    printf("récepteur : %d\n", getpid());
        //MASQUER
     sigset_t tout,old;
     sigfillset(&tout);
     sigprocmask(SIG_BLOCK,&tout,NULL);
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    for (int sig = 0; sig < NSIGNALS; sig++) {
        sigaction(sig, &sa, NULL);
    }
//DEMASQUER
    sigprocmask(SIG_UNBLOCK,&old,NULL);
    while (1)
        pause();

    return 0;
}

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == 0)
        emetteur(getppid(), argc, argv);
    else
        recepteur(pid);
}
*/
