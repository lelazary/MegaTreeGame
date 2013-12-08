
#ifndef MEGATREEDISPLAY_H
#define MEGATREEDISPLAY_H

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
#include "MegaTreeDisplay.h"
#include "HanukkaDisplay.h"


/* baudrate settings are defined in <asm/termbits.h>, which is
   included by <termios.h> */
#define BAUDRATE B115200

//Map the io ports to the actual string on the display
//const int stringMaps[12] = { 8, 5, 4, 3, 7, 6, 2,   14 , 13,  15  , 11, 12};
//const int stringMaps[12] = { 8, 5, 3, 7, 6, 4, 15, 14, 11, 2, 12, 13};
const int stringMaps[12] = { 8, 5, 6, 4, 3, 7, 15, 14, 12, 2, 13, 11};


class Display
{
public:
  Display() {}
  virtual int process(int key) = 0;
};

class Music
{

public:

  enum FXSOUND {DROP, GOOD_HIT, BAD_HIT, GRINCH_HIT, LASER};
  Music() {
    //For the music
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
    int audio_channels = 2;
    int audio_buffers = 4096;
    SDL_Init(SDL_INIT_AUDIO);
    music = NULL;

    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
      printf("Unable to open audio!\n");
      exit(1);
    }

    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);

    dropWav = Mix_LoadWAV("music/drop.wav");
    goodHitWav = Mix_LoadWAV("music/goodHit.wav");
    badHitWav = Mix_LoadWAV("music/badHit.wav");
    grinchHouseHit = Mix_LoadWAV("music/grinchHouseHit.wav");
    laserWav = Mix_LoadWAV("music/laser.wav");

    //Mix_HookMusicFinished(musicDone);
  }

  ~Music()
  {
    Mix_HaltMusic(); //Stop the music
    if (music)
      Mix_FreeMusic(music);

    Mix_CloseAudio();
    SDL_Quit();

  }

  void playBackground(const char* file)
  {
    Mix_HaltMusic(); //Stop the music
    if (music)
      Mix_FreeMusic(music);
    /* Actually loads up the music */
    music = Mix_LoadMUS(file);
    Mix_PlayMusic(music, 0);
  }

  void playFX(FXSOUND fx)
  {
    Mix_HaltChannel(0); //Stop the drop sown
    switch(fx)
    {
      case DROP: Mix_PlayChannel(0, dropWav, 0); break;
      case GOOD_HIT: Mix_PlayChannel(0, goodHitWav, 0); break;
      case BAD_HIT: Mix_PlayChannel(0, badHitWav, 0); break;
      case GRINCH_HIT: Mix_PlayChannel(0, grinchHouseHit, 0); break;
      case LASER: Mix_PlayChannel(0, laserWav, 0); break;
    }
  }

  void stopFX()
  {
    Mix_HaltChannel(0); //Stop the drop sown
  }

private:
  Mix_Music *music;
  Mix_Chunk *dropWav;
  Mix_Chunk *goodHitWav;
  Mix_Chunk *badHitWav;
  Mix_Chunk *grinchHouseHit;
  Mix_Chunk *laserWav;

};

struct PixelString
{
  std::vector<CvPoint> points;
  std::vector<CvScalar> colors;

  PixelString(std::vector<CvPoint> p) :
    points(p)
  {
    for(int i=0; i<points.size(); i++)
      colors.push_back(CV_RGB(255,0,0));
  }

  PixelString(CvPoint p1, CvPoint p2)
  {
    int dx = p2.x-p1.x;
    int dy = p2.y-p1.y;
    double len;

    if(abs(dx) > abs(dy))
      len = dx;
    else
      len = dy;

    double xIncr = dx/len;
    double yIncr = dy/len;

    double x=p1.x;
    double y=p1.y;
    int step = round((len+1)/150);

    points.push_back(cvPoint(x, y));
    colors.push_back(CV_RGB(0,0,0));
    for(int i=1;i<len+4;i++)
    {
      x=x+xIncr;
      y=y+yIncr;
      if (!(i%step))
      {
        points.push_back(cvPoint(x, y));
        colors.push_back(CV_RGB(0,0,0));
      }
    }
  }

  void setColor(int i, CvScalar c) {
    if (i >= 0 && (i*3)+2 < colors.size())
    {
      //Out string has 3 pixels per index
      colors[(i*3)+0] = c;
      colors[(i*3)+1] = c;
      colors[(i*3)+2] = c;
    }
  }

  void setColor(CvScalar c) {
    for(int i=0; i<colors.size(); i++)
      colors[i] = c;
  }

