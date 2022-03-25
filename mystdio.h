#define BUFF_SIZE 256
#define NULL ((void*)0)

typedef struct FILE
{
  int fd;
  char mode;
  int obuf_pos;
  char obuf[BUFF_SIZE];
} FILE;

// FILE *mystdout should be pre-defined and implicitly opened upon any execution of a user program.
extern FILE *mystdout;

extern void myfflush(FILE *fp);
extern FILE *myfopen(const char *, char);
extern void myfclose(FILE *);
extern void myfprintf(FILE *, const char *, ...);