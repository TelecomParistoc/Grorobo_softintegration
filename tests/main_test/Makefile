SRC_DIRS = ../../src/sensors ../../src/board_singleton ../../src/collision_behaviour
OBJ_DIRS = $(addsuffix /obj, $(SRC_DIRS))
OBJS = $(addsuffix /obj/*.o, $(SRC_DIRS))
INCLUDES = $(addprefix -I, $(addsuffix /include, $(SRC_DIRS)))
SRC = test.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 $(INCLUDES) -g
LDFLAGS = -lwiringPi -lpthread -lrobotdriver

EXEC = test


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
