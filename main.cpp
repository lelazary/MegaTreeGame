#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B38400
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB0"


int main()
{
  int fd,c, res;
  struct termios oldtio,newtio;
  char buf[255];
  /*
     Open modem device for reading and writing and not as controlling tty
     because we don't want to get killed if linenoise sends CTRL−C.
     */
  fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
  if (fd <0) {
    printf("Error opening device\n");
    perror(MODEMDEVICE);
    return -1;
  }
  printf("Device open\n");

  tcgetattr(fd,&oldtio); /* save current serial port settings */

  bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
  /*
    BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
    CRTSCTS : output hardware flow control (only used if the cable has
    all necessary lines. See sect. 7 of Serial−HOWTO)
    CS8
    : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL
    CREAD
    : local connection, no modem contol
    : enable receiving characters
  */
  newtio.c_cflag = 2239; //BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = 1; //IGNPAR; 
  newtio.c_oflag = 0; //Raw Output
  newtio.c_lflag = 0;

  /*
    now clean the modem line and activate the settings for the port
  */
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);

  /*
     terminal settings done, now handle input
     In this example, inputting a 'z' at the beginning of a line will
     exit the program.
  */

  write(fd, "A", 1);
  sleep(1);
  write(fd, "a", 1);

  /* restore the old port settings */
  tcsetattr(fd,TCSANOW,&oldtio);
}

