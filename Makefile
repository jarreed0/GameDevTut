CC       := g++
LFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

a.out: game.o main.o object.o entity.o audio.o
	$(CC) *.o $(LFLAGS)
	rm *.o

game.o: game.cpp
	$(CC) -c game.cpp

main.o: main.cpp
	$(CC) -c main.cpp

object.o: object.cpp
	$(CC) -c object.cpp

entity.o: entity.cpp
	$(CC) -c entity.cpp

audio.o: audio.cpp
	$(CC) -c audio.cpp

clean:
	rm a.out $(OBJECTS)

