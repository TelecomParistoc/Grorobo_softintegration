OBJ_DIR = obj
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

CC = gcc
CFLAGS = -Wall -Werror -Wextra -O1 -g


all: createDir $(OBJ)

$(OBJ_DIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

obj: createDir $(OBJ)

createDir:
	@mkdir -p $(OBJ_DIR)

clean :
	@rm -rf $(OBJ_DIR)
