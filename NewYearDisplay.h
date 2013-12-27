#ifndef NewYearDisplay_H
#define NewYearDisplay_H

#include "MegaTreeDisplay.h"
#include <list>

struct Line
{
	int x;
	int y;
	int length;
	int dir;
	cv::Scalar color;

	Line(int _x, int _y, int _l, int _d, cv::Scalar _c) :
		x(_x), y(_y), length(_l), dir(_d), color(_c)
	{}
};

class NewYearDisplay: public Display
{
public:
  NewYearDisplay(MegaTree& mt) : megaTree(mt)
  {
    printf("Setup NewYear\n");

    fireWorksPos = 49;
    fireWorksState = 0;
		fireWorksStart = 0;
		fireWorksColor = CV_RGB(255,0,0);
		displayState = 0;
		snowflakePosX = 0;
		snowflakePosY = 0;
		numFrames = 0;

		algDisplayCnt = 12;
		algDisplayDir = rng.uniform(-3,3);
		if (algDisplayDir == 0) algDisplayDir = 1;
		algDisplayLength = rng.uniform(1,10);
		algDisplayNewRndColor = rng.uniform(0,2);
		algDisplayLoopIdx = rng.uniform(0,2);


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
				{
					snowflakePosY = 0;
					displayState++;
				}
				break;
			case 4:
				if (drawSnowflakes())
					displayState++;
				break;
			case 5:
				numFrames++;
				if (!(numFrames%50))
				{
					algDisplayCnt = 12;
					algDisplayDir = rng.uniform(-3,3);
					if (algDisplayDir == 0) algDisplayDir = 1;
					algDisplayLength = rng.uniform(1,10);
					algDisplayNewRndColor = rng.uniform(0,2);
					algDisplayLoopIdx = rng.uniform(0,2);
				}

				treeAlgDisplay(algDisplayCnt, algDisplayDir, algDisplayLength,
							algDisplayNewRndColor, algDisplayLoopIdx);
				if (numFrames > 500)
				{
					numFrames = 0;
					displayState=0;
				}
				break;
		}

	}

	int treeAlgDisplay(int cnt, int dir, int length, int newRndColor, int loopIdx)
	{


		int start = (dir > 0) ? 0 : 40;
		cv::Scalar color = CV_RGB(
				25+rng.uniform(0,100)*2, 
				25+rng.uniform(0,100)*2, 
				25+rng.uniform(0,100)*2); 
    linePos = (linePos+1)%24;

		for(int i=0; i<cnt; i++)
		{
			if (newRndColor)
				color = CV_RGB(
						25+rng.uniform(0,100)*2, 
						25+rng.uniform(0,100)*2, 
						25+rng.uniform(0,100)*2); 

			int idx = i;
			if (loopIdx)
				idx = abs(-12 + linePos);
			lines.push_back(Line(idx,start,length,length*dir,color));
		}

		displayLines();

		return 0;
	}


  int displayLines()
	{
		for(std::list<Line>::iterator it=lines.begin(); it != lines.end();)
		{
			if (it->y > -1 && it->y < 50)
			{
				for(int i=0; i<it->length; i++)
					megaTree.setPixel(it->x,it->y+i, it->color);
				it->y += it->dir;
				++it;
			} else {
				it = lines.erase(it);
			}
				
		}
		return 0;
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
					if (r<0) r = 0;
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

					for(int i=0; i<360; i += 3+rng.uniform(5,10))
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
int numFrames;
int linePos;
std::list<Line> lines;

int algDisplayCnt;
int algDisplayDir;
int algDisplayLength;
int algDisplayNewRndColor;
int algDisplayLoopIdx;


};

#endif
