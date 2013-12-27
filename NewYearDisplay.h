#ifndef NewYearDisplay_H
#define NewYearDisplay_H

#include "MegaTreeDisplay.h"

class NewYearDisplay: public Display
{
public:
  NewYearDisplay(MegaTree& mt) : megaTree(mt)
  {
    fireWorksPos = 49;
    fireWorksState = 0;
		fireWorksStart = 0;
		fireWorksColor = CV_RGB(255,0,0);
		displayState = 4;
		snowflakePosX = 0;
		snowflakePosY = 0;
    printf("Setup NewYear\n");

		banner = new Object("sprites/happynewyear.png", megaTree);
		snowflake = new Object("sprites/snowflake.png", megaTree);
		
  }

  virtual int process(int key){ 
		megaTree.setColor(CV_RGB(0,0,0));
	

		switch(displayState)
		{
			case 0:
				banner->draw();
				if (banner->move(2,0, 0, -150))
					displayState = 1;
				break;
			case 1:
			case 2:
			case 3:
				if (fireWorks())
					displayState++;
				break;
			case 4:
				if (drawSnowflakes())
					displayState=0;
				break;
		}

	}


	int drawSnowflakes()
	{
		snowflakePosY++;
		if (snowflakePosY > 100)
			return 1;
		for(int i=0; i<5; i++)
		{
			snowflakePosX = (-2 + rng.uniform(0,6));
			if (snowflakePosX < -2) snowflakePosX = -2;
			if (snowflakePosX > 10) snowflakePosX = 10;
			snowflake->setPos(snowflakePosX,snowflakePosY-i*10);
			snowflake->draw();
		}
		return 0;
	}

	int fireWorks()
	{
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
						return 1;

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
		return 0;
	}

private:
MegaTree& megaTree;
cv::RNG rng;
int fireWorksPos;
int fireWorksStart;
int fireWorksState;
int displayState;
cv::Scalar fireWorksColor;
Object *banner;
Object *snowflake;
int snowflakePosX;
int snowflakePosY;
};

#endif
