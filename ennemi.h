#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"
#include "player.h"
#include "room.h"
#include "projectile.h"

class Ennemi : public Movable{

public :
    Ennemi(int h,float x, float y, float s,QVector2D pos,QVector2D text);
    Ennemi(Room* r,Player* p,int h, float x, float y, float s,int cd,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus);
    void IA();
    int OnTriggerEnter(Interactable2D* other) override;
    void timerEvent(QTimerEvent *e) override;
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
