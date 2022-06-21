/*********************
 * List.c
 * Author: Rishab Gaddi and Maheshbhai Chauhan
 * Date: 2018-11-19
 *********************/

#include "List.h"
#include <stdio.h>

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

status remFromListAt(List *l, int n, void *res)
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
  Node *tmp = prev->next;
  prev->next = tmp->next;
  *(void **)res = tmp->val;
  free(tmp);
  l->nelts--;
  return OK;
}

status remFromList(List *l, void *e)
{
  if (!l->comp)
    return ERRUNABLE;
  Node *prev = l->head;
  while (prev)
  {
    if (l->comp(prev->val, e) == 0)
    {
      Node *tmp = prev->next;
      prev->next = tmp->next;
      free(tmp);
      l->nelts--;
      return OK;
    }
    prev = prev->next;
  }
  return ERRABSENT;
}

status displayList(List *l)
{
  if (!l->pr)
    return ERRUNABLE;
  Node *node = l->head;
  printf("[ ");
  while (node)
  {
    l->pr(node->val);
    node = node->next;
    if (node)
      printf(" ");
  }
  printf(" ]\n");
  return OK;
}

void forEach(List *l, void (*f)(void *))
{
  Node *node = l->head;
  while (node)
  {
    f(node->val);
    node = node->next;
  }
}

int lengthList(List *l)
{
  return l->nelts;
}

status addList(List *l, void *e)
{
  if (!l->comp)
    return ERRUNABLE;
  Node *node = malloc(sizeof(Node));
  if (!node)
    return ERRALLOC;
  node->val = e;
  Node *prev = l->head;
  if (!prev)
  {
    node->next = 0;
    l->head = node;
    l->nelts++;
    return OK;
  }
  while (prev)
  {
    if (l->comp(prev->val, e) == 0)
    {
      free(node);
      return ERREXIST;
    }
    if (l->comp(prev->val, e) > 0)
    {
      node->next = prev->next;
      prev->next = node;
      l->nelts++;
      return OK;
    }
    if (!prev->next)
      break;
  }
  node->next = 0;
  prev->next = node;
  l->nelts++;
  return OK;
}

Node *isInList(List *l, void *e)
{
  Node *node = l->head;
  if (!node)
    return 0;
  if (l->comp(node->val, e) == 0)
    return node;
  while (node->next)
  {
    if (l->comp(node->next->val, e) == 0)
      return node->next;
    node = node->next;
  }
  return 0;
}

void *firstThat(List *l, int (*f)(void *))
{
  Node *node = l->head;
  while (node)
  {
    if (f(node->val))
      return node->val;
    node = node->next;
  }
  return 0;
}

List *allThat(List *l, int (*f)(void *))
{
  List *resultList = newList(l->comp, l->pr);
  Node *node = l->head;
  while (node)
  {
    if (f(node->val))
      addList(resultList, node->val);
    node = node->next;
  }
  return resultList;
}
