#ifndef INTERACTABLE2D_H
#define INTERACTABLE2D_H

#include "hitbox.h"
#include "base2d.h"

class Interactable2D : public Base2D {

protected:
    Hitbox collider;

public :
    Interactable2D(const Interactable2D&) = delete;
    Interactable2D(QVector2D pos,QVector2D text);
    void setCollider(Hitbox h);
    Hitbox getCollider();
    virtual int OnTriggerEnter(Interactable2D* other) = 0;//abstract
};

#endif // INTERACTABLE2D_H
