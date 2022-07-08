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
  if (nWords < 2) {
    printf("Enter the name of the source city: ");
    scanf("%s", source);
    printf("Enter the name of the destination city: ");
    scanf("%s", dest);
  } else if (nWords < 3) {
    strcpy(source, words[1]);
    printf("Enter the name of the destination city: ");
    scanf("%s", dest);
  } else {
    strcpy(source, words[1]);
    strcpy(dest, words[2]);
  }
  List *l = newList(compString, prString);
  if (!l)
    return 1;

  FILE *f = fopen("FRANCE.MAP", "r");
  if (!f) {
    fprintf(stderr, "Could not open file FRANCE.MAP\n");
    return 2;
  }
  char name[255];
  int val1, val2;
  City *city;
  status res;
  while (fscanf(f, "%s %d %d", name, &val1, &val2) != EOF)
  {
    if (val2 != 9999)
    {
      city = addCity(l, name, val1, val2);
    }
    else
    {
      res = addNeighbour(l, city, name, val1);
      if (res != OK)
        return 1;
    }
    val1 = 0;
    val2 = 9999;
  }
  fclose(f);
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
    return 4;
  }
  return 0;
}
