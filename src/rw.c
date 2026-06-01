#include <libdragon.h>

#include "rw.h"

#include "menu.h"
#include "util.h"

const menu_opt_t pak_size_menu[] = {
    { 1, "1 bank (32KiB, 123 pages, standard)" },
    { 2, "2 banks (64KiB, 248 pages, double)" },
    { 4, "4 banks (128KiB, 498 pages, Datel/Blaze 1 meg)" },
    { 16, "16 banks (512KiB, 1998 pages, Datel 4 meg)" },
    { 62, "62 banks (1984KiB, 7748 pages, max formatted capacity)" },
    { 256, "256 banks (2MiB, max raw capacity)" },
    { 0, NULL },
};

action_t get_action(void) {
    const menu_opt_t menu[] = {
        { READ,  "Dump pak to " READ_PATH     },
        { WRITE, "Write pak from " WRITE_PATH },
        { 0,     NULL },
    };

    console_clear();

    return render_menu(menu, NAME "\n\n\nUp/down to move, A to select\n");
}
