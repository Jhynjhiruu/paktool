#ifndef _MENU_H
#define _MENU_H

#include <stddef.h>

typedef struct {
    int opt;
    char const * msg;
} menu_opt_t;

#define render_menu(entries, header, ...) _render_menu(entries, header __VA_OPT__(,) __VA_ARGS__, 0)
#define _render_menu(entries, header, start_index, ...) __render_menu(entries, header, start_index)

int __render_menu(menu_opt_t const* entries, char const * header, size_t index);

#endif
