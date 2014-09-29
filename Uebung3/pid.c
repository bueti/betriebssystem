#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

static char* PIPE = "/tmp/my_pipe";

int main(int argc, const char *argv[])
{

    pid_t child_pid = fork();
    char ch;

    if (child_pid == 0) {
        // child, lese was geschrieben wurde
        FILE* in = fopen(PIPE, "r");

        while( ( ch = fgetc(in) ) != EOF )
            printf("%c",ch);

        fclose(in);
        printf("child finished\n");

    } else {
        // Parent, schreib was in die pipe
        FILE* in = fopen(PIPE, "a");

        fputs("foo bar 2\n", in);

        fclose(in);
        printf("parent finished\n");
    }

    return 0;
}


