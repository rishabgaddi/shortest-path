OBJ = status.o List.o map.o main.o

aStar: $(OBJ)
	gcc -o aStar $(OBJ)

$(OBJ): status.h

List.o map.o main.o: List.h
map.o main.o: map.h
