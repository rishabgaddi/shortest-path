#include <stdio.h>
#include <string.h>
#include "map.h"

/*************************************************************
 * Function to compare two elements (strings) : based on strcmp
 * @param s1 the first string to compare
 * @param s2 the second string to compare
 * @return <0 if s1 is less than s2
 * @return 0 if s1 equals s2
 * @return >0 otherwise
 *************************************************************/
static int compString (void * s1, void * s2) {
  return strcmp((char*)s1, (char*)s2);
}

/*************************************************************
 * Function to display an element of the list
 * @param s the string to display
 *************************************************************
 */
static void prString(void * s) {
  printf("%s",(char*)s);
}

int main () {
  /* list creation */
  List* l = newList(compString,prString);
  if (!l) return 1;

  City *city = addCity(l, "Calais", -200, 1200);
  if (!city) return 1;
  status res = addNeighbour(l, city, "Nancy", 534);
  if (res != OK) return 1;
  displayAllCities(l);
}
