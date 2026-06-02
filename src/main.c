#define _GNU_SOURCE
#include <libdragon.h>

#include <errno.h>

#include "read.h"
#include "rw.h"
#include "util.h"
#include "write.h"

int main(void) {
    console_init();
    console_set_render_mode(RENDER_MANUAL);

    debug_init(DEBUG_FEATURE_LOG_USB | DEBUG_FEATURE_LOG_EMU);

    if (sys_bbplayer()) {
        error("This program cannot be used on an iQue Player\n");

        INFLOOP;
    }

    joypad_init();

    if (!debug_init(DEBUG_FEATURE_FILE_SD)) {
        error("SD card not found\n");

        INFLOOP;
    }

    bool found = false;
    while (!found) {
        console_clear();

        printf(NAME "\n");

        joypad_poll();

        switch (joypad_get_accessory_type(JOYPAD_PORT_1)) {
            case JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK: {
                printf("Found controller pak\n");
                console_render();
                found = true;
            } break;

            case JOYPAD_ACCESSORY_TYPE_NONE: {
                printf("No accessory in controller port 1\n");
                console_render();
            } break;

            default: {
                printf("Wrong accessory in controller port 1\n");
                console_render();
            } break;
        }
    }

    errno = 0;
    int banks = cpak_probe_banks(JOYPAD_PORT_1);
    if (banks < 0) {
        switch (errno) {
            case EIO: {
                error("Failed to probe banks from pak\n");
                // not fatal
                banks = 0;
            } break;

            default: {
                error("Unexpected error: %s (%d)\n", strerror(errno), errno);
                // fatal
                INFLOOP;
            } break;
        }
    } else if (banks == 0) {
        error("Controller pak reports 0 banks\n");
        // fatal, since libdragon should never report 0 banks
        INFLOOP;
    }

    errno = 0;
    unsigned int filesystem_banks = 0;
    if (cpakfs_mount(JOYPAD_PORT_1, "cpak:/") < 0) {
        switch (errno) {
            case EIO:
            case ENXIO: {
                error("Failed to probe banks from filesystem (check to make sure the pak is plugged in and not damaged)\n");
                // fatal
                INFLOOP;
            } break;

            case ENODEV: {
                info("Controller pak does not contain a valid filesystem\n");
                // not fatal
            } break;
        }
    } else {
        cpakfs_stats_t stats;

        errno = 0;
        if (cpakfs_get_stats(JOYPAD_PORT_1, &stats) < 0) {
            error("Get stats failed: %s (%d)\n", strerror(errno), errno);
            // not fatal
        } else {
            assert((stats.num_banks >= 0) && (stats.num_banks <= 256));
            filesystem_banks = (unsigned int)stats.num_banks;
        }

        errno = 0;
        if (cpakfs_unmount(JOYPAD_PORT_1) < 0) {
            error("Unmount failed: %s (%d)\n", strerror(errno), errno);
            // not fatal
        }
    }

    const action_t action = get_action();

    switch (action) {
        case READ: {
            pak_read((unsigned int)banks, filesystem_banks);
        } break;

        case WRITE: {
            pak_write((unsigned int)banks, filesystem_banks);
        } break;
    }

    INFLOOP;
}
