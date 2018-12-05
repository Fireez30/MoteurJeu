#ifndef MOVABLE_H
#define MOVABLE_H

#include "interactable2d.h"

class Movable: public Interactable2D
{
protected :
    float speed;
    QVector2D direction;

public :
    Movable();
    Movable(float x, float y,float sp,QVector2D pos,QVector2D text);
    QVector2D GetDirection();
    float GetSpeed();
    void ChangeSpeed(float s);
    void ChangeDirection(QVector2D dir);
    void Move(QVector3D dir);
};

#endif // MOVABLE_H
