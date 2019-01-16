#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"

class Ennemi : public Movable {

public :
    Ennemi(const Ennemi&) = delete;
    Ennemi();
    virtual void IA() = 0;

};

#endif // ENNEMI_H
