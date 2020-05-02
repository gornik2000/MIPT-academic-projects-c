#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"
#define CLEAR     "\x1b[1A"

#define TEXT_SIZE 50
#define ERROR_CHECK(n)                                        \
if ((n) == -1)                                                \
{                                                             \
    printf("ERROR %s line %d\n", strerror(errno), __LINE__);  \
    return 0;                                                 \
}

char const *FILE_NAME = "test.txt";
char const *END_CMD   = "/exit";
int  const  FILE_ID   = 1;

const char TRUE  = 127;
const char FALSE = 126;

const long TYPE_SYSTEM = 1002;
const long TYPE_SERVER = 1003;

struct MsgBuf 
{
    long type_;
    char text_[TEXT_SIZE];
    char first_;
};

int const MSG_SIZE = sizeof(struct MsgBuf) - sizeof(long);
