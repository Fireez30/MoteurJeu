#ifndef PILE_H
#define PILE_H

#include "interactable2d.h"
#include "movable.h"

class Pile : public Interactable2D{

protected:
    float range;
    float coneAngle;
    float lifespan;
    int damage;
    int idPile;

public :
    Pile(QVector2D pos,QVector2D text);
    Pile(QVector2D pos,float r, float c, float l, int d,QVector2D text, int id);
    virtual void Affect(Movable* m) = 0;
    float GetRange();
    int OnTriggerEnter(Interactable2D* other) override;
    float GetConeAngle();
    float GetLifepan();
    int GetDamage();
    int getID();
    void setLifespan(float life);
    float getLifespan();
};

#endif // PILE_H
