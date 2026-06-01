#ifndef _UTIL_H
#define _UTIL_H

#define INFLOOP do {} while (true)

#define info(fmt, ...) console_set_debug(true); printf(fmt __VA_OPT__(,) __VA_ARGS__); console_render(); console_set_debug(false)
#define error(fmt, ...) console_set_debug(true); printf("[Error] " fmt __VA_OPT__(,) __VA_ARGS__); console_render(); console_set_debug(false)

#define NAME "Controller pak tool"

#define BANK_SIZE (32 * 1024)

#endif
