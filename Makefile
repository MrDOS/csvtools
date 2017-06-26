CFLAGS := -Os -std=gnu99
debug: CFLAGS := -Wall -Wextra -Werror -pedantic -std=gnu99 -g

all: csvsplit csvjoin
debug: csvsplit csvjoin
test: test-mac-csv \
      test-quotes-appropriately \
      test-quotes-unprintable \
      test-adds-newline \
      test-retains-newlines \
      test-ignores-blank-lines \
      test-accepts-staggered-rows

%: obj/%.o obj/writebuf.o
	$(CC) $^ -o $@

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $(CFLAGS) $< -o $@

test-mac-csv:
	@cat tests/$@.txt
	<tests/test-mac-csv-before.csv ./csvfix \
		| diff tests/test-mac-csv-after.csv -
	@echo

test-%: csvsplit csvjoin
	@cat tests/$@.txt
	<tests/$@-before.csv ./csvsplit \
		| ./csvjoin \
		| diff tests/$@-after.csv -
	@echo

.PHONY: clean
clean:
	-rm csvsplit csvjoin obj/*.o
