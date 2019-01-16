#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"

class Ennemi : public Movable {

public :
    Ennemi(const Ennemi&) = delete;
    Ennemi(QVector2D pos, QVector2D dir, QVector2D text, int life, int v, int cdTrigger, int animTime, int nbFrame, bool animStatus);
    virtual void IA() = 0;

};

#endif // ENNEMI_H
