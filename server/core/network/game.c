#include "../header.h"

void displayGame(t_core *core) {
    int x, y;

    put("\n---\n");
    x = 0;
    while (x < 3) {
        y = 0;
        while (y < 3) {
            if (core->game->map[x][y]) {
                put((core->game->map[x][y] == 1) ? "x" : "o");
            } else {
                put(" ");
            }
            y += 1;
        }
        put("\n");
        x += 1;
    }
    put("---\n");
}

int createMap(int ***map, int size) {
    int i;

    i = 0;
    if ((*map = malloc(sizeof(int *) * size)) == NULL)
    return (0);
    while (i < size) {
        if (((*map)[i] = malloc(sizeof(int) * size)) == NULL)
        return (0);
        i++;
    }

    return (1);
}

t_game *initGame(t_core *core) {
    t_game *game;
    int i, x;

    if ((game = malloc(sizeof(t_game))) == NULL)
    return (NULL);

    game->core = core;
    i = 0;
    while (i < MAX_PLAYER) {
        game->players[i] = NULL;
        i += 1;
    }
    game->turn = 1;

    createMap(&game->map, core->size);
    i = 0;
    while (i < core->size) {
        x = 0;
        while (x < core->size) {
            game->map[i][x] = 0;
            x += 1;
        }
        i += 1;
    }

    return (game);
}

char *mapToString(t_core *core) {
    char *info, *tmp;
    int size, i, x;

    size = 2 + ((core->size < 10)? 2 : 3) + 2;
    i = 0;
    while (i < core->size) {
        x = 0;
        while (x < core->size) {
            size += ((core->game->map[i][x] < 10)? 2 : 3);
            x += 1;
        }
        i += 1;
    }

    if ((info = malloc(sizeof(char) * size)) == NULL)
    return (NULL);

    my_memset(info, 0, size);
    my_strcat(info, "m,");
    tmp = my_nbrtostr(core->size);
    my_strcat(info, tmp);
    free(tmp);
    my_strcat(info, ",");
    i = 0;
    while (i < core->size) {
        x = 0;
        while (x < core->size) {
            if (x != 0 || i != 0) {
                my_strcat(info, ",");
            }
            my_strcat(info, my_nbrtostr(core->game->map[i][x]));
            x += 1;
        }
        i += 1;
    }
    my_strcat(info, ";");

    put(core, my_nbrtostr(size));
    put(core, "\n");
    put(core, my_nbrtostr(my_strlen(info)));
    put(core, "\n");

    return (info);
}

void sendGameState(t_core *core) {
    char *tmp, *tmp2;

    tmp = mapToString(core);
    sendAll(core, tmp, my_strlen(tmp));
    free(tmp);

    if ((tmp = malloc(sizeof(char) * 4)) == NULL)
    return;

    my_memset(tmp, 0, 5);
    my_strcat(tmp, "t,");
    tmp2 = my_nbrtostr(core->game->turn);
    my_strcat(tmp, tmp2);
    my_strcat(tmp, ";");
    sendAll(core, tmp, my_strlen(tmp));
    free(tmp2);
    free(tmp);
}

void hasWon(t_core *core) {
    int x[8][3] = {
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 0, 0},
        {1, 1, 1},
        {2, 2, 2},
        {0, 1, 2},
        {0, 1, 2}
    }, y[8][3] = {
        {0, 0, 0},
        {1, 1, 1},
        {2, 2, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2},
        {2, 1, 0}
    }, full, size, i, v, c;

    size = 8;
    full = 1;
    i = 0;
    while (i < size) {
        v = (c = 0);
        while (v < core->size) {
            if (core->game->map[y[i][v]][x[i][v]] == 0) {
                c = 0;
                full = 0;
                break;
            }
            if (v == 0) {
                c = core->game->map[y[i][v]][x[i][v]];
            } else {
                if (c != core->game->map[y[i][v]][x[i][v]]) {
                    c = 0;
                    break;
                }
            }
            v += 1;
        }

        if (c != 0) {
            core->running = 0;
            sendAll(core, (c == 1) ? "w,1;" : "w,2;", 5);
            return;
        }

        i += 1;
    }

    if (full) {
        core->running = 0;
        sendAll(core, "w,0;", 5);
    }
}

void gameAction(t_core *core, int key, char *request) {
    int code;

    put(core, request);
    put(core, "\n");

    my_putstr("server request :");
    my_putstr(request);
    my_putstr("- \n");
    if (request[0] == 't') {
        code = a_placeTick(core, key, request);
        sendGameState(core);
    }

    if (core->debug) {
        displayGame(core);
    }

    hasWon(core);
    put(core, "after action\n");

    if (core->game->players[key] != NULL) {
        put(core, (code) ? "OK\n" : "KO\n");
        my_putstr((code) ? "OK\n" : "KO\n");
        if (code) {
            sendPayload(core->game->players[key]->socket, "OK;", 3);
        } else {
            sendPayload(core->game->players[key]->socket, "KO;", 3);
        }
    }
}
