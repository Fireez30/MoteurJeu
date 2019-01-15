#include "pile.h"
#include "player.h"
#include <iostream>

Pile::Pile(Player* p,QVector2D pos,QVector2D text):Interactable2D (pos,text,1),range(1),coneAngle(30),lifespan(60*60),damage(1),color(1,1,1),ls(pos,QVector3D(0,1,0),0.0005f,0.5f,180,180.0f,QVector3D(1,0,0),1,1.2f),player(p){

}

Pile::Pile(Player* p,QVector2D pos,float r, float c, int l, int d,QVector2D text, int id,QVector3D thecolor):Interactable2D (pos,text,3),range(r),coneAngle(c),lifespan(l*60),damage(d),idPile(id),color(thecolor),ls(pos,thecolor,0.0005f,0.5f,180,180.0f,QVector3D(1,0,0),1.0f,1.2f),player(p){
}

float Pile::GetRange(){
    return range;
}

float Pile::GetConeAngle(){
    return coneAngle;
}

bool Pile::Update(){
    if (lifespan > 0){
        lifespan--;
        return true;
    }
    return false;
}

int Pile::OnTriggerEnter(Interactable2D* other){
    //si memory leak check here
    Player* p;
    p = dynamic_cast<Player*> (other);
    if(p != nullptr){
        p->SetPileSecondaire(this);
        canCollide = false;
        return -1;
    }
    return 0;
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

void Pile::setLifespan(int life){
    lifespan  = life;
}
int Pile::getLifespan(){
    return lifespan;
}

LightSource* Pile::getLightSource(){
    return &ls;
}

void Pile::changeLight(){
    ls = LightSource(QVector2D(position.x(),position.y()),color, 0.005f,0.5f,coneAngle-5.0f,coneAngle,QVector3D(1,0,0),range-1.0f,range);
}
