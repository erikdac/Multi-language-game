CC = g++
CFLAGS = -std=c++11 -Wall -pedantic -Iincludes -pthread
SOURCES = $(wildcard *.cpp)
OBJ_FILES = $(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))

obj/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

MapCreator: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)
