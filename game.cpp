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

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "opencv/cv.h"
#include "opencv/highgui.h"
#endif
#include <stdlib.h>
#include <stdio.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B115200
/* change this definition for the correct port */
#define MODEMDEVICE "/dev/ttyUSB1"


int fd;

int stringMaps[12] = 
{ 6, 2, 5, 
13, 3, 4, 
7, 8, 9, 10, 11, 12 };

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
    for(int i=1;i<len;i++)
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
    if (i >= 0 && i*3+2 < colors.size())
    {
      //Out string has 3 pixels per index
      colors[i*3+0] = c;
      colors[i*3+1] = c;
      colors[i*3+2] = c;
    }
  }

  void setColor(CvScalar c) {
    for(int i=0; i<colors.size(); i++)
      colors[i] = c;
  }

  void getBuffer(char* buffer) {
    for(int i=0; i<50; i++)
		{
			buffer[2+((i)*3)+0] = colors[(50-i)*3+0].val[0];
			buffer[2+((i)*3)+1] = colors[(50-i)*3+1].val[2];
			buffer[2+((i)*3)+2] = colors[(50-i)*3+2].val[1];
		}
  }

  void draw(IplImage* img)
  {
    for(int i=0; i<points.size(); i++)
    {
      //cvDrawCircle(img, points[i], 1, colors[i]);
      cvSet2D(img, points[i].y,points[i].x, colors[i]);
    }
		
  }
};

class MegaTree
{

public:
  MegaTree(CvPoint pt, CvPoint size, int numStrings)
  {
    for(int i=-numStrings/2; i<numStrings/2; i++)
      strings.push_back(PixelString(cvPoint((pt.x+1)+(i*2),pt.y),
                                    cvPoint((pt.x+5)+(i*10),pt.y+size.y)));
  }

  void draw(IplImage* img)
  {
    for(int i=0; i<strings.size(); i++)
		{
			int id = stringMaps[i];
      strings[i].draw(img);
			char buffer[1+1+(50*3)];
			buffer[0] = id;
			buffer[1] = 50;
			strings[i].getBuffer(buffer);
			write(fd, buffer, 1+1+(50*3));
			usleep(25000);

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
      int x = itsRng.uniform(0,11);
      int y = itsRng.uniform(0,50);
      setPixel(x, y, c);
    }
  }
private:
  cv::RNG itsRng;
  std::vector<PixelString> strings;
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
	
	void move(double x = 1, double y = 0)
	{
		posX -= x;
		if (posX < -24)
			posX = 12;

		posY += y;
	}

	double getPosX(){ return posX; }
	double getPosY(){ return posY; }

private:
	MegaTree&  itsMegaTree;
  cv::Mat itsSprite;
	double posX;
	double posY;
};


int main(int argc, char *argv[])
{
  char* filename = argv[1]; 
  cv::RNG rng;

  //Setup the serial
  fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
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

  MegaTree megaTree(cvPoint(555,355), cvPoint(0,295), 12);
  IplImage *image;
  if( (image = cvLoadImage( filename, 1)) == 0 )
    return -1;

  cvNamedWindow("Test", 1);
  cvMoveWindow("Test", 0, 0);

	Object santaSlay("sprites/santaSlay.png", megaTree);
	Object santa("sprites/santa.png", megaTree);
	Object tree("sprites/tree.png", megaTree);

	std::vector<Object*> presents;
	presents.push_back(new Object("sprites/present1.png", megaTree));

  cv::Mat background = cv::imread("sprites/background.png", -1); //Load RGBA png image
	
	std::vector<Object*> objects;
	objects.push_back(new Object("sprites/house1.png", megaTree));
	objects.push_back(new Object("sprites/house2.png", megaTree));
	objects.push_back(new Object("sprites/snowman.png", megaTree));
	objects.push_back(new Object("sprites/treeSmall.png", megaTree));
	objects.push_back(new Object("sprites/treeSmall2.png", megaTree));


  int currentObject = 0;
	int y = rng.uniform(20,35);
	objects[currentObject]->setPos(12, y);
	
	Object* present = NULL;
	double presentSpeed = 1.5;
	double objectSpeed = 0.2;
	double santaSpeed = 0.1;
	int idx = 0;
	int drawBit = 0;
  while(1)
  {

    IplImage *tmpImg = cvCreateImage( cvSize(image->width, image->height), image->depth, image->nChannels);
    cvCopy(image, tmpImg, NULL);
    //Draw the objects
    megaTree.draw(tmpImg);

    cvShowImage("Test", tmpImg);
    int key = cvWaitKey(10);
    cvReleaseImage(&tmpImg);
  
    if (key != -1)
    {
      switch (key)
      {
        case 49:
					if (present == NULL)
					{
						if (santaSlay.getPosX() < 5 && santaSlay.getPosX() > -15)
						{ 
							present = presents[0];
							present->setPos(3, 5);
						}
					}
						
        
          break;
        case 50:
          break;
      }
		}

		megaTree.setImage(background);
    idx++;
	  if (idx > 10)
		{
				idx = 0;
				drawBit = !drawBit;
		}

		if (drawBit)
				tree.draw();
		else
				santa.draw();
			
		//santaSlay.move(santaSpeed, 0);
		//santaSlay.draw();

		//if (present != NULL)
		//{
		//	present->move(0, presentSpeed);
		//	present->draw();
		//	if (present->getPosY() > 39)
		//		present = NULL;
		//}

		//objects[currentObject]->move(objectSpeed, 0);
		//objects[currentObject]->draw();
		//if (objects[currentObject]->getPosX() < -23)
		//{
		//	currentObject = rng.uniform(0,objects.size()); //Check if inclusive
		//	printf("Object %i\n", currentObject);
		//	int y = rng.uniform(20,35);
		//	objects[currentObject]->setPos(12, y);
		//}
		megaTree.drawSnow(CV_RGB(255,255,255));

  }

  /* close device (this not explicitly needed in most implementations) */

  /* restore the old port settings */
  tcsetattr(fd,TCSANOW,&oldtio);

  close(fd);

  cvReleaseImage(&image);
  cvDestroyWindow("Test");

  return 0;
}


