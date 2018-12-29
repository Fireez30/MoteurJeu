#ifndef MOVABLE_H
#define MOVABLE_H

#include "interactable2d.h"

class Movable: public Interactable2D
{
protected :
    float speed;
    float initSpeed;
    QVector2D direction;
    QVector2D initPos;
    int health;
    bool dead;
    int animTime;
    int nbFrames;
    bool walking;
    void timerEvent(QTimerEvent *e) override;

public :
    QBasicTimer animTimer;
    Movable();
    Movable(int health,float x, float y,float sp,QVector2D pos,QVector2D text);
    QVector2D GetDirection();
    float GetSpeed();
    void ChangeSpeed(float s);
    void ChangeDirection(QVector2D dir);
    void Move(QVector3D dir);
    void ResetPos();
    virtual void Update() = 0;
    virtual QVector3D GetLastMove() = 0;
    int getHealth();
    void setHealth(int h);
    void Damage(int d);
    bool isDead();
    void Walk();
    void StopWalk();
    bool isWalking();
};

#endif // MOVABLE_H
