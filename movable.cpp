#include "movable.h"

Movable::Movable():Interactable2D(QVector2D(0,0),QVector2D(0,0),1000),speed(1),direction(0,0),initPos(0,0),health(3),dead(false){

}

Movable::Movable(int h,float x, float y,float sp,QVector2D pos,QVector2D text):Interactable2D(pos,text,1000),speed(sp),direction(x,y),initPos(pos),health(h),dead(false){

}

QVector2D Movable::GetDirection(){
    return direction;
}

float Movable::GetSpeed(){
    return speed;
}

void Movable::ChangeSpeed(float s){
    speed = s;
}

void Movable::ChangeDirection(QVector2D dir){
    direction = dir;
}
void Movable::Move(QVector3D dir){
    position += dir;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
}

void Movable::ResetPos(){
    position = initPos;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
}

int Movable::getHealth(){
    return health;
}

void Movable::setHealth(int h){
    health = h;
}

void Movable::Damage(int d){
    if (health > d){
        health -= d;
    }
    else {
        health = 0;
        dead = true;
    }
}

bool Movable::isDead(){
    return dead;
}
