#ifndef PILE_H
#define PILE_H

#include "interactable2d.h"
#include "ennemi.h"

class Pile : Interactable2D{

protected:
    float range;
    float coneAngle;
    float lifespan;
    int damage;

public :
    Pile(QVector2D pos,QVector2D text);
    Pile(QVector2D pos,float r, float c, float l, int d,QVector2D text);
    virtual void Affect() = 0;
    float GetRange();
    float GetConeAngle();
    float GetLifepan();
    int GetDamage();
};

#endif // PILE_H
