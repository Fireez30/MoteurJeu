#ifndef BOSS_TORCHE_H
#define BOSS_TORCHE_H

#include "ennemi.h"
#include "player.h"
#include "room.h"
#include "projectile.h"

class Boss_torche : public Ennemi{

public :
    Boss_torche(int h,float x, float y, float s,QVector2D pos,QVector2D text);
    Boss_torche(Room* r,Player* p,QVector2D pos);
    void IA();
    int OnTriggerEnter(Interactable2D* other) override;
    void Update() override;
    QVector2D GetRoomPos();

private :
    Player* player;
    Room* room;
    QVector2D dir;
};



#endif // BOSS_TORCHE_H
