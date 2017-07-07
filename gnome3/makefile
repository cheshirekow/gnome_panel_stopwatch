all: stopwatch-applet

stopwatch-applet : 
	gcc -o stopwatch-applet stopwatch-applet.c `pkg-config --cflags --libs gtk+-3.0 libpanelapplet-4.0`
	
clean:
	rm stopwatch-applet