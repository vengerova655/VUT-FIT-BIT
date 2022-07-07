EXECUTABLE = proj2
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -pedantic main.c -std=gnu99 

all: $(EXECUTABLE)

$(EXECUTABLE): main.o
	$(CC) $(CFGLAGS) -o $@ $^ -lpthread
	
main.o: main.c
	$(CC) $(CFGLAGS) -c $^ -lpthread

clean:
	rm -f $(EXECUTABLE) *.o	
