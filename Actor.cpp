#include "Actor.h"
#include "StudentWorld.h"


bool Actor::canFall()
{
    int cx = getX();
    int cy = getY();
    
    
    for (int x = - SPRITE_WIDTH+3; x < SPRITE_WIDTH-3; x++)
    {
        for (int y = -2; y <2; y++)
        {
            int tx = cx + x;
            int ty = cy - y - SPRITE_HEIGHT;
            if (getWorld()->isBlockingObjectAt(tx, ty, this))
                return false;
        }
    }
    
    return true;
}


bool Actor::canMoveLeft(int m)
{
    int tarX = getX()-m;
    int curY = getY();


    for (int h = -SPRITE_HEIGHT+1; h<SPRITE_HEIGHT/2; h++)
    {
        //outer loop loops through possible y coordinates
        //inner from tarX-SPRITE_WIDTH/2
        //up to m+SPRITE_WIDTH/2
        if (getWorld()->isBlockingObjectAt(tarX-(SPRITE_WIDTH/2), curY-h, this))
            return false;

        int add = 0;
        while (m+(SPRITE_WIDTH/2)+add < SPRITE_WIDTH)
        {
            if (getWorld()->isBlockingObjectAt(tarX-(SPRITE_WIDTH/2)-add, curY-h, this))
                return false;
            add++;
        }
    }


    return true;

}



bool Actor::canMoveRight(int m)
{
    int tarX = getX() + m;
    int curY = getY();


    for (int h = -SPRITE_HEIGHT+1; h<SPRITE_HEIGHT/2; h++)
    {

        if (getWorld()->isBlockingObjectAt(tarX+(SPRITE_WIDTH/2), curY-h, this))
            return false;

        int add = 0;
        while (m+(SPRITE_WIDTH/2)+add < SPRITE_WIDTH)
        {
            if (getWorld()->isBlockingObjectAt(tarX+(SPRITE_WIDTH/2)+add, curY-h, this))
                return false;
            add++;
        }
    }

    return true;
}


//PEACH IMPLEMENTATION


void Peach::doSomething()
{
    if (isDead())
        return;
    
    if (starInv == 0)
        star = false;
    if (star)
    {
        starInv--;
    }

    
    if (tempInv>0)
        tempInv--;
    
    
    if (time_to_recharge > 0)
    {
        time_to_recharge--;
    }
    

    
    
    if (remaining_jump_dist > 0)
    {
        int cx = getX();
        int cy = getY();
        
        if (getWorld() -> isBlockingObjectAt(cx, cy+(SPRITE_HEIGHT/2), this) || getWorld() ->isBlockingObjectAt(cx+(SPRITE_WIDTH/2), cy+(SPRITE_HEIGHT/2), this) || getWorld()->isBlockingObjectAt(cx-(SPRITE_WIDTH/2), cy+(SPRITE_HEIGHT/2), this))
        {

            getWorld()->bdIfOverlap(this, 'b');

            
            remaining_jump_dist = 0;
        }
        else
        {
            moveTo(cx, cy+4);
            remaining_jump_dist--;
        }
        
    }
        
        if (remaining_jump_dist == 0)
        {
            if (canFall())
                moveTo(getX(), getY()-4);
        }
    

    
    
    
    //checks for user input
    int ch;
    if (getWorld() -> getKey(ch))
    {
        int curX = getX();
        int curY = getY();
        switch (ch)
        {

            case (KEY_PRESS_LEFT):
            {
                int tarX = curX-4;
                setDirection(180);

                if (!canMoveLeft(4))
                {

                    getWorld()->bdIfOverlap(this, 'b');
                }
                else
                    moveTo(tarX, curY);

                break;
            }

            case (KEY_PRESS_RIGHT):
            {
                int tarX = curX+4;
                setDirection(0);

                if (!canMoveRight(4))
                {

                    getWorld()->bdIfOverlap(this, 'b');

                }
                else
                    moveTo(tarX, curY);

                break;
            }

            case (KEY_PRESS_UP):
            {
                
                if (getWorld()->isBlockingObjectAt(curX, curY-SPRITE_HEIGHT, this)
                    || getWorld()->isBlockingObjectAt(curX+(SPRITE_WIDTH/2), curY-SPRITE_HEIGHT, this) || getWorld()->isBlockingObjectAt(curX-(SPRITE_WIDTH/2), curY-SPRITE_HEIGHT, this))
                {
                    if (jump)
                        remaining_jump_dist = 12;
                    else
                        remaining_jump_dist = 8;

                    getWorld()->playSound(SOUND_PLAYER_JUMP);
                }

                break;
            }
            case (KEY_PRESS_SPACE):
            {
                if (!shoot || time_to_recharge>0)
                    break;
                else
                {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge = 8;
                    int addinDir;
                    if (getDirection() == 0)
                        addinDir = 4;
                    else
                        addinDir = -4;
                    
                    PeachFireball* pf = new PeachFireball(IID_PEACH_FIRE, getX()+addinDir, getY(), getDirection(), getWorld());
                    
                    getWorld()->addActor(pf);
                }
                
                break;
            }
        }
    }

}


