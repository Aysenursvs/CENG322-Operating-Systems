#include "parser.h"
#include <string.h>
#include <stdlib.h>

/*
 * Splits a command line into tokens by single spaces.
 * The resulting array is NULL-terminated for execvp compatibility.
 */
static void tokenize(char *str, char **args) {
    char *token = strtok(str, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; /* null-terminate the array */
}

/*
 * Removes leading and trailing spaces in place.
 * This keeps parsing stable when users type extra spaces.
 */
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

/*
 * Parses a raw input line into one of these cases:
 *  - CMD_AND: left && right
 *  - CMD_PIPE: left | right
 *  - CMD_BACKGROUND: command &
 *  - CMD_BUILTIN / CMD_SYSTEM: normal commands
 * Assumes at most one special operator in the line as per assignment.
 */
ParsedCommand parse(char *input) {
    ParsedCommand cmd;
    /* initialize all fields so the caller can safely use them */
    cmd.type = CMD_SYSTEM;
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd.args[i] = NULL;
        cmd.left_args[i] = NULL;
        cmd.right_args[i] = NULL;
    }
    cmd.background = 0;

    /* remove trailing newline so matching and execvp work cleanly */
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    trim_spaces(input);

    /* 1. check for && first to avoid matching single & */
    char *and_pos = strstr(input, "&&");
    if (and_pos != NULL) {
        /* split into left and right, then tokenize each side */
        *and_pos = '\0';
        char *right = and_pos + 2;
        trim_spaces(input);
        trim_spaces(right);
        tokenize(input, cmd.left_args);
        tokenize(right, cmd.right_args);
        cmd.type = CMD_AND;
        return cmd;
    }

    /* 2. check for | (single pipe only) */
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

    /* 3. check for & only at the end (background) */
    len = strlen(input);
    if (len > 0 && input[len - 1] == '&') {
        input[len - 1] = '\0';
        trim_spaces(input);
        tokenize(input, cmd.args);
        cmd.type = CMD_BACKGROUND;
        cmd.background = 1;
        return cmd;
    }

    /* 4. normal command - detect builtin vs system */
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