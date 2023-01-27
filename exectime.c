// execute another command and time how long it takes to run

#include "user.h"
#include "defs.h"
#include "types.h"

int main(int argc, char *argv[])
{
    int pid, stime, etime, dtime;

    stime = uptime();

    if(argc < 2){
        printf(2, "Usage: exectime cmd [args...]\n");
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

    etime = uptime();
    dtime = etime - stime;
    printf(1, "%d %d %d\n", stime, etime, dtime);
    exit();
}