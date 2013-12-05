
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"

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
#include <time.h>
#include "SDL.h"
#include "SDL_mixer.h"

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "opencv/cv.h"
#include "opencv/highgui.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include "HanukkaDisplay.h"
#include "SetupDisplay.h"
#include "MegaTreeDisplay.h"
#include "XmesGameDisplay.h"

/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B230400

//D6 D7 D2 D3 C6 C7 B7 D1 B4 D0 B2 B3

int fd=-1;
fd_set fds;
cv::RNG rng;

int main(int argc, char *argv[])
{
  char* ttydev = NULL;
  char* filename = NULL;
  int mode  = 0;
  
  if (argc > 2)
  {
    mode = atoi(argv[1]);
    ttydev = argv[2]; 
    if (argc > 3)
      filename = argv[3]; 
  } else {
    printf("Usage: %s mode [0=test, 1=hanukka display, 2=xmes game, 3=xmes normal] ttyDev houseImage.jpg\n", argv[0]);
    exit(1);
  }

  MegaTree megaTree(cvPoint(555,355), cvPoint(0,295), 12);
  megaTree.setupSerial(ttydev);

  IplImage *image = NULL;
  if (filename)
    image = cvLoadImage( filename, 1);

  if (image)
  {
    cvNamedWindow("Test", 1);
    cvMoveWindow("Test", 0, 0);
  }

  Display* display = NULL;
  switch (mode)
  {
     case 0:
      if (argc > 6)
        display = new SetupDisplay(megaTree, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
      else
      {
        printf("Usage: %s mode [0=test] ttyDev houseImage.jpg stringId red green blue\n", argv[0]);
        exit(1);
      }
      break;
     case 1:
      display = new HanukkaDisplay(megaTree, 8);
      break;
     case 2:
      display = new XmesGameDisplay(megaTree);
      break;
  }

  // start and end times
  time_t start, end;
  int frameCounter = 0;

  bool fireHeart = 0;
  time(&start);

  while(1)
  {
    IplImage *tmpImg  = NULL;
    if (image)
    {
      tmpImg = cvCreateImage( cvSize(image->width, image->height), image->depth, image->nChannels);
      cvCopy(image, tmpImg, NULL);
    }
    //Draw the objects
    megaTree.draw(tmpImg);

    int key = -1;
    if (image)
    {
      cvShowImage("Test", tmpImg);
      key = cvWaitKey(10);
      cvReleaseImage(&tmpImg);
    }

    if (display)
      display->process(key);

    time(&end);
    frameCounter++;
    double sec = difftime(end, start);
    double fps = frameCounter/sec;
    //printf("FPS=%0.2f\n", fps);
  }

  close(fd);

  if (image)
  {
    cvReleaseImage(&image);
    cvDestroyWindow("Test");
  }

  return 0;
}


