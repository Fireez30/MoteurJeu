#ifndef PLAYER_H
#define PLAYER_H

#include "baseobject.h"
#include "movable.h"

class Player : public Movable
{
protected:

public :
    Player();
    Player(float x,float y, float sp,QVector2D dir);
    void Input();
    void OnTriggerEnter(Hitbox e) override;
};

#endif // PLAYER_H
