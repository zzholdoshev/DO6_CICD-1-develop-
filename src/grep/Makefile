CC=gcc -Wall -Wextra -Werror
all: s21_grep
s21_grep: s21_grep.c
	$(CC) s21_grep.c -o s21_grep
clean: s21_grep
	rm s21_grep
test: s21_grep
	bash ./diff_grep.sh
style: 
	clang-format -n --style=google *.c *.h
