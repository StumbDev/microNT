# Makefile for MicroNT Project
CC := gcc
CFLAGS := -Wall -Wextra -mwindows
TARGET := MicroNT.exe
SRC := main.c
OBJ := $(SRC:.c=.o)

.PHONY: all clean

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
