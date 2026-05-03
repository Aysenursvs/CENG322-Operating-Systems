#include "history.h"
#include <stdio.h>
#include <string.h>

/* counter that tracks how many commands have been entered */
static int   history_count = 0;

/* array that stores commands - hidden from outside (static) */
static char  history[HISTORY_SIZE][MAX_CMD_LEN];


/* adds command to history */
void history_add(char *cmd) {

    /* is capacity full? */
    if (history_count == HISTORY_SIZE) {
        /* shift the array */
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        /* decrease counter */
        history_count--;
    }

    /* add new command */
    strncpy(history[history_count], cmd, MAX_CMD_LEN - 1);
    history[history_count][MAX_CMD_LEN - 1] = '\0';

    /* update counter */
    history_count++;
}


/* prints history to screen */
void history_print() {

    /* print each command in [number] command format */
    for (int i = 0; i < history_count; i++) {
        printf("[%d] %s\n", i + 1, history[i]);
    }
}