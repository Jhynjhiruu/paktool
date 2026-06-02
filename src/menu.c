#include <libdragon.h>

#include "menu.h"

static void __render(menu_opt_t const* entries, size_t num_entries, char const * header, size_t index) {
    console_clear();
    printf("%s", header);
    for (size_t i = 0; i < num_entries; i++) {
        printf(" %c %s\n", (i == index) ? '>' : ' ', entries[i].msg);
    }
    console_render();
}

int __render_menu(menu_opt_t const* entries, char const * header, size_t index) {
    bool render = true;
    bool sel = false;
    joypad_inputs_t prev = { 0 }, cur;
    size_t num_entries;

    joypad_poll();
    cur = joypad_get_inputs(JOYPAD_PORT_1);

    for (num_entries = 0; entries[num_entries].msg != NULL; num_entries++) {
        continue;
    }

    assertf(num_entries != 0, "No menu entries\n");

#define RISE(inp) ((cur.inp) && !(prev.inp))
#define UP() (RISE(btn.d_up) || RISE(stick_y >= 40))
#define DOWN() (RISE(btn.d_down) || RISE(stick_y <= -40))
#define SEL() (RISE(btn.a))
#define UNSEL() (RISE(btn.a == false))

    while (true) {
        joypad_poll();
        prev = cur;
        cur = joypad_get_inputs(JOYPAD_PORT_1);

        if (DOWN() && (index < (num_entries - 1))) {
            index++;
            render = true;
        } else if (UP() && (index > 0)) {
            index--;
            render = true;
        } else if (SEL()) {
            sel = true;
        } else if (UNSEL() && sel) {
            break;
        }

        if (render) {
            __render(entries, num_entries, header, index);
        }

        render = false;
    }

    return entries[index].opt;
}
