CC = gcc
FLAGS = -Wall -Werror -Wextra -pedantic

s21_cat: s21_cat.o libfile_reader.a
		$(CC) $^ -o $@
		rm -rf *.o *.a *.so
tests:
		bash ./tests.sh

libfile_reader.a: file_reader.o
		ar rcs $@ $^

s21_cat.o: s21_cat.c
		$(CC) ${FLAGS} -c $^ -o $@

file_reader.o: file_reader.c 
		$(CC) ${FLAGS} -c $^ -o $@

clean:
		rm -rf *.o *.a *.so s21_cat

rebuild: 
		make clean all
