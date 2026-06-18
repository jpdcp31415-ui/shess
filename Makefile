CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Werror -Wextra

# 1. Grab all .c files in the current directory
SRCS = $(wildcard *.c)

# 2. Convert that list of .c files into a list of .o files (e.g., main.c -> main.o)
OBJS = $(patsubst %.c, %.o, $(SRCS))

# The final executable name
TARGET = main 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# How to compile any .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
