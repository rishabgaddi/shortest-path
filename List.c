/*********************
 * List.c
 * Description: Implementation of A* algorithm
 * Author: Rishab Gaddi and Maheshbhai Chauhan
 * Date: 2018-11-19
 *********************/

#include "List.h"

List *newList(compFun comp, prFun pr)
{
  List *l = malloc(sizeof(List));
  if (!l)
    return 0;
  l->nelts = 0;
  l->head = 0;
  l->comp = comp;
  l->pr = pr;
  return l;
}

void delList(List *l)
{
  Node *n = l->head;
  while (n)
  {
    Node *tmp = n;
    n = n->next;
    free(tmp);
  }
  free(l);
}

status nthInList(List *l, int n, void *res)
{
  Node *node = l->head;
  int i = 1;
  while (node && i < n)
  {
    node = node->next;
    i++;
  }
  if (!node)
    return ERRINDEX;
  *(void **)res = node->val;
  return OK;
}

status addListAt(List *l, int n, void *e)
{
  Node *node = malloc(sizeof(Node));
  if (!node)
    return ERRALLOC;
  node->val = e;
  if (n == 0)
  {
    node->next = l->head;
    l->head = node;
  }
  else
  {
    Node *prev = l->head;
    int i = 1;
    while (prev && i < n)
    {
      prev = prev->next;
      i++;
    }
    if (!prev)
      return ERRINDEX;
    node->next = prev->next;
    prev->next = node;
  }
  l->nelts++;
  return OK;
}
