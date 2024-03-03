CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic

s21_grep: s21_grep.o 
	$(CC) $^ -o $@
	rm -rf *.o *.a *.so
tests:
	bash ./tests.sh
test_leaks:
	bash ./test_leaks.sh
s21_grep.o: s21_grep.c s21_grep.h
	$(CC) ${FLAGS} -c $< -o $@
clean:
	rm -rf *.o *.a *.so s21_grep

rebuild: 
	make clean all
