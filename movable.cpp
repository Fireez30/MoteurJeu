#include "movable.h"

Movable::Movable():speed(1),direction(0,0){

}

Movable::Movable(float x, float y,float sp):speed(sp),direction(x,y){

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
