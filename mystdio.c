#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "mystdio.h"

FILE mystdouttag = {.fd = 1, .mode = 'w', .obuf_pos = 0, .obuf = {0}};
FILE *mystdout = (FILE *)&mystdouttag;

// this is not a requirement, but it is needed to flush stdout, since our implementation is fully buffered.
// when the program calls exit(), the call to myfflush(mystdout) is not made automatically, and the buffer never gets flushed.
void myfflush(FILE *fp)
{
    if (fp->obuf_pos > 0)
    {
        write(fp->fd, fp->obuf, fp->obuf_pos);
        fp->obuf_pos = 0;
    }
}

// open a file for reading or writing
// returns a FILE pointer, or NULL if the file could not be opened.
FILE *myfopen(const char *path, char mode)
{
    int flags, fd;
    FILE *fp;
    if (mode == 'r')
        flags = O_RDONLY;
    else if (mode == 'w')
        flags = O_WRONLY | O_CREATE;
    else
        return NULL;
    if ((fd = open(path, flags)) < 0)
        return NULL;
    fp = malloc(sizeof(FILE));
    fp->fd = fd;
    fp->mode = mode;
    fp->obuf_pos = 0;
    memset(fp->obuf, 0, sizeof(fp->obuf));
    return fp;
}

// flush the file, then close it and free memory
void myfclose(FILE *fp)
{
    if (fp->mode == 'w')
        myfflush(fp);
    close(fp->fd);
    free(fp);
}

// this is where all buffering takes place
static void putc(FILE *fp, char c)
{
    fp->obuf[fp->obuf_pos++] = c;
    if (fp->obuf_pos == BUFF_SIZE)
        myfflush(fp);
    {
        write(fp->fd, fp->obuf, fp->obuf_pos);
        fp->obuf_pos = 0;
    }
}

// unmodified from xv6/printf.c
static void printint(FILE *fp, int xx, int base, int sgn)
{
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (sgn && xx < 0)
    {
        neg = 1;
        x = -xx;
    }
    else
    {
        x = xx;
    }

    i = 0;
    do
    {
        buf[i++] = digits[x % base];
    } while ((x /= base) != 0);
    if (neg)
        buf[i++] = '-';

    while (--i >= 0)
        putc(fp, buf[i]);
}

// Print to the given fp. Only understands %d, %x, %p, %s.
// reused existing printf function, only changed to using file pointers instead of file descriptors
void myfprintf(FILE *fp, const char *fmt, ...)
{
    char *s;
    int c, i, state;
    uint *ap;

    state = 0;
    ap = (uint *)(void *)&fmt + 1;
    for (i = 0; fmt[i]; i++)
    {
        c = fmt[i] & 0xff;
        if (state == 0)
        {
            if (c == '%')
            {
                state = '%';
            }
            else
            {
                putc(fp, c);
            }
        }
        else if (state == '%')
        {
            if (c == 'd')
            {
                printint(fp, *ap, 10, 1);
                ap++;
            }
            else if (c == 'x' || c == 'p')
            {
                printint(fp, *ap, 16, 0);
                ap++;
            }
            else if (c == 's')
            {
                s = (char *)*ap;
                ap++;
                if (s == 0)
                    s = "(null)";
                while (*s != 0)
                {
                    putc(fp, *s);
                    s++;
                }
            }
            else if (c == 'c')
            {
                putc(fp, *ap);
                ap++;
            }
            else if (c == '%')
            {
                putc(fp, c);
            }
            else
            {
                // Unknown % sequence.  Print it to draw attention.
                putc(fp, '%');
                putc(fp, c);
            }
            state = 0;
        }
    }
}