void Peach::bonk()
{
    if (tempInv > 0 || starInv >0)
    {
        return;
    }
    else
    {
        setHit(getHit()-1);
        tempInv = 10;
        if (shoot)
            shoot = false;
        if (jump)
            jump = false;
        
        if (hitPoints >0)
            getWorld()->playSound(SOUND_PLAYER_HURT);
        else if (hitPoints==0)
        {
            setDead();
        }
        
    }
    
}

void Peach::addPowerUp(char g)
{
    switch(g)
    {
        case ('f'):
        {
            shoot = true;
            break;
        }
        case ('s'):
        {
            star = true;
            starInv = 150;
            break;
        }
        case ('m'):
        {
            jump = true;
            break;
        }
    }
}

//BLOCK IMPLEMENTATION



void Block::bonk()
{
    switch (goodieRelease)
    {
        case ('n'):
        {
            getWorld()->playSound(SOUND_PLAYER_BONK);
            break;
        }
        case ('s'):
        {
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            //create NEW star object 8 pix above
            
            Actor* st = new Star(getX(), getY()+SPRITE_HEIGHT, getWorld());
            getWorld()->addActor(st);
            //ADD STAR to StudentWorld's list of actors
            //change goodieRelease to n
            goodieRelease = 'n';
            break;
        }
        case ('f'):
        {
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            //create NEW flower object 8 pix above

            Actor* fl = new Flower(getX(), getY()+SPRITE_HEIGHT, getWorld());
            getWorld()->addActor(fl);

            goodieRelease = 'n';
            break;
        }
        case ('m'):
        {
            //play sound powerup appears
            getWorld()->playSound(SOUND_POWERUP_APPEARS);
            //create NEW mushroom object 8 pix above
            
            Actor* mu = new Mushroom(getX(), getY()+SPRITE_HEIGHT, getWorld());
            getWorld()->addActor(mu);
            //ADD MUSHROOM to StudentWorld's list of actors
            //change goodieRelease to n
            goodieRelease = 'n';
            break;
        }


    }
    
    
}


void LevelOver::doSomething()
{
    
    if (isDead())
        return;
    
    if (getWorld()->overlapPeach(this))
    {
        getWorld()->increaseScore(1000);
        setDead();
    }
}

void Flag::doSomething()
{
    LevelOver::doSomething();
    if (isDead())
    {
        getWorld()->levelComp();
    }
}

void Mario::doSomething()
{
    LevelOver::doSomething();
    if (isDead())
    {
        getWorld()->gameComp();
    }
}


void Goodie::doSomething()
{
    int cx = getX();
    int cy = getY();
    

    if (getWorld()->overlapPeach(this))
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    
    
    
    if (canFall())
        moveTo(cx, cy-2);
        
    
    if (getDirection() == 0)
    {
        if (canMoveRight(2))
        {
            moveTo(getX()+2, getY());
        }
        else
        {
            setDirection(180);
            return;
        }
    }
    else if (getDirection() == 180)
    {
        if (canMoveLeft(2))
            moveTo(getX()-2, getY());
        else
        {
            setDirection(0);
            return;
        }
    }
    
    
}

void Flower::doSomething()
{
    if (getWorld()->overlapPeach(this))
    {
        getWorld()->setPHit(2);
        
        getWorld()->increaseScore(50);

        getWorld()->addPpowerUp('f');
        
    }
    Goodie::doSomething();
}

void Star::doSomething()
{
    if (getWorld()->overlapPeach(this))
    {
        getWorld()->increaseScore(100);
        getWorld()->addPpowerUp('s');

    }
    Goodie::doSomething();
}

