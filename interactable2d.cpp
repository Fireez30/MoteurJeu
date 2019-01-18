#include <iostream>

#include "interactable2d.h"

Interactable2D::Interactable2D(QVector2D pos,QVector2D text,int time): Base2D(pos.x(),pos.y(),0,text),timeCollide(time*1000),canCollide(true){

}
void Interactable2D::setCollider(Hitbox h){
    collider = h;
}

Hitbox Interactable2D::getCollider(){
    return collider;
}

//timer de frames d'invulnérabilité (empechant les collisions)
void Interactable2D::startTimer(){
    timer.start(timeCollide,this);
    //std::cout << "Timer collision débute ! temps = " << timeCollide << std::endl;
}

bool Interactable2D::isTimerActive(){
    return timer.isActive();
}

//fin du timer, on peut collide a nouveau
void Interactable2D::timerEvent(QTimerEvent *){
    canCollide = true;
    //std::cout << "Timer collision termine" << std::endl;
    timer.stop();
}
