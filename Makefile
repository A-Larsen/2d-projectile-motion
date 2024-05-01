projectile: projectile.c
	gcc -o $@ $^ -lSDL2 -lm
