sbuild:
	gcc -g -o jlink-share jlink-share.c $(shell pkgconf --libs --cflags libnm)