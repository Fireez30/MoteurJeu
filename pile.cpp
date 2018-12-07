#include "pile.h"
#include "player.h"
#include <iostream>

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

int Pile::OnTriggerEnter(Interactable2D* other){
    //si memory leak check here
    std::cout << "CONTACT" << std::endl;
    Player* p;
    p = dynamic_cast<Player*> (other);
    if(p != NULL){
        p->SetPileSecondaire(this);
    }

    return -1;
}

int Pile::GetDamage(){
    return damage;
}
