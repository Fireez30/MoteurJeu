#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "interactable2d.h"

class Projectile : public Interactable2D{

protected:
    int lifespan;
    int damage;
    QVector2D direction;
    float speed;
    float initspeed;
    QBasicTimer life;
    void timerEvent(QTimerEvent *e) override;
    bool alive;

public :
    Projectile();
    Projectile(QVector2D pos,QVector2D text,int collidetime,int lifetime,int dmg,float sp,QVector2D dir);
    int Update();
    void changeSpeed(float factor);
    int OnTriggerEnter(Interactable2D* other) override;
};

#endif // PROJECTILE_H
