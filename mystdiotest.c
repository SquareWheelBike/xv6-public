#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFSIZE 512

typedef struct {
    int fd;
    int mode;
    char buf[BUFSIZE];
    int bufp;
} FILE;

FILE *stdin;
FILE *stdout;

FILE *myfopen(char *path, char *mode) {
    FILE *fp;
    int fd;
    int m;

    if (*mode == 'r') {
        m = O_RDONLY;
    } else if (*mode == 'w') {
        m = O_WRONLY | O_CREATE;
    } else {
        return 0;
    }

    if ((fd = open(path, m)) < 0) {
        return 0;
    }

    if ((fp = malloc(sizeof(FILE))) == 0) {
        close(fd);
        return 0;
    }

    fp->fd = fd;
    fp->mode = m;
    fp->bufp = 0;
    return fp;
}

int myfgetc(FILE *fp) {
    if (fp->mode != O_RDONLY) {
        return -1;
    }

    if (fp->bufp == 0) {
        int n = read(fp->fd, fp->buf, BUFSIZE);
        if (n <= 0) {
            return -1;
        }
        fp->bufp = n;
    }

    return fp->buf[--fp->bufp];
}

int myfputc(int c, FILE *fp) {
    if (fp->mode != O_WRONLY) {
        return -1;
    }

    if (fp->bufp == BUFSIZE) {
        int n = write(fp->fd, fp->buf, BUFSIZE);
        if (n != BUFSIZE) {
            return -1;
        }
        fp->bufp = 0;
    }

    fp->buf[fp->bufp++] = c;
    return c;
}

int myfclose(FILE *fp) {
    int r = 0;

    if (fp->mode == O_WRONLY) {
        r = write(fp->fd, fp->buf, fp->bufp);
    }

    close(fp->fd);
    free(fp);
    return r;
}

int myfprintf(FILE *fp, char *s, ...){
    char *p;
    int c;
    int i;
    int *ap;

    ap = (int *)(&s + 1);
    for (p = s; *p; p++) {
        if (*p != '%') {
            myfputc(*p, fp);
            continue;
        }

        switch (*++p) {
        case 'd':
            c = *ap;
            if (c < 0) {
                myfputc('-', fp);
                c = -c;
            }
            i = 1;
            while (c / i >= 10) {
                i *= 10;
            }
            while (i > 0) {
                myfputc('0' + c / i, fp);
                c %= i;
                i /= 10;
            }
            break;
        case 'x':
            c = *ap;
            i = 28;
            while (i >= 0) {
                int d = (c >> i) & 0xf;
                if (d > 0 || i == 0) {
                    myfputc(d + (d < 10 ? '0' : 'a' - 10), fp);
                }
                i -= 4;
            }
            break;
        case 's':
            for (i = 0; ((char *)ap)[i]; i++) {
                myfputc(((char *)ap)[i], fp);
            }
            break;
        case 'c':
            myfputc(*ap, fp);
            break;
        default:
            myfputc(*p, fp);
            break;
        }
        ap++;
    }
    return 0;
}