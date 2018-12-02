#include "pile.h"

    float range;
    float coneAngle;
    float lifespan;
    int damage;

Pile::Pile(QVector2D pos):Interactable2D (pos),range(1),coneAngle(30),lifespan(60),damage(1){

}

Pile::Pile(QVector2D pos,float r, float c, float l, int d):Interactable2D (pos),range(r),coneAngle(c),lifespan(l),damage(d){

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
