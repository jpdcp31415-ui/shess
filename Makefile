CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Werror -Wextra

# 1. Grab all .c files in the current directory
SRCS = $(wildcard ./src/*.c)

# 2. Convert that list of .c files into a list of .o files (e.g., main.c -> main.o)
OBJS = $(patsubst ./src/%.c, ./obj/%.o, $(SRCS))

TARGET = main 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# How to compile any .c file into a .o file
./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) $(SRCS) -c $(OBJS) **--outdir=**obj

clean:
	rm -f *.o $(TARGET)
