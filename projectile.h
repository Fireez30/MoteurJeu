#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "interactable2d.h"

class Projectile : public Interactable2D{

protected:
    int lifespan,damage;
    float speed, initspeed;
    bool alive;
    QVector2D direction;
    QBasicTimer life;
    void timerEvent(QTimerEvent *e) override;

public :
    Projectile();
    Projectile(QVector2D pos,QVector2D text,int collidetime,int lifetime,int dmg,float sp,QVector2D dir);
    int Update();
    int OnTriggerEnter(Interactable2D* other) override;
    void changeSpeed(float factor);
};

#endif // PROJECTILE_H
