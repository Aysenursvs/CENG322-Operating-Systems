#ifndef PARSER_H
#define PARSER_H

#define MAX_ARGS    11   /* max 10 arguments + NULL */
#define MAX_CMD_LEN 101

typedef enum {
    CMD_BUILTIN,
    CMD_SYSTEM,
    CMD_PIPE,
    CMD_AND,
    CMD_BACKGROUND
} CommandType;

typedef struct {
    CommandType type;
    char *args[MAX_ARGS];
    char *left_args[MAX_ARGS];
    char *right_args[MAX_ARGS];
    int  background;
} ParsedCommand;

ParsedCommand parse(char *input);

#endif