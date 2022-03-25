// Cole Fuerth pstate command

// print out the current process state as a list
// example:
/**
 * pid       name      state          parent
 * -----------------------------------------
 * 1         init      SLEEPING       (init)
 * 2         sh        SLEEPING       init
 * 34        pstate    RUNNING        sh
 * 29        pi        RUNNABLE       init
 * 31        pi        RUNNABLE       init
 * 33        pi        RUNNING        init
 * total: 6
 * cpu 0: pstate
 * cpu 1: pi
 */

#include "types.h"
#include "defs.h"

int main(void)
{
    pstate();
    exit();
}