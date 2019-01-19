#ifndef MOVANIMATOR_H
#define MOVANIMATOR_H

class Movable;

#include "movable.h"
#include <QOpenGLWidget>

class MovAnimator : public QOpenGLWidget{

protected :
    Movable* Object;
    QBasicTimer moveAnimTimer;
    int nbFrames,moveAnimTime, movAnimStatus;
    bool walking, started;
    void timerEvent(QTimerEvent *e) override;

public :
    MovAnimator(Movable* obj);
    MovAnimator(Movable* obj,int walkTime,int frames,bool walk);
    void Walk();
    void StopWalk();
    void StartAnimator();
    bool isWalking();
    bool isStarted();
};

#endif // MOVANIMATOR_H
