#include "header.h"

void my_sort_list(t_list **begin, int (*cmp)())
{
    t_list *prev;
    t_list *link;
    t_list *next;

    link = *begin;
    prev = NULL;
    while (link != NULL)
    {
        next = link->next;
        if (next != NULL && cmp(link->data, next->data) > 0)
        {
            link->next = next->next;
            next->next = link;
            if (prev == NULL)
            *begin = next;
            else
            prev->next = next;
            link = *begin;
            prev = NULL;
        }
        else
        link = (prev = link)->next;
    }
}

t_list *my_add_list(t_list **begin)
{
    t_list *tmp;

    if ((tmp = malloc(sizeof(t_list))) == NULL)
    return (NULL);
    tmp->next = *begin;
    *begin = tmp;
    return (tmp);
}

void my_remove_list(t_list **begin, void *data_ref, int (*cmp)())
{
    t_list *list;
    t_list *node;

    list = *begin;
    node = NULL;
    while (list != NULL)
    {
        if (cmp(list->data, data_ref) == 0)
        {
            if (node == NULL)
            *begin = list->next;
            else
            node->next = list->next;
            free(list);
        }
        node = list;
        list = list->next;
    }
}

void *my_find_list(t_list *begin, void *data_ref, int (*cmp)())
{
    t_list *list;
    int i;

    list = begin;
    i = 0;
    while (list != NULL)
    {
        my_putstr(my_nbrtostr(i));
        my_putstr("\n");
        i += 1;
        if (cmp(list->data, data_ref) == 0)
        {
            return (list->data);
        }
        list = list->next;
    }
    return (NULL);
}
