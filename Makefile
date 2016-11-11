CFLAGS := -Os -std=gnu99
debug: CFLAGS := -Wall -Wextra -Werror -pedantic -std=gnu99 -g

all: csvsplit csvjoin
debug: csvsplit csvjoin

%: obj/%.o obj/writebuf.o
	$(CC) $^ -o $@

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	-rm csvsplit csvjoin obj/*.o
