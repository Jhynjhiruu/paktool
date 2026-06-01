#define _GNU_SOURCE
#include <libdragon.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "rw.h"
#include "util.h"

void pak_write(unsigned int banks, unsigned int filesystem_banks) {
    char * num_banks;
    if (asprintf(&num_banks, "pak reported %d bank%s", banks, (banks == 1) ? "" : "s") < 0) {
        error("Failed to format string: %s (%d)\n", strerror(errno), errno);

        INFLOOP;
    }

    char * num_fs_banks;
    if (asprintf(&num_fs_banks, "filesystem reported %d bank%s", filesystem_banks, (filesystem_banks == 1) ? "" : "s") < 0) {
        error("Failed to format string: %s (%d)\n", strerror(errno), errno);

        INFLOOP;
    }

    char * num_banks_string;
    if (asprintf(&num_banks_string, "\n(%s%s%s)", (banks == 0) ? "" : num_banks, ((banks == 0) || (filesystem_banks == 0)) ? "" : ", ", (filesystem_banks == 0) ? "" : num_fs_banks) < 0) {
        error("Failed to format string: %s (%d)\n", strerror(errno), errno);

        INFLOOP;
    }

    free(num_banks);
    free(num_fs_banks);

    char * header;
    if (asprintf(&header, "Select number of banks to write%s\n\n", ((banks == 0) && (filesystem_banks == 0)) ? "" : num_banks_string) < 0) {
        error("Failed to format string: %s (%d)\n", strerror(errno), errno);

        INFLOOP;
    }

    free(num_banks_string);

    size_t start_index = 0;

    for (size_t i = 0; pak_size_menu[i].msg != NULL; i++) {
        if ((unsigned int)pak_size_menu[i].opt == banks) {
            start_index = i;
            break;
        }
    }

    const unsigned int banks_to_write = (unsigned int)render_menu(pak_size_menu, header, start_index);

    free(header);

    errno = 0;
    const int f = open(WRITE_PATH, O_RDONLY);
    if (f < 0) {
        error("Failed to open " WRITE_PATH ": %s (%d)\n", strerror(errno), errno);

        INFLOOP;
    }

    uint8_t buf[BANK_SIZE];

    for (unsigned int bank = 0; bank < banks_to_write; bank++) {
        info("Writing bank %d\n", bank);

        size_t done = 0;
        while (done < sizeof(buf)) {
            const size_t left = sizeof(buf) - done;
            ssize_t res;

            errno = 0;
            res = read(f, buf + done, left);
            if (res == 0) {
                memset(buf + done, 0, left);
                done += left;
            } else if (res < 0) {
                error("Failed to read " WRITE_PATH ": %s (%d)\n", strerror(errno), errno);

                INFLOOP;
            } else {
                done += (size_t)res;
            }
        }

        errno = 0;
        int res = cpak_write(JOYPAD_PORT_1, (uint8_t)bank, 0x0000, buf, sizeof(buf));
        if (res < (int)sizeof(buf)) {
            error("Failed to write bank %d", bank);

            if (res < 0) {
                error(": %s (%d)", strerror(errno), errno);
            }

            error("\n");

            INFLOOP;
        }
    }

    if (close(f) < 0) {
        error("Failed to close " WRITE_PATH ": %s (%d)\n", strerror(errno), errno);
        // not fatal
    }
}