void Mushroom::doSomething()
{
    if (getWorld()->overlapPeach(this))
    {
        getWorld()->setPHit(2);
        
        getWorld()->increaseScore(75);

        getWorld()->addPpowerUp('m');

    }
    Goodie::doSomething();
}


void Fireball::doSomething()
{
    //implement fireball's doSomething method here
    //peach fireball is slightly different than pirahna fireball
    
    if (canFall())
        moveTo(getX(), getY()-2);
    
    if (getDirection() == 180)
    {
        if (canMoveLeft(2))
            moveTo(getX()-2, getY());
        else
        {
            setDead();
            return;
        }
        
    }
    else if (getDirection() == 0)
    {
        if (canMoveRight(2))
            moveTo(getX()+2, getY());
        else
        {
            setDead();
            return;
        }
        
    }
    
}

void PeachFireball::doSomething()
{

    if (!getWorld()->overlapPeach(this))
    {
        
        if (getWorld()->bdIfOverlap(this, 'd'))
        {
            setDead();
            return;
        }

    }
    Fireball::doSomething();
}

void Shell::doSomething()
{
    PeachFireball::doSomething();
}


void PirFireball::doSomething()
{
    if (getWorld()->overlapPeach(this))
    {
        getWorld()->damagePeach();
        setDead();
        return;
    }
    
    Fireball::doSomething();
}





void Enemy::doSomething()
{
    if (isDead())
        return;
    
    //takes care of random direction given when initialized
    if (getDirection() != 0 && getDirection()<90)
        setDirection(0);
    else if (getDirection() != 180 && getDirection()>=90)
        setDirection(180);
    
    
    
    if (getWorld()->overlapPeach(this))
    {
        if (getWorld()->isInv())
        {
            bonk();
        }
        
        getWorld()->bonkPeach(this);
        return;
    }


    
    if (getDirection() == 0)
    {
        
        if (canMoveRight(1))
        {
            //enemy would fall if it goes right
            if (getWorld()->enemyFall(getX()+SPRITE_WIDTH, getY(), this))
            {
                setDirection(180);
            }
            else
                moveTo(getX()+1, getY());
        }
        else
            setDirection(180);

    }
    else
    {
        if (canMoveLeft(1))
        {

            if (getWorld()->enemyFall(getX()-1, getY(), this))
            {
                setDirection(0);
            }
            else
                moveTo(getX()-1, getY());
        }
        else
            setDirection(0);
    }
    

}


void Enemy::bonk()
{
    if (! getWorld()->overlapPeach(this))
    {
        return;
    }
    else if (getWorld()->isInv())
    {
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setDead();
    }
    
}

void Koopa::bonk()
{
    Enemy::bonk();
    
    if (isDead())
    {
        Actor* sh = new Shell(getX(), getY(), getDirection(), getWorld());
        getWorld()->addActor(sh);
    }

}
void Koopa::damage()
{
    Enemy::damage();
    Actor* sh = new Shell(getX(), getY(), getDirection(), getWorld());
    getWorld()->addActor(sh);
}

void Enemy::damage()
{
    getWorld()->increaseScore(100);
    setDead();

}

void Piranha::doSomething()
{
    if (getDirection() != 0 && getDirection()<90)
        setDirection(0);
    else if (getDirection() != 180 && getDirection()>=90)
        setDirection(180);
    increaseAnimationNumber();
    
    if (getWorld()->overlapPeach(this))
    {
        getWorld()->bonkPeach(this);
        return;
    }
    
    int py = getWorld()->getPeachY();
    int px = getWorld()->getPeachX();
    if (!(py > (getY()-(1.5*SPRITE_HEIGHT)) && py < (getY()+1.5*SPRITE_HEIGHT)))
        return;
    else
    {
        if (px < getX())
            setDirection(180);
        else
            setDirection(0);
        
        if (firing_delay >0)
        {
            firing_delay--;
            return;
        }
        else
        {
            if (px >=getX()-(8*SPRITE_WIDTH) && px<= getX()+(8*SPRITE_WIDTH))
            {
                Actor* pf = new PirFireball(getX(), getY(), getDirection(), getWorld());
                getWorld()->addActor(pf);
                getWorld()->playSound(SOUND_PIRANHA_FIRE);
                firing_delay = 40;
            }
        }
    }
    
}



