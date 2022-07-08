/**************************************************************************
 * map.c
 * Author(s): Rishab Gaddi and Maheshbhai Chauhan
 * ************************************************************************
 * This file contains the implementation of the functions declared in map.h
 * ************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

/** constructMap constructs a map of cities and their neighbors.
 * @param l the list of cities
 * @param filename the file containing the map
 * @return OK if successful, ERROR otherwise
 */
status constructMap(List *l, char *filename)
{
  FILE *map = fopen(filename, "r");
  if (!map)
  {
    return ERRFILENOTFOUND;
  }
  char name[255];
  int val1, val2;
  City *city;
  status res;
  while (fscanf(map, "%s %d %d", name, &val1, &val2) != EOF)
  {
    if (val2 != 9999)
    {
      city = addCity(l, name, val1, val2);
    }
    else
    {
      res = addNeighbour(l, city, name, val1);
      if (res != OK)
      {
        return res;
      }
    }
    val1 = 0;
    val2 = 9999;
  }
  fclose(map);
  return OK;
}

/** addCity adds a city to the list of cities.
 * @param l the list of cities
 * @param name the name of the city
 * @param latitude the latitude of the city
 * @param longitude the longitude of the city
 * @return the city that was added
 */
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

/** getCity returns the city with the given name.
 * @param l the list of cities
 * @param name the name of the city
 * @return the city with the given name
 */
City *getCity(List *l, char *name)
{
  if (l->nelts == 0)
    return 0;
  Node *node = l->head;
  while (node)
  {
    City *city = node->val;
    if ((l->comp)(city->name, name) == 0)
      return city;
    node = node->next;
  }
  return 0;
}

/** addNeighbor adds a neighbor to the list of neighbors of a city.
 * @param l the list of cities
 * @param city the city to add the neighbor to
 * @param name the name of the neighbor
 * @param distance the distance between the city and the neighbor
 * @return OK if successful, ERR otherwise
 */
