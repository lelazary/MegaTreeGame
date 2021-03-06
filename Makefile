# For debugging, define PM_CHECK_ERRORS
# Use this for linux alsa (0.9x) version
#ALSALIB = -lasound
CC = g++ $(CFLAGS) $(VFLAGS) $(PMFLAGS) -g

all: game serialClient

game: game.cpp *.h
	$(CC) game.cpp -o game -lopencv_core -lopencv_highgui `sdl-config --cflags --libs` -lSDL_mixer
serialClient: serialClient.cpp
	$(CC) serialClient.cpp -o serialClient 

clean:
	rm -f game game.o
