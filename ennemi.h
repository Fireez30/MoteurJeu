#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"
#include "player.h"

class Ennemi : public Movable{

public :
    Ennemi(float x, float y, float s,QVector2D pos,QVector2D text);
    Ennemi(Player* p, float x, float y, float s,QVector2D pos,QVector2D text);
    void IA();
    void OnTriggerEnter(Interactable2D* other) override;


private :
    Player* player;
    QVector2D dir;

};



#endif // ENNEMI_H
