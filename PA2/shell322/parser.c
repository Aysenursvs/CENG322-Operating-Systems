#include "parser.h"
#include <string.h>
#include <stdlib.h>

/* fills an args array by tokenizing the given string by spaces */
static void tokenize(char *str, char **args) {
    char *token = strtok(str, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; /* null-terminate the array */
}

/* trims leading and trailing spaces in place */
static void trim_spaces(char *str) {
    size_t start = 0;
    size_t len = strlen(str);

    while (start < len && str[start] == ' ') {
        start++;
    }

    size_t end = len;
    while (end > start && str[end - 1] == ' ') {
        end--;
    }

    if (start > 0) {
        memmove(str, str + start, end - start);
    }
    str[end - start] = '\0';
}

ParsedCommand parse(char *input) {
    ParsedCommand cmd;
    /* initialize everything to NULL/0 */
    cmd.type = CMD_SYSTEM;
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd.args[i] = NULL;
        cmd.left_args[i] = NULL;
        cmd.right_args[i] = NULL;
    }
    cmd.background = 0;

    /* remove trailing newline if present */
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    trim_spaces(input);

    /* 1. check for && first */
    char *and_pos = strstr(input, "&&");
    if (and_pos != NULL) {
        /* split into left and right, tokenize each */
        *and_pos = '\0';
        char *right = and_pos + 2;
        trim_spaces(input);
        trim_spaces(right);
        tokenize(input, cmd.left_args);
        tokenize(right, cmd.right_args);
        cmd.type = CMD_AND;
        return cmd;
    }

    /* 2. check for | */
    char *pipe_pos = strchr(input, '|');
    if (pipe_pos != NULL) {
        *pipe_pos = '\0';
        char *right = pipe_pos + 1;
        trim_spaces(input);
        trim_spaces(right);
        tokenize(input, cmd.left_args);
        tokenize(right, cmd.right_args);
        cmd.type = CMD_PIPE;
        return cmd;
    }

    /* 3. check for & */
    len = strlen(input);
    if (len > 0 && input[len - 1] == '&') {
        input[len - 1] = '\0';
        trim_spaces(input);
        tokenize(input, cmd.args);
        cmd.type = CMD_BACKGROUND;
        cmd.background = 1;
        return cmd;
    }

    /* 4. normal command - check if builtin or system */
    tokenize(input, cmd.args);
    if (cmd.args[0] != NULL) {
        if (strcmp(cmd.args[0], "cd") == 0 ||
            strcmp(cmd.args[0], "pwd") == 0 ||
            strcmp(cmd.args[0], "mkdir") == 0 ||
            strcmp(cmd.args[0], "rmdir") == 0 ||
            strcmp(cmd.args[0], "history") == 0 ||
            strcmp(cmd.args[0], "exit") == 0) {
            cmd.type = CMD_BUILTIN;
        } else {
            cmd.type = CMD_SYSTEM;
        }
    }

    return cmd;
}