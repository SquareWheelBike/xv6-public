#include "types.h"
#include "user.h"
#include "fcntl.h"

#define NULL ((void*)0)

// I need a realloc function, none implemented in xv6
void *realloc(void *ptr, uint cursize, uint newsize)
{
    void *newptr = malloc(newsize);
    if (newptr ==  0)
        return 0;
    if (cursize > 0)
    {
        memmove(newptr, ptr, cursize);
        free(ptr);
    }
    return newptr;
}


// This function counts the number of characters in string str.
int num_elements(char *str)
{
    return strlen(str);
}

// This function appends char c to string str. It might need to resize str
void append(char **str, char c)
{
    int len = num_elements(*str);
    *str = realloc(*str, len + 1, len + 2);
    (*str)[len] = c;
    (*str)[len + 1] = '\0';
}

/**
 * Open 'commands.txt' and count the frequency of each command, one command per line.
 */
int main(void)
{
    // open commands.txt
    int fd = open("commands.txt", O_RDONLY);
    if (fd < 0)
    {
        printf(2, "open commands.txt failed\n");
        exit();
    }

    int numcmds = 0;
    char **cmds = NULL;
    int *freqs = NULL;
    char *str = malloc(sizeof(char));
    str[0] = '\0';
    char c;
    while (read(fd, &c, 1) > 0)
    {
        if (c == '\n')
        {
            int i = 0;
            // check if str already in cmds
            while (i < numcmds)
            {
                if (strcmp(str, cmds[i]) == 0)
                {
                    freqs[i]++;
                    free(str);
                    break;
                }
                i++;
            }
            // if not, add it to cmds
            if (i == numcmds)
            {
                numcmds++;
                cmds = realloc(cmds, (numcmds - 1) * sizeof(char *), numcmds * sizeof(char *));
                freqs = realloc(freqs, (numcmds - 1) * sizeof(int), numcmds * sizeof(int));
                cmds[i] = str;
                freqs[i] = 1;
            }
            str = (char *)malloc(sizeof(char));
            str[0] = '\0';
        }
        else
        {
            append(&str, c);
        }
    }
    close(fd);

    // display the frequency of each command
    for (int i = 0; i < numcmds; i++)
    {
        printf(1, "%s: %d\n", cmds[i], freqs[i]);
        free(cmds[i]);
    }
    free(cmds);
    free(freqs);
    free(str);
    exit();
}