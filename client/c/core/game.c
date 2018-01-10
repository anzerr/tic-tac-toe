#include "header.h"

int charToInt(char c) {
    return (c - 48);
}

void displayGame(t_core *core) {
    int x, y;

    write(1, "\033[H\0", 5);
    write(1, "\033[2J\0", 5);
    write(1, (core->id == core->turn) ? "\x1b[32m" : "\x1b[31m", 5);
    my_putstr("\n---\n");
    x = 0;
    while (x < MAP_SIZE) {
        y = 0;
        while (y < MAP_SIZE) {
            if (core->map[x][y]) {
                my_putstr((core->map[x][y] == 1) ? "x" : "o");
            } else {
                my_putstr(" ");
            }
            y += 1;
        }
        my_putstr("\n");
        x += 1;
    }
    my_putstr("---\n");
}

void runGamePacket(t_core *core, char *pack) {
    char buffer[4096];
    int i, x, y, size;
    char **tab;

    my_putstr(pack);
    my_putstr("\n");

    my_memset(buffer, 0, 4096);
    i = (x = 0);
    while (pack[i - 1] != '\0' || (i == 0 && pack[i] != '\0')) {
        if (pack[i] == ';' || pack[i] == '\0') {
            if (buffer[0] == 'm') {
                tab = my_str_to_wordtab(buffer);
                y = 0;
                size = my_getnbr(tab[1]);
                while (tab[y + 2]) {
                    core->map[(int)(y / size)][y % size] = my_getnbr(tab[y + 2]);
                    y += 1;
                }
            }
            if (buffer[0] == 't') {
                core->turn = buffer[2] - 48;
                my_putstr("turn: ");
                my_putstr(my_nbrtostr(core->turn));
                my_putstr("\n");
            }
            if (buffer[0] == 'w') {
                core->running = 0;
            }
            if (buffer[0] == 'i') {
                core->id = buffer[2] - 48;
                my_putstr("id: ");
                my_putstr(my_nbrtostr(core->id));
                my_putstr("\n");
            }

            my_memset(buffer, 0, 4096);
            x = 0;
        } else {
            buffer[x] = pack[i];
            x += 1;
        }
        i += 1;
    }

    if (core->running) {
        displayGame(core);
    }
}
