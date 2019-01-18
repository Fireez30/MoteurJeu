
#include "movanimator.h"
#include <iostream>

MovAnimator::MovAnimator(Movable* obj):Object(obj),moveAnimTime(200),nbFrames(3),walking(false){
    movAnimStatus = 0;
    moveAnimTimer.start(moveAnimTime,this);
}

MovAnimator::MovAnimator(Movable* obj,int walkTime,int frames,bool walk) : Object(obj),moveAnimTime(walkTime),nbFrames(frames),walking(walk){
    movAnimStatus = 0;
    moveAnimTimer.start(moveAnimTime,this);
}

//lance l'animation de marche si pas en cours
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

//changement frames de l'animation
void MovAnimator::timerEvent(QTimerEvent *){
    Object->renderer.spriteCoords = Object->renderer.initText;
    moveAnimTimer.stop();

    if (walking){//si en train de marcher, changer frame sur la spritesheet et restart timer
        movAnimStatus++;
        movAnimStatus = movAnimStatus %nbFrames;
        Object->renderer.spriteCoords.setX(Object->renderer.spriteCoords.x()+(movAnimStatus/16.0));
        Object->renderer.CreateGeometry();
        moveAnimTimer.start(moveAnimTime,this);
    }
}
