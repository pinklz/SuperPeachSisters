#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    stats<< "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    
    
    ostringstream lv;
    lv.fill('0');
    lv << "level" <<setw(2)<< getLevel() << ".txt";
    string level_file = lv.str();

    Level::LoadResult result = lev.loadLevel(level_file);


    
    if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    else if (result == Level::load_success)
    {
        Level::GridEntry ge;
        
        levelOver = false;
        gameOver = false;
        
        for (int c = 0; c < GRID_WIDTH; c++)
        {
            for (int r = 0; r < GRID_HEIGHT; r++)
            {
                ge = lev.getContentsOf(c, r);
                int x = c*SPRITE_WIDTH;
                int y = r*SPRITE_HEIGHT;
                
                switch(ge)
                {
                    case Level::empty:
                        break;
                    case Level::peach:
                    {
                        //create new peach with coordinates
                        ppeach = new Peach(x, y, this);
                        actors.push_back(ppeach);
                        break;
                    }
                    case Level::block:
                    {
                        //create new block with coordinates
                        actors.push_back(new Block(x, y, this, 'n'));
                        break;
                    }
                    case Level::star_goodie_block:
                    {
                        actors.push_back(new Block (x, y, this, 's'));
                        break;
                    }
                    case Level::mushroom_goodie_block:
                    {
                        actors.push_back(new Block (x, y, this, 'm'));
                        break;
                    }
                    case Level::flower_goodie_block:
                    {
                        actors.push_back(new Block (x, y, this, 'f'));
                        break;
                    }
                    case Level::flag:
                    {
                        actors.push_back(new Flag(x, y, this));
                        break;
                    }
                    case Level::pipe:
                    {
                        actors.push_back(new Pipe(x, y, this));
                        break;
                    }
                    case Level::goomba:
                    {
                        actors.push_back(new Goomba(x, y, this));
                        break;
                    }
                    case Level::koopa:
                    {
                        actors.push_back(new Koopa(x, y, this));
                        break;
                    }
                    case Level::piranha:
                    {
                        actors.push_back(new Piranha(x, y, this));
                        break;
                    }
                    case Level::mario:
                    {
                        actors.push_back(new Mario(x, y, this));
                        break;
                    }
                }
            }
        }
        
        
    }
    
    //set status bar
    setGameStatText(stats.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{ 
    //update text on screen
    stats.str("");
    stats<< "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore();
    if (isInv())
    {
        stats << " StarPower!";
    }
    if (ppeach->hasJ())
    {
        stats << " JumpPower!";
    }
    if (ppeach->hasSh())
    {
        stats << " ShootPower!";
    }
    setGameStatText(stats.str());
    
   //Give Each actor a chance to do something
    for (list<Actor*>::iterator p = actors.begin(); p!= actors.end(); p++)
    {
        Actor* a = *p;
        
        
        
        if (!a->isDead())
        {
            a->doSomething();
        }
        if (ppeach->isDead())
        {
            playSound(SOUND_PLAYER_DIE);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        

        if (gameOver)
        {
            playSound(SOUND_GAME_OVER);
            return GWSTATUS_PLAYER_WON;
        }
        if (levelOver)
        {
            playSound(SOUND_FINISHED_LEVEL);
            
            return GWSTATUS_FINISHED_LEVEL;
        }
        
    }
    
    
    //remove dead actors after each tick
    for (list<Actor*>::iterator d = actors.begin(); d != actors.end(); )
    {
        Actor* a = *d;
        if (a->isDead())
        {
            delete a;
            d = actors.erase(d);
        }
        else
            d++;
    }
    
    


    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator p = actors.begin(); p!= actors.end(); )
    {
        Actor* a = *p;
        delete a;
        p = actors.erase(p);
    }
    
}



bool StudentWorld::bdIfOverlap(const Actor* cur, char c)
{
    //returns true if it bonked or damaged something (whatever it was asked to do)
    Actor* ov = nullptr;
    if (cur == nullptr)
        return false;

    int cx = cur->getX();
    int cy = cur->getY();
    
    
    
    //sets ov to any object that it overlaps with
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        Actor* a = *p;
        int ax = a->getX();
        int ay = a->getY();
        
         if (cx>=ax && cx <=ax+SPRITE_WIDTH-1 &&
             ((ay>=cy && ay <=cy+SPRITE_HEIGHT-1) || (cy>=ay && cy<=ay+SPRITE_HEIGHT-1)) && a!= cur)
        {
            ov = a;
        }
        
        if ((ax >= cx && ax <= cx+SPRITE_WIDTH-1) &&
            (ay >= cy && ay <= cy+SPRITE_HEIGHT-1)
            && a!= cur)
        {
            ov = a;
        }
                
        
    }
    if (ov==nullptr)
        return false;
    
    

    if (ov != nullptr && c == 'b')
    {
        ov->bonk();
        return true;
    }
    else if (ov != nullptr && c == 'd' && ov->isDamagable())
    {
        ov->damage();
        return true;
    }
    else
        return false;
}

bool StudentWorld::overlapPeach(const Actor* cur)
{
    int cx = cur->getX();
    int cy = cur->getY();
    
    int px = ppeach->getX();
    int py = ppeach->getY();
    
    if ((cx >= px && cx<=px+SPRITE_WIDTH-1 && cy>=py && cy<=py+SPRITE_HEIGHT-1) ||
        (px >=cx && px<=cx+SPRITE_WIDTH-1 && py>=cy && py <=cy+SPRITE_HEIGHT-1))
        return true;
    else
        return false;
    
}

void StudentWorld::bonkPeach (const Actor* cur)
{
        ppeach->bonk();
}


bool StudentWorld::isBlockingObjectAt(const int x, const int y, Actor* current)
{


    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        Actor* a = *p;
        if (a->getX() == x && a->getY() == y && a->doesBlock() && a != current)
            return true;
    }
    

    return false;
}




bool StudentWorld::enemyFall(int x, int y, Actor* current)
{


        for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
        {
            Actor* a = *p;
            if(a == current)
                continue;;
            
            if (x >= a->getX() && x <= a->getX() + SPRITE_WIDTH && y >= a->getY() && y <= a->getY() + SPRITE_WIDTH)
                return false;
        }


    return true;
}
