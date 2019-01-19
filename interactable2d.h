#ifndef INTERACTABLE2D_H
#define INTERACTABLE2D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QElapsedTimer>

#include "hitbox.h"
#include "base2d.h"

class Interactable2D : public Base2D , public QOpenGLWidget {

protected:
    Hitbox collider;
    void timerEvent(QTimerEvent *e) override;

public :
    bool canCollide;
    int timeCollide;
    QBasicTimer timer;

    Interactable2D(const Interactable2D&) = delete;
    Interactable2D(QVector2D pos,QVector2D text,int time);
    virtual int OnTriggerEnter(Interactable2D* other) = 0;//abstract
    void startTimer();
    bool isTimerActive();
    Hitbox getCollider();
    void setCollider(Hitbox h);

};

#endif // INTERACTABLE2D_H
