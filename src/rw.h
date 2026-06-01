#ifndef _RW_H
#define _RW_H

#include "menu.h"

#define READ_PATH "sd:/dump.pak"
#define WRITE_PATH "sd:/write.pak"

typedef enum {
    READ,
    WRITE,
} action_t;

extern const menu_opt_t pak_size_menu[];

action_t get_action(void);

#endif
