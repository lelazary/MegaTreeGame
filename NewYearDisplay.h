#ifndef NewYearDisplay_H
#define NewYearDisplay_H

#include "MegaTreeDisplay.h"

class NewYearDisplay: public Display
{
public:
  NewYearDisplay(MegaTree& mt) : megaTree(mt)
  {
    fireWorksPos = 0;
    fireWorksState = 1;
		fireWorksStart = 0;
		fireWorksColor = CV_RGB(255,0,0);
    printf("Setup NewYear\n");
  }

  virtual int process(int key){ 
    megaTree.setColor(CV_RGB(0,0,0));

    switch(fireWorksState)
    {
			case 0:
			{
				megaTree.setPixel(fireWorksStart,fireWorksPos, fireWorksColor);
				fireWorksPos--;
				if (fireWorksPos < 10)
				{
					fireWorksPos = 0;
					fireWorksState = 1;
				}
				break;
			}
			case 1:
			{
				float a = 6;
				float b = 10;
				int r = fireWorksPos++;
				if (fireWorksPos > 8)
				{
					fireWorksPos = 50;
					fireWorksState = 0;
					fireWorksStart = rng.uniform(0,12);
					fireWorksColor = CV_RGB(
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2); 

				}
				cv::Scalar c1 = CV_RGB(
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2); 
				cv::Scalar c2 = CV_RGB(
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2); 
				cv::Scalar c3 = CV_RGB(
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2, 
							25+rng.uniform(0,100)*2); 

				for(int i=0; i<360; i++)
				{
					int x = a + r * cos(float(i)*180/M_PI);
					int y = b + r * sin(float(i)*180/M_PI);

					int n = rng.uniform(0,3);
					megaTree.setPixel(x+n,y+n, c1);

					x = a + (r-2) * cos(float(i)*180/M_PI);
					y = b + (r-2) * sin(float(i)*180/M_PI);
					n = rng.uniform(0,3);
					megaTree.setPixel(x+n,y+n,c2);

					x = a + (r-4) * cos(float(i)*180/M_PI);
					y = b + (r-4) * sin(float(i)*180/M_PI);
					n = rng.uniform(0,3);
					megaTree.setPixel(x+n,y+n, c3);
				}
				
			}
		}
  }

private:
  MegaTree& megaTree;
  cv::RNG rng;
  int fireWorksPos;
  int fireWorksStart;
  int fireWorksState;
	cv::Scalar fireWorksColor;
};

#endif
