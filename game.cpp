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


struct Geometry
{
  std::vector<CvPoint> points;
  std::vector<CvScalar> colors;

  Geometry(std::vector<CvPoint> p) :
    points(p)
  {
    for(int i=0; i<points.size(); i++)
			colors.push_back(CV_RGB(255,0,0));
  }

  Geometry(CvPoint p1, CvPoint p2)
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

		//cvDrawCircle(img, cvPoint(p1.x,p1.y), 1, color,1);
		//cvSet2D(img, p1.y,p1.x, color);
		double x=p1.x;
		double y=p1.y;
		int step = 2; //floor((len+1)/150);

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
				//cvSet2D(img, y,x, color);
			//cvDrawCircle(img, cvPoint(x,y), 1, color,1);
		}
  }

	void setColor(int i, CvScalar c) {
		if (i > 0 && i*3+2 < colors.size())
		{
			colors[i*3+0] = c;
			colors[i*3+1] = c;
			colors[i*3+2] = c;
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


void drawLine(IplImage *img, CvPoint p1, CvPoint p2, CvScalar color)
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

	//cvDrawCircle(img, cvPoint(p1.x,p1.y), 1, color,1);
	cvSet2D(img, p1.y,p1.x, color);
	double x=p1.x;
	double y=p1.y;
	int step = floor((len+1)/150);
	for(int i=1;i<len;i++)
	{
		x=x+xIncr;
		y=y+yIncr;
		if (!(i%step))
			cvSet2D(img, y,x, color);
			//cvDrawCircle(img, cvPoint(x,y), 1, color,1);
	}
}
	


//std::map<int, Object> objects;

int main(int argc, char *argv[])
{
  char* filename = argv[1]; 
  
	std::vector<Geometry> megaTree;

	megaTree.push_back(Geometry(cvPoint(555-11,355), cvPoint(555-55,650)));
	megaTree.push_back(Geometry(cvPoint(555-9,355), cvPoint(555-45,650)));
	megaTree.push_back(Geometry(cvPoint(555-7,355), cvPoint(555-35,650)));
	megaTree.push_back(Geometry(cvPoint(555-5,355), cvPoint(555-25,650)));
	megaTree.push_back(Geometry(cvPoint(555-3,355), cvPoint(555-15,650)));
	megaTree.push_back(Geometry(cvPoint(555-1,355), cvPoint(555-5,650)));

	megaTree.push_back(Geometry(cvPoint(555+1,355), cvPoint(555+5,650)));
	megaTree.push_back(Geometry(cvPoint(555+3,355), cvPoint(555+15,650)));
	megaTree.push_back(Geometry(cvPoint(555+5,355), cvPoint(555+25,650)));
	megaTree.push_back(Geometry(cvPoint(555+7,355), cvPoint(555+35,650)));
	megaTree.push_back(Geometry(cvPoint(555+9,355), cvPoint(555+45,650)));
	megaTree.push_back(Geometry(cvPoint(555+11,355),cvPoint(555+55,650)));

  IplImage *image;
  if( (image = cvLoadImage( filename, 1)) == 0 )
    return -1;

  cvNamedWindow("Test", 1);
  cvMoveWindow("Test", 0, 0);

	int idx = 0;
  while(1)
  {

    IplImage *tmpImg = cvCreateImage( cvSize(image->width, image->height), image->depth, image->nChannels);
    cvCopy(image, tmpImg, NULL);
    //Draw the objects
		for(int i=0; i<megaTree.size(); i++)
			megaTree[i].draw(tmpImg);

    cvShowImage("Test", tmpImg);
    int key = cvWaitKey(10);
    cvReleaseImage(&tmpImg);

		

		if (key != -1)
		{
			printf("Key %i\n", key);
			switch (key)
			{
				case 49:
					idx++;
		//			megaTree[5].setColor(idx++, CV_RGB(255,0,0));
					break;

				case 50:
					break;
			}
		}
					
  }

  /* close device (this not explicitly needed in most implementations) */

  cvReleaseImage(&image);
  cvDestroyWindow("Test");
    
  return 0;
}


