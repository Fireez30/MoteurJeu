#ifndef INTERACTABLE2D_H
#define INTERACTABLE2D_H

#include <QOpenGLWidget>

#include "hitbox.h"
#include "base2d.h"

class Interactable2D : public Base2D,public QOpenGLWidget {

protected:
    Hitbox collider;
    void timerEvent(QTimerEvent *e) override;

public :
    Interactable2D(const Interactable2D&) = delete;
    Interactable2D(QVector2D pos,QVector2D text,int time);
    void setCollider(Hitbox h);
    Hitbox getCollider();
    virtual int OnTriggerEnter(Interactable2D* other) = 0;//abstract
    void startTimer();
    QBasicTimer timer;
    bool canCollide;
    int timeCollide;

};

#endif // INTERACTABLE2D_H
