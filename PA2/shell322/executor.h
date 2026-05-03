#ifndef EXECUTOR_H
#define EXECUTOR_H

/* executes a simple system command, background=1 means no wait */
void exec_simple(char **args, int background);

/* executes two commands connected with a pipe */
void exec_pipe(char **left_args, char **right_args);

/* executes second command only if first succeeds */
void exec_and(char **first_args, char **second_args);

#endif