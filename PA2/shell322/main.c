#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "builtins.h"
#include "executor.h"
#include "history.h"

static int is_blank_line(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            return 0;
        }
    }
    return 1;
}

int main() {
    char input[MAX_CMD_LEN];
    char raw[MAX_CMD_LEN];

    while (1) {
        /* print empty line before prompt as required */
        printf("\n");

        /* print prompt */
        printf("shell322> ");
        fflush(stdout);

        /* read input */
        if (fgets(input, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }

        /* skip empty input */
        if (is_blank_line(input)) {
            continue;
        }

        strncpy(raw, input, MAX_CMD_LEN - 1);
        raw[MAX_CMD_LEN - 1] = '\0';

        

        /* strip trailing newline for history */
        size_t raw_len = strlen(raw);
        if (raw_len > 0 && raw[raw_len - 1] == '\n') {
            raw[raw_len - 1] = '\0';
        }

        /* parse the input */
        ParsedCommand cmd = parse(input);

        if (cmd.type == CMD_SYSTEM && cmd.args[0] == NULL) {
            continue;
        }

        /* add to history (except history command, it handles itself) */
        if (!(cmd.type == CMD_BUILTIN && cmd.args[0] != NULL &&
              strcmp(cmd.args[0], "history") == 0)) {
            history_add(raw);
        }

        /* route to correct handler based on command type */
        switch (cmd.type) {
            case CMD_BUILTIN:
                run_builtin(cmd.args);
                break;
            case CMD_SYSTEM:
                exec_simple(cmd.args, 0);
                break;
            case CMD_BACKGROUND:
                exec_simple(cmd.args, 1);
                break;
            case CMD_PIPE:
                exec_pipe(cmd.left_args, cmd.right_args);
                break;
            case CMD_AND:
                exec_and(cmd.left_args, cmd.right_args);
                break;
        }
    }

    return 0;
}