#ifndef PLAYER_H
#define PLAYER_H

#include "baseobject.h"
#include "movable.h"
#include "pile.h"

class Player : public Movable
{
protected:
    std::vector<QVector2D> sprites;
    int h, w;
public :
    Player();
    Player(float x,float y, float sp,QVector2D dir);
    void Input();
    void OnTriggerEnter(Hitbox e) override;
    void ChangePile(Pile *p);
    void ChangeOrientation(int x, int y,QMatrix4x4 m);
};

#endif // PLAYER_H
