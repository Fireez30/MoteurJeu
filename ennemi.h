#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"
#include "player.h"

class Ennemi : public Movable{

public :
    Ennemi(float x, float y, float s,QVector2D pos,QVector2D text);
    Ennemi(Player* p, float x, float y, float s,QVector2D pos,QVector2D text);
    void IA();
    int OnTriggerEnter(Interactable2D* other) override;
    void Update() override;

private :
    Player* player;
    QVector2D dir;

};



#endif // ENNEMI_H
