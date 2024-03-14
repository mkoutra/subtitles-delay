# Makefile for subtitle delay program

CC = gcc
CFLAGS = -Wall
SRC = sub_delay.c main.c
OBJ_DIR = build
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))
TARGET = subtitle_delay

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Run the executable
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)