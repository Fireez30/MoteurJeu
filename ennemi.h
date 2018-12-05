#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"

class Ennemi : public Movable{

public :
    Ennemi(float x, float y, float s,QVector2D pos,QVector2D text);
    void IA();
};

#endif // ENNEMI_H
