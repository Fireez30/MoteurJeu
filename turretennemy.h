#ifndef TURRETENNEMY_H
#define TURRETENNEMY_H

#include "ennemi.h"
#include "player.h"
#include "room.h"
#include "projectile.h"

class TurretEnnemi : public Ennemi{

public :
    TurretEnnemi(int h,float x, float y, float s,QVector2D pos,QVector2D text);
    TurretEnnemi(Room* r,Player* p,int h, float x, float y, float s,int cd,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus,bool targetplayer,float cooldown,float projspeed,int projtime);
    void IA() override;
    int OnTriggerEnter(Interactable2D* other) override;
    void timerEvent(QTimerEvent *e) override;
    void Update() override;

private :
    Player* player;
    Room* room;
    QVector2D dir;
    void startTimer();
    QBasicTimer shoottimer;
    QVector2D shootdir;
    float timerTime;
    float initTime;
    bool targetPlayer;
    float projectileSpeed;
    int projectileTravelTime;
    bool canShoot;
};

#endif // TURRETENNEMY_H
