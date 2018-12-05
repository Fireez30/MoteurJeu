#include "pile.h"

Pile::Pile(QVector2D pos,QVector2D text):Interactable2D (pos,text),range(1),coneAngle(30),lifespan(60),damage(1){

}

Pile::Pile(QVector2D pos,float r, float c, float l, int d,QVector2D text):Interactable2D (pos,text),range(r),coneAngle(c),lifespan(l),damage(d){

}

float Pile::GetRange(){
    return range;
}

float Pile::GetConeAngle(){
    return coneAngle;
}

float Pile::GetLifepan(){
    return lifespan;
}

int Pile::GetDamage(){
    return damage;
}
