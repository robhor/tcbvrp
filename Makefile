CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))
EXECUTABLE=tcbvrp

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
