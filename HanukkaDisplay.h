#ifndef HanukkaDisplay_H
#define HanukkaDisplay_H

#include "MegaTreeDisplay.h"

class HanukkaDisplay: public Display
{
public:
  HanukkaDisplay(MegaTree& mt, int nc) : megaTree(mt),  numCandels(nc)
  {
    printf("Setup Hannukka\n");
    sevivonIdx = 60;
    sevivonVal = rng.uniform(0,4);
    sevivonDisplayTime = 20;

    sevivon.push_back(new Object("sprites/Sevivon1.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon2.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon3.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon4.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon5.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon6.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon7.png", megaTree));
    sevivon.push_back(new Object("sprites/Sevivon8.png", megaTree));

    menora = new Object("sprites/menora.png", megaTree);

    banner = new Object("sprites/happyhanukkah.png", megaTree);
  }

  virtual int process(int key){ 
    megaTree.setColor(CV_RGB(0,0,0));
    if (sevivonDisplayTime > 0)
    {
    	if (sevivonIdx <= 0)
    	{
    		sevivon[1+(sevivonVal*2)]->draw();
    		sevivonDisplayTime--;
    	} else {
    		//Spin sevivon
    		sevivon[7-(sevivonIdx%8)]->draw();
    		sevivonIdx--;
    	}
    } else {
    	banner->draw();
    	if (banner->move(2,0, 0, -150))
    	{
    		sevivonIdx = 50+rng.uniform(0,10);
    		sevivonVal = rng.uniform(0,4);
    		sevivonDisplayTime = 20;
    	}
    }
    drawMenora(megaTree, *menora);
  }

  void drawMenora(MegaTree& tree, Object& base)
  {
    base.setPos(0,30);
    base.draw();

    //Draw the samas
    unsigned char flameR = rng.uniform(100,255);
    unsigned char flameG = rng.uniform(50,150);
    unsigned char flameB = rng.uniform(50,150);
    unsigned char flameSize = rng.uniform(1,3);
    for(int i=0; i<5; i++) //Candel base
      tree.setPixel(10,25+i, CV_RGB(0, 0,255));
    for(int i=0; i<flameSize; i++) //Candel flame
      tree.setPixel(10,24-i, CV_RGB(flameR, flameG,0));

    for(int c=0; c<numCandels; c++)
    {
      flameR = rng.uniform(100,255);
      flameG = rng.uniform(50,150);
      flameB = rng.uniform(50,150);
      flameSize = rng.uniform(1,3);

      for(int i=0; i<5; i++) //Candel base
        tree.setPixel(c,28+i, CV_RGB(0, 0,255));
      for(int i=0; i<flameSize; i++) //Candel flame
        tree.setPixel(c,27-i, CV_RGB(flameR, flameG,0));
    }
  }

private:
  MegaTree& megaTree;
  std::vector<Object*> sevivon;
  Object *menora;
  Object *banner;
  int sevivonIdx;
  int sevivonVal;
  int sevivonDisplayTime;
  cv::RNG rng;
  int numCandels;
};

#endif
