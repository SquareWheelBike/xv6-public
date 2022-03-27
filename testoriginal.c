#include "types.h"
#include "user.h"

int main(void)
{
    // fork into parent and child; parent will kill child after a short sleep
    int pid = fork();
    if (!pid) // child
    {
        sleep(5);
        printf(1, "I am the child, my pid is %d\n", getpid());
        while (1)
        {
            sleep(30);
            printf(1, "child heartbeat <3\n");
        }
    }
    // parent code
    printf(1, "I am the parent, my pid is %d\n", getpid());
    sleep(100); // wait for a few heartbeats
    printf(1, "parent killing child pid=%d...\n", pid);
    kill(pid, 0);
    printf(1, "done. exiting parent.\n");
    exit();
}
