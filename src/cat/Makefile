CC=gcc -Wall -Wextra -Werror 
all: s21_cat

s21_cat:
	$(CC) s21_cat.c -o s21_cat
clean:
	rm s21_cat
test: s21_cat
	bash ./diffcat.sh
style:
	clang-format -n --style=google *.c 
