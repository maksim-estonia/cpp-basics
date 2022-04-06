#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>

/* Function declaration */
bool isNumeric(const char *str) 
{
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
            return false;
        str++;
    }
    return true;
}



int main(int argc, char* argv[]) {
    // Check if input is recieved
    if (argc == 1) {
        printf("Input not received!\n");
        exit(1);
    }
    // Check if the input is an int
    if (isNumeric(argv[1]) == false) {
        printf("Input is not an Integer!\n");
        exit(1);
    }

    // Initialize pipe
    int      fd[2];
    pid_t    childpid;

    pipe(fd);
    childpid = fork();

    if (childpid == -1) {
        printf("fork failed");
        exit(1);
    }
    else if (childpid > 0) {
            char string[100];
            close(fd[1]);
            printf("PARENT START\n");
            while (read(fd[0], string, sizeof(string)) > 0) {
                    printf("%s\n", string);
            }
            printf("PARENT END\n");
            close(fd[0]);
    }
    else {
            close(fd[0]);
            char string[100];
            string[0] = '\0';
            printf("CHILD START\n");
            for (int i = 0; i < 5; i++) {
                    sprintf(string, "%d", i);
                    write(fd[1], string, strlen(string)+1);
            }
            printf("CHILD END\n");
            close(fd[1]);
            exit(0);
    }
}