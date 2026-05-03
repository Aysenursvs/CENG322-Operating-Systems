#include "builtins.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/* checks if the command is a built-in name */
int is_builtin(char *cmd) {
    /* compare cmd against known built-in names */
    /* return 1 if match, 0 otherwise */
    return (strcmp(cmd, "cd")      == 0 ||
            strcmp(cmd, "pwd")     == 0 ||
            strcmp(cmd, "mkdir")   == 0 ||
            strcmp(cmd, "rmdir")   == 0 ||
            strcmp(cmd, "history") == 0 ||
            strcmp(cmd, "exit")    == 0);
}

/* routes to the correct built-in function based on args[0] */
void run_builtin(char **args) {
    /* check args[0] and call the right function */
    if (args == NULL || args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "cd") == 0) {
        builtin_cd(args);
    } else if (strcmp(args[0], "pwd") == 0) {
        builtin_pwd();
    } else if (strcmp(args[0], "mkdir") == 0) {
        builtin_mkdir(args);
    } else if (strcmp(args[0], "rmdir") == 0) {
        builtin_rmdir(args);
    } else if (strcmp(args[0], "history") == 0) {
        builtin_history();
    } else if (strcmp(args[0], "exit") == 0) {
        builtin_exit();
    }
}

/* changes current directory, with HOME fallback and PWD update */
void builtin_cd(char **args) {
    /* if no argument, go to HOME */
    /* otherwise call chdir() */
    /* update PWD environment variable */
    const char *target;

    if (args[1] == NULL) {
        /* no argument, go to HOME */
        target = getenv("HOME");
    } else if (strcmp(args[1], "~") == 0) {
        /* ~ means HOME directory */
        target = getenv("HOME");
    } else {
        target = args[1];
    }

    if (target == NULL) {
        fprintf(stderr, "cd: HOME not set\n");
        return;
    }

    if (chdir(target) != 0) {
        perror("cd");
        return;
    }

    /* update PWD environment variable to match new directory */
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        setenv("PWD", cwd, 1);
    }
}

/* prints current working directory using getcwd */
void builtin_pwd() {
    /* call getcwd() and print result */
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

/* creates one or more directories (up to 10 names) */
void builtin_mkdir(char **args) {
    /* loop through args (up to 10) */
    /* call mkdir() for each */
    for (int i = 1; args[i] != NULL && i <= 10; i++) {
        /* 0755: owner rwx, group rx, others rx */
        if (mkdir(args[i], 0755) != 0) {
            perror("mkdir");
        }
    }
}

/* removes an empty directory; fails if it is not empty */
void builtin_rmdir(char **args) {
    /* call rmdir() with args[1] */
    if (args[1] == NULL) {
        fprintf(stderr, "rmdir: missing operand\n");
        return;
    }

    /* rmdir only removes empty directories */
    if (rmdir(args[1]) != 0) {
        perror("rmdir");
    }
}

/* prints command history, including the history command itself */
void builtin_history() {
    /* call history_add() first, then history_print() */
    history_add("history");
    history_print();
}

/* exits the shell immediately */
void builtin_exit() {
    /* call exit(0) */
    exit(0);
}