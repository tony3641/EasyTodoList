#all: list

#list: list.c
	#gcc -lncurses list.c -o list

TARGET = list
OBJS = event.o main.o
CFLAGS += -Wall -Wextra -std=c11 -lncurses

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)


.PHONY: debug
debug: list
debug: CFLAGS += -fprofile-arcs -ftest-coverage

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).out *.gc* .*.txt test.o test
