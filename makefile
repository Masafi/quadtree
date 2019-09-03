CC=g++ -g
CFLAGS=-c -Wall -pthread -std=c++17 "-I./SFML-2.5.1/include"
LDFLAGS="-L./SFML-2.5.1/lib"
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
SOURCES=main.cpp quadtree.cpp geometry.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=quadtree
BINDIR=/usr/bin

all: $(SOURCES) $(EXECUTABLE)
        
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LIBFLAGS)

clean:
	rm *.o $(EXECUTABLE)
