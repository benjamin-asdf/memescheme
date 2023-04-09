CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic

OBJS=interpreter.o main.o reader.o print.o

EXEC=memescheme

SRC=main.c \
    reader.c \
    evaluator.c \
    print.c


.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
