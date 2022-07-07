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
static int compString(void *s1, void *s2)
{
  return strcmp((char *)s1, (char *)s2);
}

/*************************************************************
 * Function to display an element of the list
 * @param s the string to display
 *************************************************************
 */
static void prString(void *s)
{
  printf("%s", (char *)s);
}

int main()
{
  /* list creation */
  List *l = newList(compString, prString);
  if (!l)
    return 1;

  FILE *f = fopen("FRANCE.MAP", "r");
  char name[255];
  int val1, val2;
  City *city;
  while (fscanf(f, "%s %d %d", name, &val1, &val2) != EOF)
  {
    if (val2 != 9999)
    {
      city = addCity(l, name, val1, val2);
    }
    else
    {
      status res = addNeighbour(l, city, name, val1);
      if (res != OK)
        return 1;
    }
    val1 = 0;
    val2 = 9999;
  }
  fclose(f);
  findShortestPath(l, "Rennes", "Lyon");
  return 0;
}
