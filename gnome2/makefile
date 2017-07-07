all: stopwatch-applet

stopwatch-applet:
	gcc -o stopwatch-applet stopwatch-applet.c `pkg-config --cflags --libs libpanelapplet-2.0`
	
clean:
	rm stopwatch-applet