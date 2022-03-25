#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "mystdio.h"

int main(int argc, char *argv[])
{
    FILE *fp = myfopen("mystdiotest.txt", 'w');
    if (fp == NULL)
    {
        printf(1, "myfopen failed\n");
        exit();
    }
    myfprintf(mystdout, "myfopen succeeded.\n");
    myfflush(mystdout);
    myfprintf(mystdout, "writing some lines to mystdiotest.txt\n");
    myfflush(mystdout);
    myfprintf(fp, "COMP 3300 Assignment 1 Test File\n");
    myfprintf(mystdout, "Done writing to mystdiotest.txt\n");
    myfflush(mystdout);
    myfclose(fp);
    myfprintf(mystdout, "Done closing mystdiotest.txt. Exiting...\n");
    myfflush(mystdout);
    exit();
}