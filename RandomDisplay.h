#ifndef RandomDisplay_H
#define RandomDisplay_H

#include "MegaTreeDisplay.h"

class RandomDisplay: public Display
{
public:
  RandomDisplay(MegaTree& mt) : megaTree(mt), rng(time(0))
  {
    printf("Random Display\n");
    displayTime = 300;
    currentObject = 0;
    timer=0;
    numDisplay=0;

    objects.push_back(new Object("sprites/santa.png", megaTree));
    objects.push_back(new Object("sprites/grinch.png", megaTree));
    objects.push_back(new Object("sprites/rudolph.png", megaTree));
    objects.push_back(new Object("sprites/snowman.png", megaTree));
    objects.push_back(new Object("sprites/tree.png", megaTree));
  }

  virtual int process(int key){ 
    timer++;
    megaTree.setColor(CV_RGB(0,0,0));
    if (timer > displayTime)
    {
	    currentObject = rng.uniform(0,objects.size());
			timer=0;
			numDisplay++;
			if (numDisplay > 10)
				exit(0);
		}
		objects[currentObject]->draw();
    megaTree.drawSnow(CV_RGB(255,255,255));
  }

private:
  MegaTree& megaTree;
  std::vector<Object*> objects;
  int displayTime;
  int currentObject;
  int timer;
  int numDisplay;
  cv::RNG rng;
};

#endif
