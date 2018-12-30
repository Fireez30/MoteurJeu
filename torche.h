#ifndef TORCHE_H
#define TORCHE_H

#include "interactable2d.h"
#include "movable.h"
#include "room.h"

class Torche : public Interactable2D{

protected:
    float range;
    int damage;
    Room* room;

public :
    Torche(QVector2D pos,QVector2D text);
    Torche(QVector2D pos,float r, int d,QVector2D text, Room* ro);
    float GetRange();
    int OnTriggerEnter(Interactable2D* other) override;
    int GetDamage();
};

#endif // Torche
