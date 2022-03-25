#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf(2, "incorrect number of arguments!\ncommand: %s <pid> <priority>\n", argv[0]);
        exit();
    }
    set(atoi(argv[1]), atoi(argv[2]));
    exit();
}