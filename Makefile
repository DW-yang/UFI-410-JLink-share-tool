build:
	gcc -g -o jlink-share jlink-share.c $(shell pkgconf --libs --cflags libnm)

install:
	cp -f ./jlink-share /usr/bin/jlink-share
	cp -f ./jlink-share.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable jlink-share