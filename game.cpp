#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "opencv/cv.h"
#include "opencv/highgui.h"
#endif
#include <stdlib.h>
#include <stdio.h>


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
      strings[i].draw(img);
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

int main(int argc, char *argv[])
{
  char* filename = argv[1]; 

  MegaTree megaTree(cvPoint(555,355), cvPoint(0,295), 12);
  IplImage *image;
  if( (image = cvLoadImage( filename, 1)) == 0 )
    return -1;

  cvNamedWindow("Test", 1);
  cvMoveWindow("Test", 0, 0);

  cv::Mat sprite = cv::imread(argv[2], -1); //Load RGBA png image

  megaTree.setColor(CV_RGB(0,0,255));

  int offset = 0;
  
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
          offset++;
          if (offset > 50)
            offset = 0;
        
          break;
        case 50:
          break;
      }
      printf("Offset %i\n", offset);
      megaTree.setColor(CV_RGB(0,0,255));
      megaTree.setImage(sprite, 0, offset);
      //megaTree.setPixel(0, 0, CV_RGB(255,255,255));
      megaTree.drawSnow(CV_RGB(255,255,255));
    }

  }

  /* close device (this not explicitly needed in most implementations) */

  cvReleaseImage(&image);
  cvDestroyWindow("Test");

  return 0;
}


