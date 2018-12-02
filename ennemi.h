#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"

class Ennemi : public Movable{

public :
    Ennemi(float x, float y, float s,QVector2D pos);
    virtual void IA() = 0;
};

#endif // ENNEMI_H
