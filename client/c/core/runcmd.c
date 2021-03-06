#include "header.h"

void addCmd(t_list **cmd, char *com, int (*func)())
{
    t_command *tmp;
    t_list *node;

    if ((tmp = malloc(sizeof(t_command))) == NULL)
    return;
    tmp->command = my_strdup(com);
    tmp->func = func;
    node = my_add_list(cmd);
    node->data = tmp;
}

void initCmd(t_core *c)
{
    addCmd(&c->cmd, "t", &move);
}

int runCmd(char *line, t_core * core) {
    t_command *tmp;
    t_list *list;
    int run;

    list = core->cmd;
    run = (my_strncmp(line, "/bye", 4) != 0);
    if (run) {
        my_putstr(line);
        tmp = (t_command*)my_find_list(list, line, &cmpcommand);

        if (tmp != NULL && tmp->func != NULL) {
            if (tmp->func(core, line)) {
                return (1);
            }
        }
    }
    my_putstr((run) ? "/ko\n" : "This isn't brave. It's murder.\n");
    return (0);
}
