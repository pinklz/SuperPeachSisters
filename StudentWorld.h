#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <list>
#include <string>
#include <sstream>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
    ~StudentWorld() {cleanUp();};
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    
    bool isBlockingObjectAt(const int x, const int y, Actor* current);
    bool enemyFall(int x, int y, Actor* current);
    
    bool bdIfOverlap(const Actor* cur, char a);
    bool overlapPeach(const Actor* cur);
    void bonkPeach(const Actor* cur);
    void damagePeach() {ppeach->damage();};
    bool isInv() {return ppeach->isInv();};
    
    
    int getPeachX() const {return ppeach->getX();};
    int getPeachY() const {return ppeach->getY();};
    
    void setPHit(int m) {ppeach->setHit(m);};
    void addPpowerUp(char a) {ppeach->addPowerUp(a);};
    
    void addActor(Actor* na) {actors.push_back(na);};
    
    void levelComp() {levelOver = true;};
    void gameComp() {gameOver = true;};
    
    std::string getStat() {return stats.str();};
    
    
private:
   std::list<Actor*> actors;
    Peach* ppeach;
    
    std::ostringstream stats;
    
    bool levelOver;
    bool gameOver;
};

#endif // STUDENTWORLD_H_
