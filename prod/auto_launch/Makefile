SRC_DIRS = .
OBJ_DIRS = $(addsuffix /obj, $(SRC_DIRS))
OBJS = $(addsuffix /obj/*.o, $(SRC_DIRS))
SRC = main.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -g
LDFLAGS =

EXEC = auto_launch


.PHONY: $(SRC_DIRS)

$(EXEC): $(SRC_DIRS)
	$(CC) -o $@ $(OBJS) $(SRC) $(CFLAGS) $(LDFLAGS)

$(SRC_DIRS):
	make -C $@ obj

run: $(EXEC)
	./$(EXEC)

clean :
	@rm -rf $(EXEC)

clean-all :
	@rm -rf $(OBJ_DIRS)
	@rm -rf $(EXEC)
