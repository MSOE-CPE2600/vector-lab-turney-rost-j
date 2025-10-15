# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
TARGET  := vectorcalc

# Source and object files
SRCS    := main.c vector.c util.c io.c
OBJS    := $(SRCS:.c=.o)
DEPS    := vector.h util.h io.h

all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file into a .o file
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)
