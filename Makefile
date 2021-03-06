OBJS = C3dsParser.o graphics.o sound.o main.o fht.o intro.o scene.o outro.o
CC = g++
CFLAGS =-c -g -std=gnu++0x
LFLAGS =
LIBS =-lGLU -lglut -lasound -lGL -lvorbisfile -lpthread

run: demo
	./demo

demo: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) -o $@ 

C3dsParser.o : C3dsParser.cpp C3dsParser.h
	$(CC) $(CFLAGS) -o $@ C3dsParser.cpp

graphics.o : graphics.cpp graphics.h C3dsParser.h chunks.def sound.h vector.h
	$(CC) $(CFLAGS) -o $@ graphics.cpp

sound.o : sound.cpp sound.h fht.h
	$(CC) $(CFLAGS) -o $@ sound.cpp

main.o : main.cpp graphics.h
	$(CC) $(CFLAGS) -o $@ main.cpp

fht.o : fht.cpp fht.h
	$(CC) $(CFLAGS) -o $@ fht.cpp

scene.o : scene.cpp
	$(CC) $(CFLAGS) -o $@ scene.cpp

intro.o : intro.cpp
	$(CC) $(CFLAGS) -o $@ intro.cpp

outro.o : outro.cpp
	$(CC) $(CFLAGS) -o $@ outro.cpp

.PHONY :  clean

clean:
	rm demo $(OBJS)
