#include <iostream>

#include "interactable2d.h"

Interactable2D::Interactable2D(QVector2D pos,QVector2D text,float time): Base2D(pos.x(),pos.y(),0,text),timeCollide(time),canCollide(true){

}
void Interactable2D::setCollider(Hitbox h){
    collider = h;
}

Hitbox Interactable2D::getCollider(){
    return collider;
}

void Interactable2D::startTimer(){
    timer.start(timeCollide,this);
    //std::cout << "Timer collision débuté\n";
}

void Interactable2D::timerEvent(QTimerEvent *){
    canCollide = true;
    //std::cout << "Timer collision terminé\n";
    timer.stop();
}
