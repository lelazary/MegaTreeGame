#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <map>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B115200
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB1"


int main(int argc, char *argv[])
{

  //Setup the serial
  int fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
  if (fd <0) {
    printf("Error opening device\n");
    perror(MODEMDEVICE);
    return -1;
  }

  struct termios oldtio,newtio;
  tcgetattr(fd,&oldtio); /* save current serial port settings */
  bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */

  newtio.c_cflag = BAUDRATE; // | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = 1; //IGNPAR; 
  newtio.c_oflag = 0; //Raw Output
  newtio.c_lflag = 0;
  
  /*
    now clean the modem line and activate the settings for the port
  */
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

  
  char buffer[1+1+(50*3)];
  buffer[0] = 3; //Starts at 2
  buffer[1] = 50;
  for(int i=0; i<1; i++)
	{
     buffer[2+(i*3)] = 0;
     buffer[2+(i*3)+1] = 0;
     buffer[2+(i*3)+2] = 255;
	}
  for(int i=1; i<4; i++)
	{
     buffer[2+(i*3)] = 255;
     buffer[2+(i*3)+1] = 0;
     buffer[2+(i*3)+2] = 255;
	}
  for(int i=4; i<50; i++)
	{
     buffer[2+(i*3)] = 0;
     buffer[2+(i*3)+1] = 255;
     buffer[2+(i*3)+2] = 0;
	}

	getchar();
	write(fd, buffer, 1+1+(50*3));
  usleep(100000);
	int ret = read(fd, buffer, 255);
	buffer[ret] = 0;
	printf("Got %i --%s--\n", ret, buffer);

  /* restore the old port settings */
  tcsetattr(fd,TCSANOW,&oldtio);

  close(fd);

  exit(0);
}
