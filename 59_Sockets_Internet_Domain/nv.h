#ifndef NV_H
#define NV_H

#define MAX_KEY 30
#define MAX_VAL 100
#define PAS_LEN 30

#define CMD_DEL 0
#define CMD_ADD 1
#define CMD_MOD 2
#define CMD_GET 3

struct request_msg {
    int command;
    char password[PAS_LEN];
    char key[MAX_KEY];
    char value[MAX_VAL];
};

#define REQ_SIZE sizeof(struct request_msg)

#endif
