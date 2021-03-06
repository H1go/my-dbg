#include <stdio.h>
#include <stdlib.h>

#include "breakpoint.h"
#include "list.h"

struct list *list_init(void)
{
    struct list *l = malloc(sizeof(*l));
    if(!l)
        return NULL;
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

struct node *create_node(struct breakpoint *data)
{
    struct node *n = malloc(sizeof(*n));
    if(!n)
        return NULL;
    n->data = data;
    n->next = NULL;
    return n;
}

void list_push(struct list *list, struct breakpoint *data)
{
    if(!list)
        return;
    struct node *n = create_node(data);
    if(!n)
        return;

    ++list->size;
    if(list->size == 1)
    {
        list->head = n;
        list->tail = n;
        return;
    }
    n->next = list->head;
    list->head = n;
}

void list_pop(struct list *list)
{
    if(!list || list->size == 0)
        return;

    struct node *n = list->head;
    list->head = list->head->next;
    --list->size;

    if(list->size == 0)
        list->tail = NULL;
    free(n);
}

void list_print(struct list *list)
{
    char *types[3] = {"PERMANENT", "TEMPORARY", NULL};
    if (!list->size)
        return;

    struct node *node = list->head;
    for (; node != NULL ; node = node->next)
        printf("Breakpoint %d adress: 0x%lx type: %s\n", node->data->id,
               node->data->addr, types[node->data->type]);
}