#include "history.h"
#include <stdio.h>
#include <string.h>

/* counter that tracks how many commands are stored */
static int   history_count = 0;

/* fixed-size FIFO buffer for command history */
static char  history[HISTORY_SIZE][MAX_CMD_LEN];


/* adds a command to history, keeping only the most recent 10 */
void history_add(char *cmd) {

    /* if full, shift left to drop the oldest entry */
    if (history_count == HISTORY_SIZE) {
        /* shift the array */
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            strcpy(history[i], history[i + 1]);
        }
        /* decrease counter */
        history_count--;
    }

    /* add new command at the end */
    strncpy(history[history_count], cmd, MAX_CMD_LEN - 1);
    history[history_count][MAX_CMD_LEN - 1] = '\0';

    /* update counter */
    history_count++;
}


/* prints history in issue order with [1]..[N] */
void history_print() {

    /* print each command in [number] command format */
    for (int i = 0; i < history_count; i++) {
        printf("[%d] %s\n", i + 1, history[i]);
    }
}