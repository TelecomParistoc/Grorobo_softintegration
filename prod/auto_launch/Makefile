SRC = main.cpp

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -g
LDFLAGS =

EXEC = auto_launch


$(EXEC):
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LDFLAGS)

run: $(EXEC)
	./$(EXEC)

clean :
	@rm -rf $(EXEC)
