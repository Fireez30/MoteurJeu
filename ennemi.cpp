#include "ennemi.h"
#include <iostream>

Ennemi::Ennemi(float x, float y, float s,QVector2D pos,QVector2D text): Movable(x,y,s,pos,text){

}

Ennemi::Ennemi(Player* p, float x, float y, float s,QVector2D pos,QVector2D text): Movable(x,y,s,pos,text){
    this->player = p;
}


void Ennemi::IA(){
    QVector3D pos_ennemi = this->position;
    QVector3D pos_player = player->position;
    QVector3D ennemi_to_player = QVector3D(pos_player.x() - pos_ennemi.x(), pos_player.y() - pos_ennemi.y(), 0);
    ennemi_to_player.normalize();
    ennemi_to_player *= speed;
    this->Move(ennemi_to_player);
}

void Ennemi::Update(){
    IA();
}

int Ennemi::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        //player->TakeDamage();
        QVector3D pos_ennemi = this->position;
        QVector3D pos_player = other->position;
        QVector3D ennemi_to_player = QVector3D(pos_player.x() - pos_ennemi.x(), pos_player.y() - pos_ennemi.y(), 0);
        ennemi_to_player.normalize();
        QVector3D dirJoueur = QVector3D(ennemi_to_player.x()*0.8, ennemi_to_player.y()*0.8 ,0);
        player->Move(dirJoueur);
        return 0;
    }
    else return 1;
}
