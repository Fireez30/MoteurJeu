#include "pile.h"
#include "player.h"
#include <iostream>

Pile::Pile(QVector2D pos,QVector2D text):Interactable2D (pos,text,1),range(1),coneAngle(30),lifespan(60),damage(1),color(1,1,1),ls(pos,QVector3D(0,1,0),0.0005f,0.5f,180,180.0f,QVector3D(1,0,0),1,1.2f){

}

Pile::Pile(QVector2D pos,float r, float c, float l, int d,QVector2D text, int id,QVector3D thecolor):Interactable2D (pos,text,3000),range(r),coneAngle(c),lifespan(l),damage(d),idPile(id),color(thecolor),ls(pos,thecolor,0.0005f,0.5f,180,180.0f,QVector3D(1,0,0),1.0f,1.2f){

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
    if(p != nullptr){
        std::cout << "je suis un player " << std::endl;
        p->SetPileSecondaire(this);
        std::cout << "mis la pile secondaire " << std::endl;
        startTimer();
        std::cout << "apres start timer " << std::endl;
        changeLight();
        std::cout << "apres change light " << std::endl;
        canCollide = false;
        return -1;
    }
}

int Pile::GetDamage(){
    return damage;
}

int Pile::getID(){
    return idPile;
}

QVector3D Pile::getColor(){
    return color;
}

void Pile::setLifespan(float life){
    lifespan  = life;
}
float Pile::getLifespan(){
    return lifespan;
}

LightSource* Pile::getLightSource(){
    return &ls;
}

void Pile::changeLight(){
    ls = LightSource(QVector2D(position.x(),position.y()),color, 0.005f,0.5f,coneAngle-5.0f,coneAngle,QVector3D(1,0,0),range-1.0f,range);
}