status addNeighbour(List *l, City *city, char *name, int distance)
{
  Neighbor *neighbor = (Neighbor *)malloc(sizeof(Neighbor));
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

/** displayAllCities displays all the cities in the list.
 * @param l the list of cities
 */
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

/** displayNeighbors displays the list of neighbors of a city.
 * @param l the list of cities
 * @param city the city to display the neighbors of
 */
void displayNeighbours(List *l, char *name)
{
  City *city = getCity(l, name);
  if (!city)
    return;
  Node *node = city->neighbors->head;
  while (node)
  {
    Neighbor *neighbor = node->val;
    printf("City: %s, Distance: %d\n", neighbor->city->name, neighbor->distance);
    node = node->next;
  }
}

/** calculateHeuristicDistance calculates the heuristic distance to the goal for all cities.
 * @param l the list of cities
 * @param destinationCity the city to calculate the heuristic distance to
 */
void calculateHeuristicDistance(List *l, char *destinationCity)
{
  City *destCity = getCity(l, destinationCity);
  if (!destCity)
    return;
  Node *node = l->head;
  while (node)
  {
    City *currentCity = node->val;
    currentCity->distToGoal = (abs(currentCity->latitude - destCity->latitude) + abs(currentCity->longitude - destCity->longitude)) / 4;
    node = node->next;
  }
}

/** setInitialStartDistanceToMax sets the distance from start to all cities to 9999.
 * @param l the list of cities
 */
void setInitialStartDistanceToMax(List *l)
{
  Node *node = l->head;
  while (node)
  {
    City *city = node->val;
    city->distFromStart = 9999;
    node = node->next;
  }
}

/** displayAllCitiesWithDetails displays the list of cities with their details.
 * @param l the list of cities
 */
void displayAllCitiesWithDetails(List *l)
{
  Node *node = l->head;
  while (node)
  {
    City *city = node->val;
    printf("City: %s\n", city->name);
    printf("Latitude: %d\n", city->latitude);
    printf("Longitude: %d\n", city->longitude);
    printf("Distance from start: %d\n", city->distFromStart);
    printf("Distance to goal: %d\n", city->distToGoal);
    printf("Neighbours:\n");
    displayNeighbours(l, city->name);
    printf("----------------------------------------------------\n");
    node = node->next;
  }
}

/** getMin reuturns the city with the minimum distance from start to goal.
 * @param l the list of cities
 * @return the city with the minimum distance
 */
City *getMin(List *l)
{
  Node *node = l->head;
  City *min = node->val;
  while (node)
  {
    City *city = node->val;
    if (city->distFromStart + city->distToGoal < min->distFromStart + min->distToGoal)
      min = city;
    node = node->next;
  }
  return min;
}

/** printPath prints the path using the back pointers.
 * @param l the list of cities
 * @param city the goal city
 */
void printPath(List *l, City *city)
{
  City *current = city;
  List *path = newList(l->comp, l->pr);
  if (!path)
  {
    return;
  }
  status res = addList(path, current);
  if (res != OK)
  {
    delList(path);
    return;
  }
  while (current)
  {
    current = current->prev;
    if (current)
    {
      res = addListAt(path, 1, current);
      if (res != OK)
      {
        delList(path);
        return;
      }
    }
  }
  Node *node = path->head;
  while (node)
  {
    City *city = node->val;
    printf("%s : (", city->name);
    printf("%d km)\n", city->distFromStart);
    node = node->next;
  }
}

/** findShortestPath finds the shortest path from start to goal.
 * @param l the list of cities
 * @param originCity the city to start from
 * @param destinationCity the city to end at
 * @return OK if successful, ERR otherwise
 */
status findShortestPath(List *l, char *originCity, char *destinationCity)
{
  City *origin = getCity(l, originCity);
  City *destination = getCity(l, destinationCity);
  if (!origin || !destination)
    return ERRCITYNOTFOUND;
  List *open = newList(l->comp, l->pr);
  if (!open)
    return ERRUNABLE;
  List *closed = newList(l->comp, l->pr);
  if (!closed)
  {
    delList(open);
    return ERRUNABLE;
  }
  status res = addList(open, origin);
  if (res != OK)
  {
    delList(open);
    delList(closed);
    return res;
  }
  calculateHeuristicDistance(l, destinationCity);
  setInitialStartDistanceToMax(l);
  origin->distFromStart = 0;
  while (open->nelts > 0)
  {
    City *current = getMin(open);
    res = addList(closed, current);
    if (res != OK)
    {
      delList(open);
      delList(closed);
      return res;
    }
    res = remFromList(open, current);
    if (res != OK)
    {
      delList(open);
      delList(closed);
      return res;
    }
    if (current == destination)
    {
      printPath(l, current);
      delList(open);
      delList(closed);
      return OK;
    }
    Node *node1 = current->neighbors->head;
    while (node1)
    {
      Neighbor *neighbor = node1->val;
      City *neighborCity = neighbor->city;
      if (isInList(open, neighborCity))
      {
        node1 = node1->next;
        continue;
      }
      res = addList(open, neighborCity);
      if (res != OK)
      {
        delList(open);
        delList(closed);
        return res;
      }
      node1 = node1->next;
    }
    Node *node = current->neighbors->head;
    while (node)
    {
      Neighbor *neighbor = node->val;
      City *neighborCity = neighbor->city;
      int newDistance = current->distFromStart + neighbor->distance;
      if (newDistance > neighborCity->distFromStart)
      {
        if (isInList(closed, neighborCity))
        {
          res = remFromList(open, neighborCity);
          if (res != OK)
          {
            delList(open);
            delList(closed);
            return res;
          }
        }
        node = node->next;
        continue;
      }
      if (isInList(closed, neighborCity) != 0)
      {
        res = remFromList(closed, neighborCity);
        if (res != OK)
        {
          delList(open);
          delList(closed);
          return res;
        }
      }
      neighborCity->distFromStart = newDistance;
      neighborCity->prev = current;
      node = node->next;
    }
  }
  if (destination->prev == NULL)
  {
    return ERRMAPNOTCONNECTED;
  }
  printPath(l, destination);
  delList(open);
  delList(closed);
  return OK;
}
