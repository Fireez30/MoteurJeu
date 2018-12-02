#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"

class Ennemi : public Movable{

public :
    Ennemi(float x, float y, float s,QVector2D pos);
    void IA();
};

#endif // ENNEMI_H
