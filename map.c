#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

City *addCity(List *l, char *name, int latitude, int longitude)
{
  City *city = getCity(l, name);
  if (!city)
  {
    city = (City *)malloc(sizeof(City));
    if (!city)
      return 0;
    city->name = (char *)malloc(strlen(name) + 1);
    if (!city->name)
    {
      free(city);
      return 0;
    }
    strcpy(city->name, name);
    city->neighbors = newList(l->comp, l->pr);
    if (!city->neighbors)
    {
      free(city->name);
      free(city);
      return 0;
    }
    city->latitude = latitude;
    city->longitude = longitude;
    city->distFromStart = 0;
    city->distToGoal = 0;
    city->prev = 0;
    status res = addList(l, city);
    if (res != OK)
    {
      delList(city->neighbors);
      free(city->name);
      free(city);
      return 0;
    }
  }
  else
  {
    city->latitude = latitude;
    city->longitude = longitude;
  }
  return city;
}

City *getCity(List *l, char *name)
{
  if (l->nelts == 0)
    return 0;
  Node *node = l->head;
  while (node)
  {
    City *city = node->val;
    if (strcmp(city->name, name) == 0)
      return city;
    node = node->next;
  }
  return 0;
}

status addNeighbour(List *l, City *city, char *name, int distance)
{
  Neighbor *neighbor = malloc(sizeof(neighbor));
  if (!neighbor)
    return ERRALLOC;
  City *neighborCity = getCity(l, name);
  if (!neighborCity)
  {
    neighborCity = addCity(l, name, 0, 0);
    if (!neighborCity)
      return ERRALLOC;
  }
  neighbor->city = neighborCity;
  neighbor->distance = distance;
  status res = addList(city->neighbors, neighbor);
  if (res != OK)
    return ERRALLOC;
  return OK;
}

void displayAllCities(List *l)
{
  Node *node = l->head;
  while (node)
  {
    City *city = node->val;
    printf("%s\n", city->name);
    node = node->next;
  }
}

void displayNeighbours(List *l, char *name)
{
  City *city = getCity(l, name);
  if (!city)
    return;
  Node *node = city->neighbors->head;
  while (node)
  {
    Neighbor *neighbor = node->val;
    printf("%s %d\n", neighbor->city->name, neighbor->distance);
    node = node->next;
  }
}

void calculateDistanceToGoal(List *l, char *destinationCity)
{
  City *destCity = getCity(l, destinationCity);
  if (!destCity)
    return;
  Node *node = l->head;
  while (node)
  {
    City *currentCity = node->val;
    currentCity->distToGoal = abs(currentCity->latitude - destCity->latitude) + abs(currentCity->longitude - destCity->longitude);
    node = node->next;
  }
}

void displayAllCitiesWithDetails(List *l)
{
  Node *node = l->head;
  while (node)
  {
    City *city = node->val;
    printf("%s %d %d %d %d\n", city->name, city->latitude, city->longitude, city->distFromStart, city->distToGoal);
    node = node->next;
  }
}
