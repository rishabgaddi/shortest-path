/*********************************************************************
 * map.h
 * Author(s): Rishab Gaddi and Maheshbhai Chauhan
 * *******************************************************************
 * This file contains the declarations of the functions used in map.c
 * This makes use of the List structure defined in List.h
 * *******************************************************************/
#ifndef __MAP_H__
#define __MAP_H__

#include "List.h"

/** City is a struct that contains the name of the city, its latitude,
 *  longitude, list of its neighbors, distance from start, distance to goal,
 * and previous city.
 * The last three are calculated by the A* algorithm.
 */
typedef struct City
{
  char *name;
  List *neighbors;
  int latitude;
  int longitude;
  int distFromStart;
  int distToGoal;
  struct City *prev;
} City;

/** Neighbor is a struct that contains the pointer to the City and the distance. */
typedef struct Neighbor
{
  int distance;
  City *city;
} Neighbor;

/** constructMap constructs a map of cities and their neighbors.
 * @param l the list of cities
 * @param filename the file containing the map
 * @return OK if successful, ERROR otherwise
 */
status constructMap(List *l, char *filename);

/** addCity adds a city to the list of cities.
 * @param l the list of cities
 * @param name the name of the city
 * @param latitude the latitude of the city
 * @param longitude the longitude of the city
 * @return the city that was added
 */
City *addCity(List *l, char *name, int latitude, int longitude);

/** getCity returns the city with the given name.
 * @param l the list of cities
 * @param name the name of the city
 * @return the city with the given name
 */
City *getCity(List *l, char *name);

/** addNeighbor adds a neighbor to the list of neighbors of a city.
 * @param l the list of cities
 * @param city the city to add the neighbor to
 * @param name the name of the neighbor
 * @param distance the distance between the city and the neighbor
 * @return OK if successful, ERR otherwise
 */
status addNeighbour(List *l, City *city, char *name, int distance);

/** displayAllCities displays all the cities in the list.
 * @param l the list of cities
 */
void displayAllCities(List *l);

/** displayNeighbors displays the list of neighbors of a city.
 * @param l the list of cities
 * @param city the city to display the neighbors of
 */
void displayNeighbours(List *l, char *name);

/** calculateHeuristicDistance calculates the heuristic distance to the goal for all cities.
 * @param l the list of cities
 * @param destinationCity the city to calculate the heuristic distance to
 */
void calculateHeuristicDistance(List *l, char *destinationCity);

/** displayAllCitiesWithDetails displays the list of cities with their details.
 * @param l the list of cities
 */
void displayAllCitiesWithDetails(List *l);

/** setInitialStartDistanceToMax sets the distance from start to all cities to 9999.
 * @param l the list of cities
 */
void setInitialStartDistanceToMax(List *l);

/** getMin reuturns the city with the minimum distance from start to goal.
 * @param l the list of cities
 * @return the city with the minimum distance
 */
City *getMin(List *l);

/** printPath prints the path using the back pointers.
 * @param l the list of cities
 * @param city the goal city
 */
void printPath(List *l, City *city);

/** findShortestPath finds the shortest path from start to goal.
 * @param l the list of cities
 * @param originCity the city to start from
 * @param destinationCity the city to end at
 * @return OK if successful, ERR otherwise
 */
status findShortestPath(List *l, char *originCity, char *destinationCity);

#endif
