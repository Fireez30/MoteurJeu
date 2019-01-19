#ifndef TORCHE_H
#define TORCHE_H

#include "interactable2d.h"
#include "movable.h"
#include "room.h"
#include "lightsource.h"

class Torche : public Interactable2D{

protected:
    float range;
    int damage;
    Room* room;
    LightSource ls;
public :
    Torche(QVector2D pos,QVector2D text);
    Torche(QVector2D pos,float r, int d,QVector2D text, Room* ro);
    int OnTriggerEnter(Interactable2D* other) override;
    LightSource* getLightSource();
    int GetDamage();
    float GetRange();
};

#endif // Torche
