/**********************************************************
 * main.c
 * Author(s): Rishab Gaddi and Maheshbhai Chauhan
 * ********************************************************
 * Sample test program for the map.h and map.c files.
 * ********************************************************/
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

int main(int nWords, char *words[])
{
  char source[255];
  char dest[255];
  /* Check for correct number of arguments */
  /* If city names are not provided, ask for them */
  if (nWords < 2)
  {
    printf("Enter the name of the source city: ");
    scanf("%s", source);
    printf("Enter the name of the destination city: ");
    scanf("%s", dest);
  }
  else if (nWords < 3)
  {
    strcpy(source, words[1]);
    printf("Enter the name of the destination city: ");
    scanf("%s", dest);
  }
  else
  {
    strcpy(source, words[1]);
    strcpy(dest, words[2]);
  }
  /* Create a list to store the map */
  List *l = newList(compString, prString);
  if (!l)
  {
    fprintf(stderr, "Error: could not create list\n");
    return 4;
  }
  /* Construct the map */
  status res = constructMap(l, "FRANCE.MAP");
  if (res == ERRFILENOTFOUND)
  {
    fprintf(stderr, "Error: %s\n", message(res));
    return 2;
  }
  else if (res)
  {
    fprintf(stderr, "Error: %s\n", message(res));
    return 5;
  }
  /* Find the shortest path */
  res = findShortestPath(l, source, dest);
  if (res == ERRCITYNOTFOUND)
  {
    fprintf(stderr, "Error: %s\n", message(res));
    return 1;
  }
  else if (res == ERRMAPNOTCONNECTED)
  {
    fprintf(stderr, "Error: %s\n", message(res));
    return 3;
  }
  else if (res)
  {
    fprintf(stderr, "Error: %s\n", message(res));
    return 5;
  }
  delList(l);
  return 0;
}
