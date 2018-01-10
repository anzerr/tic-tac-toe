#include "header.h"

int move(t_core *core, char *line) {
    my_strlen(line);
    write(core->sockfd, line, my_strlen(line));
    return (1);
}
