OBJ = status.o List.o map.o main.o

aStar: $(OBJ)
	gcc -o aStar $(OBJ)

status.o: status.c status.h
	gcc -c status.c

List.o: List.c List.h status.h
	gcc -c List.c

map.o: map.c map.h List.h status.h
	gcc -c map.c

main.o: main.c map.h List.h status.h
	gcc -c main.c