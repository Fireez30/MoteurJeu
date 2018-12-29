#ifndef MOVANIMATOR_H
#define MOVANIMATOR_H

class Movable;

#include "movable.h"
#include <QOpenGLWidget>

class MovAnimator : public QOpenGLWidget{

protected :
    Movable* Object;
    int moveAnimTime;
    QBasicTimer moveAnimTimer;
    int nbFrames;
    bool walking;
    void timerEvent(QTimerEvent *e) override;
    int movAnimStatus;
    bool started;

public :
    MovAnimator(Movable* obj);
    MovAnimator(Movable* obj,int walkTime,int frames,bool walk);
    void Walk();
    void StopWalk();
    bool isWalking();
    bool isStarted();
    void StartAnimator();
};

#endif // MOVANIMATOR_H
