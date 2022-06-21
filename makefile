OBJ = status.o List.o tList.o

aStar: $(OBJ)
	gcc -o aStar $(OBJ)

$(OBJ): status.h

List.o tList.o: List.h
