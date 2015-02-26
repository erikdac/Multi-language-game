CC = g++
STD = -std=c++11
CFLAGS = -pthread
FILES = client.cpp connection.cpp login.cpp connection.hpp login.hpp json11/json11.cpp
LFLAGS = 

all:
	$(CC)  -o client $(STD) $(CFLAGS) $(FILES) $(LFLAGS) 

clean:
	rm client

