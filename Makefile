CFLAGS := -Os -std=gnu99
debug: CFLAGS := -Wall -Wextra -Werror -pedantic -std=gnu99 -g

all: csvsplit csvjoin
debug: csvsplit csvjoin

csvsplit: csvsplit.o writebuf.o
csvjoin: csvjoin.o writebuf.o

.PHONY: clean
clean:
	-rm *.o csvsplit csvjoin