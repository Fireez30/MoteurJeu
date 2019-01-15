#ifndef BOSS_TORCHE_H
#define BOSS_TORCHE_H

#include "movable.h"
#include "player.h"
#include "room.h"
#include "projectile.h"

class Boss_torche : public Movable{

public :
    Boss_torche(int h,float x, float y, float s,QVector2D pos,QVector2D text);
    Boss_torche(Room* r,Player* p,int h, float x, float y, float s,int cd,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus);
    void IA();
    int OnTriggerEnter(Interactable2D* other) override;
    void Update() override;
    QVector2D GetRoomPos();

private :
    Player* player;
    Room* room;
    QVector2D dir;
    void startTimer();
    QBasicTimer timer;
    QVector2D ennemitoplayer;
};



#endif // BOSS_TORCHE_H
