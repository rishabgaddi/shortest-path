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

void displayNeighbours(List *l, char *name);

void calculateInitialDistanceToGoal(List *l, char *destinationCity);

void displayAllCitiesWithDetails(List *l);

City *getMin(List *l);

int getList(List *l, City *city);

/** Find and display the shortest path between two cities using A* algorithm.
 * @param l the list of cities
 * @param originCity the name of the city where the path starts
 * @param destinationCity the name of the city where the path ends
 */
void shortestPath(List *l, char *originCity, char *destinationCity);

#endif
