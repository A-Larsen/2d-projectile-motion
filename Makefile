projectile: projectile.c
	gcc -g -o $@ $^ -lSDL2 -lm
