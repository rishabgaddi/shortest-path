/*************************************************************
 *
 * $Id: tList.c 2102 2018-11-19 10:26:08Z phil $
 * 
 *************************************************************
 */
/**
 * Sample test program for generic Lists : application to strings.
 *
 *************************************************************/

static char rcsId[] = "@(#) $Id: tList.c 2102 2018-11-19 10:26:08Z phil $";

#include <stdio.h>
#include <string.h>
#include "List.h"


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

/*************************************************************
 * another display function, just to test "forEach"
 * @param s the string to display
 *************************************************************/
static void prString2 (void * s) {
  printf("%s\n",(char*)s);
}

/*************************************************************
 * test predicate for firstThat / allThat
 * @param s the element to test
 *************************************************************/
int startsWithM(void* s){
  return ((char*)s)[0] == 'm';
}

/*************************************************************
 * test program: creation of a list of strings then tests
 * for main list functionalities.
 *************************************************************/

int main() {
  int i; status st;
  char * tab[] = {"belle marquise", "vos beaux yeux", "me font",
		  "mourir", "d'amour" };
  char* s;

  /* list creation */
  List* l = newList(compString,prString);
  if (!l) return 1;

  /* populating list */
  for (i=0; i < sizeof(tab)/sizeof(char*); i++)
    addList(l,tab[i]);

  /* display list - should be sorted */
  displayList(l);
  putchar('\n');

  /*  test contain predicate */
  if (isInList(l,"mourir"))
    puts("mourir is in list");
  else
    puts("mourir is not in list");
  if (isInList(l,"vivre"))
    puts("vivre is in list");
  else
    puts("vivre is not in list");

  /* access Nth element 3 = "me font" */
  st = nthInList(l, 3, &s);
  if (st) {fprintf(stderr,"%s\n", message(st)); return 1;}
  printf("me font? : %s\n",s);

  /* remove and add element */
  st = remFromListAt(l,4,&s);
  if (st) {fprintf(stderr,"%s\n", message(st)); return 2;}
  printf("mourir? : %s\n",s);
  st = addListAt(l,4,s);
  if (st) {fprintf(stderr,"%s\n", message(st)); return 3;}
  st = remFromList(l,s);
  if (st) {fprintf(stderr,"%s\n", message(st)); return 4;}
  st = addList(l,s);
  if (st) {fprintf(stderr,"%s\n", message(st)); return 5;}


  /* test length (beware: sizeof is long int!) */
  printf("length : %d (expected %ld)\n", lengthList(l),
	 sizeof(tab)/sizeof(char*));

  /* test forEach */
  puts("forEach:");
  forEach(l,prString2);

  /* test firstThat and allThat (memory not freed) */
  printf("first with m : %s\n",(char*)firstThat(l,startsWithM));
  printf("all with m : ");
  displayList(allThat(l,startsWithM));
  putchar('\n');
  
  /* final cleanup */
  delList(l);

  return 0;
}
/*************************************************************/
