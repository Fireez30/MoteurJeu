#include "movanimator.h"


MovAnimator::MovAnimator(Movable* obj):Object(obj),moveAnimTime(200),nbFrames(3),walking(false){
    movAnimStatus = 0;
    moveAnimTimer.start(moveAnimTime,this);
    started = false;
}

MovAnimator::MovAnimator(Movable* obj,int walkTime,int frames,bool walk) : Object(obj),moveAnimTime(walkTime),nbFrames(frames),walking(walk){
    movAnimStatus = 0;
    moveAnimTimer.start(moveAnimTime,this);
    started = false;
}

void MovAnimator::Walk(){
    walking = true;
    if (!moveAnimTimer.isActive()){
        moveAnimTimer.start(moveAnimTime,this);
    }

}

void MovAnimator::StopWalk(){
    walking = false;
}

bool MovAnimator::isWalking(){
    return walking;
}

bool MovAnimator::isStarted(){
    return started;
}

void MovAnimator::StartAnimator(){
    started = true;
}

void MovAnimator::timerEvent(QTimerEvent *){
    Object->renderer.spriteCoords = Object->renderer.initText;
    moveAnimTimer.stop();

    if (walking){//if still working, change sprite and restart
        movAnimStatus++;
        movAnimStatus = movAnimStatus %nbFrames;
        Object->renderer.spriteCoords.setX((Object->renderer.spriteCoords.x() + movAnimStatus)/16.0);
        Object->renderer.CreateGeometry();
        moveAnimTimer.start(moveAnimTime,this);

    }
}
