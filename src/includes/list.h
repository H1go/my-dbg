#ifndef LIST_H
#define LIST_H

#include "breakpoint.h"

struct list
{
  size_t size;           /*!<Size of the list. */
  struct node *head;  /*!<First element of the list. */
  struct node *tail;  /*!<Last element of the list. */
};

struct node
{
  struct node *next;  /*!<Pointer to the next element. */
  struct breakpoint *data;
};

struct list *list_init();

struct node *create_l_node(struct breakpoint *data);

void list_push(struct list *list, struct breakpoint *data);

void clear_nodes(struct list *list);

void node_destroy(struct node *node);

void list_destroy(struct list *list);

struct breakpoint *list_get(struct list *list, uintptr_t addr);

void list_print(struct list *list);

#endif /* !LIST_H */
