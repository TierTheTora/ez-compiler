#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ezscr file.ez\n");
        return 1;
    }

    char *ez = argv[1];
    #ifdef __unix__
    char chmod_command[256];
    snprintf(chmod_command, sizeof(chmod_command), "chmod +x %s", ez);
    int status = system(chmod_command);
    if (status == -1) {
        perror("Error running chmod command");
        return 1;
    }
    #endif

    FILE *tmp_file = fopen("tmp_input.txt", "w");
    if (tmp_file == NULL) {
        perror("Error creating temporary input file");
        return 1;
    }
    fprintf(tmp_file, "%s\n", ez);
    fclose(tmp_file);

    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        FILE *tmp_input = fopen("tmp_input.txt", "r");
        if (tmp_input == NULL) {
            perror("Error opening temporary input file");
            exit(1);
        }

        if (dup2(fileno(tmp_input), 0) == -1) {
            perror("Error redirecting stdin");
            fclose(tmp_input);
            exit(1);
        }

        fclose(tmp_input);
        execl("./main", "./main", (char *)NULL);
        perror("Error executing ./main");
        exit(1);
    }

    waitpid(pid, &status, 0);
    remove("tmp_input.txt");

    return 0;
}
