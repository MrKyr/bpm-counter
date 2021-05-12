all: bpm install

bpm: bpm.c
	gcc bpm.c -o bpm -lcurses

clean:
	rm bpm

install:
	install -m 755 bpm /usr/local/bin
