SRC = main.cpp

CC = g++
CFLAGS = -Wall -O1 -std=c++14 -g
LDFLAGS =

EXEC = robotMain


$(EXEC):
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LDFLAGS)

run: $(EXEC)
	./$(EXEC)

clean :
	@rm -rf $(EXEC)
