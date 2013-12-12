#ifndef XmesGameDisplay_H
#define XmesGameDisplay_H

#include "MegaTreeDisplay.h"

class XmesGameDisplay: public Display
{
public:
  XmesGameDisplay(MegaTree& mt) : megaTree(mt)
  {
    printf("XMes Game Display\n");

    santaSlay = new Object("sprites/santaSlay.png", megaTree);
    grinchSlay = new Object("sprites/grinchSlay.png", megaTree);

    cannon1 = new Object("sprites/cannon1.png", megaTree);
    cannon1->setPos(0,38);

    heart = new Object("sprites/heart.png", megaTree);

    cannon2 = new Object("sprites/cannon2.png", megaTree);
    cannon2->setPos(0,38);

    presents.push_back(new Object("sprites/present1.png", megaTree));

    background = cv::imread("sprites/background.png", -1); //Load RGBA png image

    objects.push_back(new Object("sprites/house1.png", megaTree));
    objects.push_back(new Object("sprites/house2.png", megaTree));
    objects.push_back(new Object("sprites/snowman.png", megaTree));
    objects.push_back(new Object("sprites/treeSmall.png", megaTree));
    objects.push_back(new Object("sprites/treeSmall2.png", megaTree));
    objects.push_back(new Object("sprites/grinchHouse.png", megaTree));

    santa = new Object("sprites/santa.png", megaTree);
    //objects.push_back(new Object("sprites/santa.png", megaTree));
    //objects.push_back(new Object("sprites/santaFace.png", megaTree));
    //objects.push_back(new Object("sprites/tree.png", megaTree));
    //objects.push_back(new Object("sprites/snowman.png", megaTree));
    //objects.push_back(new Object("sprites/grinch.png", megaTree));

    currentObject = 0;
    y = rng.uniform(20,35);
    objects[currentObject]->setPos(12, y);

    present = NULL;
    presentSpeed = 2.0/5;
    objectSpeed = 0.5/5;
    santaSpeed = 3.0/5;
    idx = 0;
    drawBit = 0;
    fireHeart = 0;
    gameState = 1;

    music.playBackground("music/deck.mp3");
    gameTime = 0;

  }

  virtual int process(int key){ 
    //key = (rng.uniform(0,100) < 15) ? 49 : -1; //Play randomly
    
    gameTime++;
    switch(gameState)
    {
       case 1: 
        if (gameMode1(key))
	{
	  gameTime = 0;
          gameState = 2;
	}
        break;
       case 2: 
	if (gameTime > 300)
        {
	 music.playBackground("music/deck.mp3");
	 gameState = 1;
        }
        gameMode2(key);
        break;
    }
  }

  int gameMode1(int key)
  {
    megaTree.setImage(background);

    if (key != -1)
    {
      printf("Key %i %i\n", key, 'f');
      switch (key)
      {
        case 'f':
          if (present == NULL)
          {
            if (santaSlay->getPosX() < -5 && santaSlay->getPosX() > -20)
            { 
              music.playFX(Music::DROP);
              present = presents[0];
              present->setPos(3, 5);
            }

          }
          break;
        case 50:
          break;
      }
      //cannon2.draw();
      //fireHeart = 1;
      //heart.setPos(0,30);

    } else {
      //  cannon1.draw();

    }


    //Random Display
    //////idx++;
    //////if (idx > 60)
    //////{
    //////  idx = 0;
    //////  currentObject = rng.uniform(0,objects.size()); 
    //////}
    //////objects[currentObject]->draw();

    //////santa.draw();

    //////if (fireHeart)
    //////{
    //////    heart.draw();
    //////    heart.move(0,-2);
    //////    if (heart.getPosY() < 10)
    //////      fireHeart = 0;
    //////}

    santaSlay->move(santaSpeed, 0);
    santaSlay->draw();

    if (present != NULL)
    {
      present->move(0, presentSpeed);
      present->draw();
      if (present->getPosY() > 39)
        present = NULL;
      else
      {
        if (intersect(present->getBB(), objects[currentObject]->getBB()))
        {
          present = NULL;
          if (currentObject == 5)
          {
            music.playFX(Music::GRINCH_HIT);
            music.playBackground("music/grinch.mp3");
            return 1;
          } else {
            if (currentObject == 3 ||
                currentObject == 4)
            {
              music.playFX(Music::BAD_HIT);
            } else {
              music.playFX(Music::GOOD_HIT);
            }
          }

        }

      }
    }

    objects[currentObject]->move(objectSpeed, 0);
    objects[currentObject]->draw();
    if (objects[currentObject]->getPosX() < -23)
    {
      currentObject = rng.uniform(0,objects.size()); //Check if inclusive
      int y = rng.uniform(20,35);
      objects[currentObject]->setPos(12, y);
    }

    megaTree.drawSnow(CV_RGB(255,255,255));

    return 0;

  }

  int gameMode2(int key)
  {
    megaTree.setImage(background);

    if (key != -1)
    {
      switch (key)
      {
        case 'f':
          music.playFX(Music::LASER);
          cannon2->draw();
          fireHeart = 1;
          heart->setPos(0,30);
          break;
        case 50:
          break;
      }

    } else {
      cannon1->draw();
    }


    if (fireHeart)
    {
        heart->draw();
        heart->move(0,-2);
        if (heart->getPosY() < 10)
          fireHeart = 0;
    }

    grinchSlay->move(santaSpeed, 0);
    grinchSlay->draw();

    megaTree.drawSnow(CV_RGB(255,255,255));


  }

private:
  MegaTree& megaTree;
  cv::RNG rng;
  Music music;

  Object* santaSlay;
  Object* grinchSlay;
  Object* cannon1;
  Object* heart;
  Object* cannon2;
  std::vector<Object*> presents;
  cv::Mat background;
  std::vector<Object*> objects;
  Object* santa;

  Object* present;
  double presentSpeed;
  double objectSpeed;
  double santaSpeed;
  int currentObject;
  int y;
  int idx;
  int drawBit;
  int fireHeart;
  int gameState;
  int gameTime;


};

#endif
