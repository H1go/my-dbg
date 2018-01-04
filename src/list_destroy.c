#include <stdlib.h>

#include "breakpoint.h"
#include "list.h"

void node_destroy(struct node *node)
{
    free(node->data);
    free(node);
}

void clear_nodes(struct list *list)
{
    if(!list)
        return;
    struct node *l = list->head;
    while (l)
    {
        struct node *tmp = l;
        l = l->next;
        node_destroy(tmp);
    }
}

void list_destroy(struct list *list)
{
    if(!list)
        return;
    clear_nodes(list);
    free(list);
    list = NULL;
}