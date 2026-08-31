CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude -O2
SRC = src/fsm.c src/ats_fsm.c tests/test_fsm.c
TARGET = test_fsm

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o *.exe
