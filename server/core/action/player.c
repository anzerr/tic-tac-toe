#include "../header.h"

int inMap(t_core *core, int x) {
    return (core->size > x && x >= 0);
}

int playerTurn(t_core *core, int key, int x, int y) {
    // is it his turn and can he place anything there
    if (core->game->players[key] && core->game->turn == key) {
        if (inMap(core, x) && inMap(core, y) && core->game->map[x][y] == 0) {
            core->game->map[x][y] = key + 1;
            return (1);
        }
    }
    return (0);
}

int a_placeTick(t_core *core, int key, char *request) {
    char **tab;
    int i, x, z1, z2;

    z1 = (z2 = -1);
    tab = my_str_to_wordtab(request);
    i = (x = 0) + (key - key);
    while (tab[i] != NULL) {
        if (i == 1 || i == 2) {
            if (i == 1) {
                z1 = my_getnbr(tab[i]);
            }
            if (i == 2) {
                z2 = my_getnbr(tab[i]);
            }
        }
        i += 1;
    }

    return  playerTurn(core, key, z1, z2);
}
