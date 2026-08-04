CC = gcc
CFLAGS = -std=c99 -O2 -Wall -Werror -Wextra -fsanitize=address -g

SRCS = $(wildcard ./src/*.c)

OBJS = $(patsubst ./src/%.c, ./obj/%.o, $(SRCS))

TARGET = main 

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

./obj/%.o: ./src/%.c
	@$(CC) -c -Iinc -o $@ $< $(CFLAGS) 

clean:
	rm -f *.o $(TARGET)
