# For debugging, define PM_CHECK_ERRORS
# Use this for linux alsa (0.9x) version
#ALSALIB = -lasound
CC = g++ $(CFLAGS) $(VFLAGS) $(PMFLAGS) -g

all: game

game: game.cpp
	$(CC) game.cpp -o game -lopencv_core -lopencv_highgui

clean:
	rm -f game game.o
