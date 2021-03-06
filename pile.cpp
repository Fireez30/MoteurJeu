#include "pile.h"
#include "player.h"
#include <iostream>

Pile::Pile(QVector2D pos,QVector2D text):Interactable2D (pos,text,1),range(1),coneAngle(30),lifespan(60*60),damage(1),color(1,1,1),ls(pos,QVector3D(0,1,0),0.0005f,0.5f,180,180.0f,QVector3D(1,0,0),1,1.2f),maxLife(lifespan){

}

Pile::Pile(QVector2D pos,float r, float c, int l, int maxL, int d,QVector2D text, int id,QVector3D thecolor):Interactable2D (pos,text,3),range(r),coneAngle(c),lifespan(l),damage(d),idPile(id),color(thecolor),ls(pos,thecolor,0.0005f,0.5f,180,180.0f,QVector3D(1,0,0),1.0f,1.2f),maxLife(maxL){
}

float Pile::GetRange(){
    return range;
}

float Pile::GetConeAngle(){
    return coneAngle;
}

//update du timer de vie de la pile
bool Pile::Update(){
    if (lifespan > 0){
        lifespan--;
        return true;
    }
    return false;
}

//on pickup par le joueur
int Pile::OnTriggerEnter(Interactable2D* other){
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

//pour affichage
QVector3D Pile::getColor(){
    return color;
}

void Pile::setLifespan(int life){
    lifespan  = life;
}
int Pile::getLifespan(){
    return lifespan;
}

//retourne la light pour les shaders
LightSource* Pile::getLightSource(){
    return &ls;
}

//met a jour la lightsource pour s'adapter a la lampe (cone)
void Pile::changeLight(){
    ls = LightSource(QVector2D(position.x(),position.y()),color, 0.005f,0.5f,coneAngle-5.0f,coneAngle,QVector3D(1,0,0),range-1.0f,range);
}

//ratio pour affichage du timer sur l'UI
float Pile::getRatioLife(){
    return (float)lifespan/(float)maxLife;
}

int Pile::getMaxLifespan(){
    return maxLife;
}
