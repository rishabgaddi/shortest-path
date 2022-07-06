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
  // Neighbor *neighbor = malloc(sizeof(neighbor));
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
    printf("City: %s, Distance: %d\n", neighbor->city->name, neighbor->distance);
    node = node->next;
  }
}

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

void setInitialStartDistanceToMax(List *l) {
  Node *node = l->head;
  while (node) {
    City *city = node->val;
    city->distFromStart = 3000;
    node = node->next;
  }
}

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

City *getMin(List *l) {
  Node *node = l->head;
  City *min = node->val;
  while (node) {
    City *city = node->val;
    if (city->distFromStart + city->distToGoal < min->distFromStart + min->distToGoal)
      min = city;
    node = node->next;
  }
  return min;
}

void findShortestPath(List *l, char *originCity, char *destinationCity) {
  City *origin = getCity(l, originCity);
  City *destination = getCity(l, destinationCity);
  if (!origin || !destination)
    return;
  List *open = newList(l->comp, l->pr);
  if (!open)
    return;
  List *closed = newList(l->comp, l->pr);
  if (!closed)
  {
    delList(open);
    return;
  }
  status res = addList(open, origin);
  if (res != OK)
  {
    delList(open);
    delList(closed);
    return;
  }
  calculateHeuristicDistance(l, destinationCity);
  setInitialStartDistanceToMax(l);
  origin->distFromStart = 0;
  while (open->nelts > 0) {
    printf("Open:\n");
    displayAllCities(open);
    City *current = getMin(open);
    res = addList(closed, current);
    if (res != OK)
    {
      delList(open);
      delList(closed);
      return;
    }
    res = remFromList(open, current);
    if (res != OK)
    {
      delList(open);
      delList(closed);
      return;
    }
    if (current == destination)
      break;
    Node *node1 = current->neighbors->head;
    // Add all neighbours to open list
    while (node1) {
      printf("Adding neighbour to open list\n");
      Neighbor *neighbor = node1->val;
      City *neighborCity = neighbor->city;
      if (isInList(open, neighborCity)) {
        printf("Neighbour already in open list\n");
        node1 = node1->next;
        continue;
      }
      res = addList(open, neighborCity);
      if (res != OK)
      {
        delList(open);
        delList(closed);
        return;
      }
      node1 = node1->next;
    }
    Node *node = current->neighbors->head;
    while (node)
    {
      printf("Checking neighbour\n");
      Neighbor *neighbor = node->val;
      City *neighborCity = neighbor->city;
      int newDistance = current->distFromStart + neighbor->distance;
      if (newDistance > neighborCity->distFromStart)
      {
        printf("skipping neighbour\n");
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
          return;
        }
      }
      neighborCity->distFromStart = newDistance;
      // Add current city as previous city to neighbor city
      neighborCity->prev = current;
      node = node->next;
      printf("Neighbour checked\n");
    }
  }
  City *current = destination;
  while (current)
  {
    printf("%s\n", current->name);
    current = current->prev;
  }
  delList(open);
  delList(closed);
}