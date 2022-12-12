#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "StudentWorld.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(int ID, int sx, int sy, StudentWorld* w, int dir = 0, int depth = 0, double size = 1.0) : GraphObject (ID, sx, sy, dir, depth, size), world(w) {m_dead = false;};
    virtual bool isDead() const {return m_dead;};
    void setDead() {m_dead = true;};
    virtual void bonk() = 0;
    virtual void doSomething() = 0;
    
    virtual bool doesBlock() = 0;
    virtual bool isDamagable() = 0;
    virtual void damage() = 0;
    
    bool canFall();
    bool canMoveLeft(int m);
    bool canMoveRight(int m);
    
    StudentWorld* getWorld() const {return world;};
    
private:
    StudentWorld* world;
    bool m_dead;
};


class Peach : public Actor
{
public:
    Peach(int x, int y, StudentWorld* w) : Actor(IID_PEACH, x, y, w), hitPoints(1), starInv(0),  tempInv(0), star(false), shoot(false), jump(false)
    {
        remaining_jump_dist = 0;
        time_to_recharge = 0;
    };
    void bonk();
    void doSomething();
    bool doesBlock() {return false;};
    bool isDamagable() {return true;};
    void damage() {bonk();};
    bool isInv() {return (starInv>0);};
    bool hasJ () const {return jump;};
    bool hasSh () const {return shoot;};
    
    
    int getHit() const {return hitPoints;};
    void setHit(int a) {hitPoints = a;};
    void addPowerUp(char g);
    
private:
    int hitPoints;
    int remaining_jump_dist;
    int time_to_recharge;
    int starInv;

    int tempInv;
    bool star;
    bool shoot;
    bool jump;
};

class Blocker : public Actor
{
public:
    Blocker(int ID, int x, int y, StudentWorld* w) : Actor(ID, x, y, w, 0, 2, 1) {};
    virtual void bonk() = 0;
    void doSomething() {return;};
    bool doesBlock() {return true;};
    bool isDamagable() {return false;};
    void damage() {return;};
private:
    
    
};

class Block : public Blocker
{
public:
    Block (int x, int y, StudentWorld* w, char g) : Blocker(IID_BLOCK, x, y, w), goodieRelease(g) {   };
    
    void bonk();
    
private:
    char goodieRelease; //set to n if no goodie, s if star, f if flower, m if mushroom
};

class Pipe : public Blocker
{
public:
    Pipe (int x, int y, StudentWorld* w) : Blocker(IID_PIPE, x, y, w) {};
    
    void bonk() {return;};
private:
    
};


class Goodie : public Actor
{
public:
    Goodie (int ID, int x, int y, StudentWorld* w) : Actor(ID, x, y, w, 0, 1, 1) {};
    void bonk() {return;};
    bool doesBlock() {return false;};
    bool isDamagable() {return false;};
    void damage() {return;};
    
    void doSomething();
private:
    
};
class Mushroom : public Goodie
{
public:
    Mushroom(int x, int y, StudentWorld* w) : Goodie(IID_MUSHROOM, x, y, w) {};
    void doSomething();
private:
    
    
};
class Star : public Goodie
{
public:
    Star (int x, int y, StudentWorld* w) : Goodie(IID_STAR, x, y, w) {};
    void doSomething();
private:
    
};
class Flower : public Goodie
{
public:
    Flower(int x, int y, StudentWorld* w) : Goodie(IID_FLOWER, x, y, w) {};
    void doSomething();
private:
    
};

class LevelOver : public Actor
{
public:
    LevelOver (int ID, int x, int y, StudentWorld* w) : Actor (ID, x, y, w, 0, 1, 1) {};
    void bonk() {return;};
    void doSomething();
    
    bool doesBlock() {return false;};
    bool isDamagable() {return false;};
    void damage(){return;};
    
};


class Flag : public LevelOver
{
public:
    Flag(int x, int y, StudentWorld* w) : LevelOver(IID_FLAG, x, y, w) {};
    void doSomething();
    
};
class Mario : public LevelOver
{
public:
    Mario (int x, int y, StudentWorld* w) : LevelOver (IID_MARIO, x, y, w) {};
    void doSomething();
};

class Fireball : public Actor
{
public:
    Fireball(int ID, int x, int y, int dir, StudentWorld* w) : Actor (ID, x, y, w, dir, 1, 1) {};
    
    void doSomething();
    bool doesBlock() {return false;};
    void bonk() {return;};
    bool isDamagable() {return false;};
    void damage() {return;};
       
};

class PeachFireball : public Fireball
{
public:
    PeachFireball(int ID, int x, int y, int dir, StudentWorld* w) : Fireball (ID, x, y, dir, w) {};
    void doSomething();
    
};
class PirFireball : public Fireball
{
public:
    PirFireball(int x, int y, int dir, StudentWorld* w) : Fireball (IID_PIRANHA_FIRE, x, y, dir, w) {};
    void doSomething();
    
};

class Shell : public PeachFireball
{
public:
    Shell (int x, int y, int dir, StudentWorld* w) : PeachFireball (IID_SHELL, x, y, dir, w) {};
    void doSomething();
};


class Enemy : public Actor
{
public:
    Enemy (int ID, int x, int y, StudentWorld* w) : Actor (ID, x, y, w, randInt(0, 180), 1, 0) {};
    void doSomething();
    void bonk();
    void damage();
    
    bool doesBlock() {return false;};
    bool isDamagable() {return true;};
   
private:
    
};

class Goomba : public Enemy
{
public:
    Goomba (int x, int y, StudentWorld* w) : Enemy (IID_GOOMBA, x, y, w) {};
    
private:
};

class Koopa : public Enemy
{
public:
    Koopa (int x, int y, StudentWorld* w) : Enemy (IID_KOOPA, x, y, w) {};
    void bonk();
    void damage();
};

class Piranha : public Enemy
{
public:
    Piranha (int x, int y, StudentWorld* w) : Enemy(IID_PIRANHA, x, y, w), firing_delay(0) {};
    void doSomething();
private:
    int firing_delay;
};


#endif // ACTOR_H_
