#include "ennemi.h"

Ennemi::Ennemi(float x, float y, float s,QVector2D pos,QVector2D text): Movable(x,y,s,pos,text){

}

Ennemi::Ennemi(Player* p, float x, float y, float s,QVector2D pos,QVector2D text): Movable(x,y,s,pos,text){
    this->player = p;
}


void Ennemi::IA(){

}

void Ennemi::OnTriggerEnter(Interactable2D* other){
    if(dynamic_cast<Player*> (other)!=NULL){
        //player->TakeDamage();
            printf("coucou");
        QVector3D pos_ennemi = this->position;
        QVector3D pos_player = other->position;
        QVector3D ennemi_to_player = QVector3D(pos_player.x() - pos_ennemi.x(), pos_player.y() - pos_ennemi.y(), 0);
        ennemi_to_player.normalize();
        QVector3D dirJoueur = QVector3D(ennemi_to_player.x()*3,ennemi_to_player.y()*3,0);
        player->Move(dirJoueur);

    }
}
