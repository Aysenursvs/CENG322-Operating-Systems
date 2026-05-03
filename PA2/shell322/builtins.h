#ifndef BUILTINS_H
#define BUILTINS_H

/* checks if the command is a built-in, returns 1 if yes 0 if no */
int  is_builtin(char *cmd);

/* routes to the correct built-in function */
void run_builtin(char **args);

/* built-in command implementations */
void builtin_cd(char **args);
void builtin_pwd();
void builtin_mkdir(char **args);
void builtin_rmdir(char **args);
void builtin_history();
void builtin_exit();

#endif