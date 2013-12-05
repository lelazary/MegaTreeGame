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
  }

  virtual int process(int key){ 
    megaTree.setColor(CV_RGB(0,0,0));
    printf("SetColor %i %i\n", stringId, red);
    megaTree.setColor(stringId,CV_RGB(red, green, blue));
  }

private:
  MegaTree& megaTree;
  int stringId;
  unsigned char red;
  unsigned char green;
  unsigned char blue;

};

#endif
