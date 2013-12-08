#ifndef SetupDisplay_H
#define SetupDisplay_H

#include "MegaTreeDisplay.h"

class SetupDisplay: public Display
{
public:
  SetupDisplay(MegaTree& mt, int id, unsigned char r, unsigned char g, unsigned char b) : 
    megaTree(mt),
    stringId(id),
    red(r),
    green(g),
    blue(b)
  {
    printf("Setup Display\n");
    idx = 0;
  }

  virtual int process(int key){ 
    megaTree.setColor(CV_RGB(0,0,0));
    printf("SetColor %i %i (%i,%i,%i)\n", stringId, idx, red, green , blue);
    //megaTree.setColor(stringId,CV_RGB(red, green, blue));
    megaTree.setPixel(stringId ,idx,CV_RGB(red, green, blue));
    idx--;
    if (idx < 0) idx = 49;
  }

private:
  MegaTree& megaTree;
  int stringId;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  int idx;

};

#endif
