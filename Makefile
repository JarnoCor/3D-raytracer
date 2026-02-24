# compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -Iinclude
CFLAGS += -DUNICODE -D_UNICODE -D_WIN32_WINNT=0x0A00
LDFLAGS := -mwindows -municode -ld2d1 -luuid -lole32 -lgdi32 -luser32

# directories
SRC_DIR := src
OBJ_DIR := build

# files
SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/windows/basewin.c \
	$(SRC_DIR)/windows/mainwin.c \
	$(SRC_DIR)/windows/canvas.c \
	$(SRC_DIR)/util.c \
	$(SRC_DIR)/scene.c \
	$(SRC_DIR)/raytracer.c \
# 	$(SRC_DIR)/light.c \

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# 	$(OBJ_DIR)/main.o \
# 	$(OBJ_DIR)/basewin.o \
# 	$(OBJ_DIR)/mainwin.o \
# 	$(OBJ_DIR)/canvas.o \
# 	$(OBJ_DIR)/util.o \
# 	$(OBJ_DIR)/scene.o \
# 	$(OBJ_DIR)/raytracer.o \

TARGET := app.exe

# default target
all: $(TARGET)

# link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
