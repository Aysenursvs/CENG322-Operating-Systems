#ifndef HISTORY_H
#define HISTORY_H

#define HISTORY_SIZE 10
#define MAX_CMD_LEN  101

// dışarıdan erişilecek fonksiyonlar
void history_add(char *cmd);
void history_print();

#endif