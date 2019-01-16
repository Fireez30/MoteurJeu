#ifndef GHOST_H
#define GHOST_H

#include "player.h"
#include "room.h"
#include "projectile.h"
#include "ennemi.h"

class Ghost : public Ennemi{

public :
    Ghost(int h,float x, float y, float s,QVector2D pos,QVector2D text);
    Ghost(Room* r,Player* p,int h, float x, float y, float s,int cd,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus);
    void IA() override;
    int OnTriggerEnter(Interactable2D* other) override;
    void Update() override;

private :
    Player* player;
    Room* room;
    QVector2D dir;
    void startTimer();
    QBasicTimer timer;
    QVector2D ennemitoplayer;
};



#endif // ENNEMI_H