  void getBuffer(char* buffer) {
    for(int i=0; i<50; i++)
    {
      buffer[(i*3)+0] = colors[(50-i)*3+0].val[0];
      buffer[(i*3)+1] = colors[(50-i)*3+1].val[2];
      buffer[(i*3)+2] = colors[(50-i)*3+2].val[1];
    }
  }

  void draw(IplImage* img)
  {
    for(int i=0; i<points.size(); i++)
      cvSet2D(img, points[i].y,points[i].x, colors[i]);
  }
};

class MegaTree
{

  
  public:
    MegaTree(CvPoint pt, CvPoint size, int numStrings) :
      fd(-1)
    {
      for(int i=-numStrings/2; i<numStrings/2; i++)
        strings.push_back(PixelString(cvPoint((pt.x+1)+(i*2),pt.y),
              cvPoint((pt.x+5)+(i*10),pt.y+size.y)));
    }

    void draw(IplImage* img)
    {
      for(int i=0; i<strings.size(); i++)
      {
        if (img)
          strings[i].draw(img);

        if (fd != -1)
        {
          char buffer[1+1+(50*3)];
          int ret = read(fd, buffer, 100);
					buffer[ret] = 0;
          int id = stringMaps[i];
          buffer[0] = id;
          buffer[1] = 50;
          ret = write(fd, buffer, 2);
          strings[i].getBuffer(buffer);
          ret = write(fd, buffer, 50*3);
          struct timeval timeout = {10, 0}; //10 seconds
          ret = select(fd+1, &fds, NULL, NULL, &timeout);
          ret = read(fd, buffer, 10);
          buffer[ret] = 0;
          if (buffer[0] != 'A')
            printf("Error while sending string data (%s)\n", buffer);
        } else {
          usleep(5000);
        }
      }
    }

    void setImage(cv::Mat& img, int offsetX = 0, int offsetY=0)
    {
      unsigned char *imgPtr = (unsigned char*)(img.data);
      for(int x = 0;x < img.cols ;x++){
        for(int y = 0;y < img.rows ;y++){
          cv::Vec4b ptr = img.at<cv::Vec4b>(y,x);
          if (ptr[3] > 0) //Alpha is either on or off
            setPixel(x+offsetX, y+offsetY, CV_RGB(ptr[2], ptr[1], ptr[0]));
        }
      }
    }

    void setColor(CvScalar c)
    {
      for(int i=0; i<strings.size(); i++)
        strings[i].setColor(c);
    }

    void setColor(int i, CvScalar c)
    {
      strings[i].setColor(c);
    }

    void setPixel(int x, int y, CvScalar c)
    {
      if (x >= 0 && x < strings.size()) 
        strings[x].setColor(y, c);
    }

    void drawSnow(CvScalar c)
    {
      for(int i=0; i<20; i++)
      {
        int x = itsRng.uniform(0,12);
        int y = itsRng.uniform(0,50);
        setPixel(x, y, c);
      }
    }

    void setupSerial(const char* ttydev)
    {
	    //Setup the serial
	    fd = open(ttydev, O_RDWR | O_NOCTTY );
	    if (fd <0) {
		    printf("Error opening device %s\n", ttydev);
		    perror(ttydev);
		    fd = -1;
	    }
	    FD_ZERO(&fds);
	    FD_SET(fd, &fds);

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
    }

  private:
    cv::RNG itsRng;
    std::vector<PixelString> strings;
    int fd;
    fd_set fds;

};


class Object
{
  public:

    Object(const char* filename, MegaTree& mt) :
      itsMegaTree(mt)
  {
    itsSprite = cv::imread(filename, -1); //Load RGBA png image
    posX  = 0;
    posY = 0;
  }

    void draw()
    {
      itsMegaTree.setImage(itsSprite, posX, posY);
    }

    void setPos(int x, int y) 
    {
      posX = x;
      posY = y;
    }

    bool move(double x = 1, double y = 0, int limit1 = 12, int limit2 = -24)
    {
      posX -= x;
      if (posX < limit2)
      {
        posX = limit1;
        return true;
      }

      posY += y;
      return false;
    }

    double getPosX(){ return posX; }
    double getPosY(){ return posY; }

    cv::Rect getBB(){
      return cvRect(posX, posY, itsSprite.cols-4, itsSprite.rows-4);
    }

  private:
    MegaTree&  itsMegaTree;
    cv::Mat itsSprite;
    double posX;
    double posY;
};


bool intersect(const cv::Rect& r1, const cv::Rect& r2)
{

  cv::Rect inter = r1 & r2;
  if (inter.width > 0 && inter.height > 0)
    return true;
  else
    return false;
}

#endif
