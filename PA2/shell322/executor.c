#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* executes a simple foreground or background system command */
void exec_simple(char **args, int background) {
    /* fork a child process */
    /* child: call execvp */
    /* parent: if foreground wait, if background print pid */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    }

    if (background) {
        printf("%d\n", pid);
    } else {
        waitpid(pid, NULL, 0);
    }
}

/* executes two commands connected with a pipe | */
void exec_pipe(char **left_args, char **right_args) {
    /* create a pipe with pipe() */
    /* fork first child for left command */
        /* child: redirect stdout to pipe write end using dup2 */
        /* child: close unused pipe ends */
        /* child: execvp left command */
    /* fork second child for right command */
        /* child: redirect stdin to pipe read end using dup2 */
        /* child: close unused pipe ends */
        /* child: execvp right command */
    /* parent: close both pipe ends */
    /* parent: wait for both children */

    if (left_args[0] == NULL || right_args[0] == NULL) {
        fprintf(stderr, "shell322: invalid pipe command\n");
        return;
    }


    int fds[2];
    if (pipe(fds) != 0) {
        perror("pipe");
        return;
    }

    pid_t left_pid = fork();
    if (left_pid < 0) {
        perror("fork");
        return;
    }

    if (left_pid == 0) {
        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]);
        close(fds[1]);
        execvp(left_args[0], left_args);
        perror("execvp");
        exit(1);
    }

    pid_t right_pid = fork();
    if (right_pid < 0) {
        perror("fork");
        close(fds[0]);
        close(fds[1]);
        waitpid(left_pid, NULL, 0);  // sol child'ı temizle
        return;
    }

    if (right_pid == 0) {
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);
        close(fds[1]);
        execvp(right_args[0], right_args);
        perror("execvp");
        exit(1);
    }

    close(fds[0]);
    close(fds[1]);
    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}

/* executes second command only if first exits with status 0 */
void exec_and(char **first_args, char **second_args) {
    /* fork and execute first command */
    /* wait and get exit status */
    /* if exit status is 0, fork and execute second command */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        execvp(first_args[0], first_args);
        perror("execvp");
        exit(1);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        pid_t pid2 = fork();
        if (pid2 < 0) {
            perror("fork");
            return;
        }

        if (pid2 == 0) {
            execvp(second_args[0], second_args);
            perror("execvp");
            exit(1);
        }

        waitpid(pid2, NULL, 0);
    }
}