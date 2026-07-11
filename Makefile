CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Werror -Wextra

# 1. Grab all .c files in the current directory
SRCS = $(wildcard ./src/*.c)

# 2. Convert that list of .c files into a list of .o files (e.g., main.c -> main.o)
OBJS = $(patsubst ./src/%.c, ./obj/%.o, $(SRCS))

TARGET = main 

# Building
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compiling
./obj/%.o: ./src/%.c
	$(CC) -c -Iinc -o $@ $< $(CFLAGS) 

clean:
	rm -f *.o $(TARGET)
