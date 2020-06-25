all:
	gcc -std=c99 -Wall -Wextra -Werror proj2.c -lm -o proj2
clean:
	rm proj2
test:
	./proj2 24 330 1e-20