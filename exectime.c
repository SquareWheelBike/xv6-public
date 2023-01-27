// execute another command and time how long it takes to run
// the number of command line arguments should not be fixed

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int pid;

    printf(1, "uptime: %d\n", uptime());

    if(argc < 2){
        printf(2, "usage: exectime command argument-list\n");
        exit();
    }

    pid = fork();

    if(pid < 0){
        printf(2, "fork failed\n");
        exit();
    }

    if(pid == 0){
        exec(argv[1], &argv[1]);
        printf(2, "exec %s failed\n", argv[1]);
        exit();
    }

    wait();

    printf(1, "uptime: %d\n", uptime());
    exit();
}