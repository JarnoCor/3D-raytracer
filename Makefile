# compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -Iinclude
CFLAGS += -DUNICODE -D_UNICODE
LDFLAGS := -mwindows -municode -ld2d1 -luuid -lole32 -lgdi32 -luser32

# directories
SRC_DIR := src
OBJ_DIR := build

# files
SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/basewin.c \
	$(SRC_DIR)/mainwin.c \
	$(SRC_DIR)/util.c \

OBJS := \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/basewin.o \
	$(OBJ_DIR)/mainwin.o \
	$(OBJ_DIR)/util.o \

TARGET := app.exe

# default target
all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
