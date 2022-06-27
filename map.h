#ifndef __MAP_H__
#define __MAP_H__

#include "List.h"

typedef struct City
{
  char *name;
  List *neighbors;
  int latitude;
  int longitude;
  int distFromStart;
  int distToGoal;
  Node *prev;
} City;

typedef struct Neighbor
{
  int distance;
  City *city;
} Neighbor;

City *addCity(List *l, char *name, int latitude, int longitude);

City *getCity(List *l, char *name);

status addNeighbour(List *l, City *city, char *name, int distance);

void displayAllCities(List *l);

void calculateDistanceToGoal(List *l, City *start, City *goal);

#endif
