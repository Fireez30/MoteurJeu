#ifndef ENNEMI_H
#define ENNEMI_H

#include "movable.h"
#include "player.h"
#include "room.h"

class Ennemi : public Movable{

public :
    Ennemi(float x, float y, float s,QVector2D pos,QVector2D text);
    Ennemi(Room* r,Player* p, float x, float y, float s,QVector2D pos,QVector2D text);
    void IA();
    int OnTriggerEnter(Interactable2D* other) override;
    void timerEvent(QTimerEvent *e) override;
    void Update() override;
    QVector3D GetLastMove() override;

private :
    Player* player;
    Room* room;
    QVector2D dir;
    void startTimer();
    QBasicTimer timer;
    QVector3D ennemitoplayer;
};



#endif // ENNEMI_H
