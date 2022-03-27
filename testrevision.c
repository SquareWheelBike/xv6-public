#include "types.h"
#include "user.h"

int main(void)
{
    printf(1, "TESTING PAUSE()/KILL(pid,1) ---------------\n");
    printf(1, "PARENT(%d) creating a child...\n", getpid());
    int pid = fork();
    if (!pid) // child
    {
        sleep(5);
        printf(1, "I am the child, my pid is %d\n", getpid());
        printf(1, "CHILD(%d) pausing...\n", getpid());
        pause();
        sleep(5);
        printf(1, "CHILD(%d) is unpaused\n", getpid());
        while (1)
        {
            sleep(30);
            printf(1, "child heartbeat <3\n");
        }
    }
    // parent code
    sleep(100); // wait for a few heartbeats
    printf(1, "parent: child(%d) should have paused for 100 ticks (3 heartbeats), unpausing...\n", pid);
    kill(pid, 1);
    printf(1, "done. child should have a heartbeat again.\nparent sleeping 50 ticks and sending unpause() again...\n");
    sleep(50);
    kill(pid, 1);
    printf(1, "unpause sent, waiting another 50 and then killing child.\n");
    sleep(50);
    printf(1, "parent killing child pid=%d...\n", pid);
    kill(pid, 0);
    printf(1, "done. sleeping 100 ticks to ensure no more heartbeat\n");
    sleep(100);

    printf(1, "TESTING KILL() ON PAUSED CHILD -----------------\n");
    pid = fork();
    if (!pid)
    {
        sleep(5);
        printf(1, "CHILD(%d) pausing...\n", getpid());
        pause();
        while (1)
        {
            sleep(30);
            printf(1, "child heartbeat <3\n");
        }
    }
    sleep(100);
    printf(1, "PARENT(%d): i waited 100; child should be paused with no heartbeats above.\nSending kill(pid,0) to child...\n", getpid());
    kill(pid,0);
    printf(1, "child should be dead, sending kill(pid,1) to child and waiting to ensure no unpause...\n");
    kill(pid,1);
    sleep(100);
    printf(1, "parent has waited for heartbeat, child should be dead. exiting.\n");

    exit();
}
