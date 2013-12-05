
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
#include "MegaTreeDisplay.h"

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

  Music music;

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

  Display* display;
  switch (mode)
  {
     case 1:
      display = new HanukkaDisplay(megaTree, 8);
      break;
     case 2:
      music.playBackground("music.mp3");
      break;
  }

  Object santaSlay("sprites/santaSlay.png", megaTree);

  Object cannon1("sprites/cannon1.png", megaTree);
  cannon1.setPos(0,38);

  Object heart("sprites/heart.png", megaTree);

  Object cannon2("sprites/cannon2.png", megaTree);
  cannon2.setPos(0,38);

  std::vector<Object*> presents;
  presents.push_back(new Object("sprites/present1.png", megaTree));

  cv::Mat background = cv::imread("sprites/background.png", -1); //Load RGBA png image

  std::vector<Object*> objects;
  objects.push_back(new Object("sprites/house1.png", megaTree));
  objects.push_back(new Object("sprites/house2.png", megaTree));
  objects.push_back(new Object("sprites/snowman.png", megaTree));
  objects.push_back(new Object("sprites/treeSmall.png", megaTree));
  objects.push_back(new Object("sprites/treeSmall2.png", megaTree));
  objects.push_back(new Object("sprites/grinchHouse.png", megaTree));

  Object santa("sprites/santa.png", megaTree);
  //objects.push_back(new Object("sprites/santa.png", megaTree));
  //objects.push_back(new Object("sprites/santaFace.png", megaTree));
  //objects.push_back(new Object("sprites/tree.png", megaTree));
  //objects.push_back(new Object("sprites/snowman.png", megaTree));
  //objects.push_back(new Object("sprites/grinch.png", megaTree));


  int currentObject = 0;
  int y = rng.uniform(20,35);
  objects[currentObject]->setPos(12, y);

  Object* present = NULL;
  double presentSpeed = 2.0/2;
  double objectSpeed = 0.5/2;
  double santaSpeed = 3.0/2;
  int idx = 0;
  int drawBit = 0;

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

    display->process(key);

    //megaTree.setImage(background);
    ////megaTree.setColor(CV_RGB(0,0,0));

    ////megaTree.setColor(atoi(argv[3]),CV_RGB(atoi(argv[4]), atoi(argv[5]), atoi(argv[6])));

    //if (key != -1)
    //{
    //  switch (key)
    //  {
    //    case 49:
    //      if (present == NULL)
    //      {
    //        if (santaSlay.getPosX() < -5 && santaSlay.getPosX() > -20)
    //        { 
    //          music.playFX(Music::DROP);
    //          present = presents[0];
    //          present->setPos(3, 5);
    //        }


    //      }
    //      break;
    //    case 50:
    //      break;
    //  }
    //  //cannon2.draw();
    //  //fireHeart = 1;
    //  //heart.setPos(0,30);

    //} else {
    //  //  cannon1.draw();

    //}


    //////idx++;
    //////if (idx > 60)
    //////{
    //////  idx = 0;
    //////  currentObject = rng.uniform(0,objects.size()); 
    //////}
    //////objects[currentObject]->draw();

    //////santa.draw();

    //////if (fireHeart)
    //////{
    //////    heart.draw();
    //////    heart.move(0,-2);
    //////    if (heart.getPosY() < 10)
    //////      fireHeart = 0;
    //////}

    //santaSlay.move(santaSpeed, 0);
    //santaSlay.draw();

    //if (present != NULL)
    //{
    //  present->move(0, presentSpeed);
    //  present->draw();
    //  if (present->getPosY() > 39)
    //    present = NULL;
    //  else
    //  {
    //    if (intersect(present->getBB(), objects[currentObject]->getBB()))
    //    {
    //      present = NULL;
    //      if (currentObject == 5)
    //      {
    //        music.playFX(Music::GRINCH_HIT);
    //        music.playBackground("grinch.mp3");
    //      } else {
    //        if (currentObject == 3 ||
    //            currentObject == 4)
    //        {
    //          music.playFX(Music::BAD_HIT);
    //        } else {
    //          music.playFX(Music::GOOD_HIT);
    //        }
    //      }

    //    }

    //  }
    //}

    //objects[currentObject]->move(objectSpeed, 0);
    //objects[currentObject]->draw();
    //if (objects[currentObject]->getPosX() < -23)
    //{
    //  currentObject = rng.uniform(0,objects.size()); //Check if inclusive
    //  int y = rng.uniform(20,35);
    //  objects[currentObject]->setPos(12, y);
    //}

    //megaTree.drawSnow(CV_RGB(255,255,255));


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


