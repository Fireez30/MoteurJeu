#include "torche.h"
#include "boss_torche.h"
#include <iostream>

Torche::Torche(QVector2D pos,QVector2D text):Interactable2D (pos,text,1),range(4),damage(1){

}

Torche::Torche(QVector2D pos,float r, int d, QVector2D text, Room* ro):Interactable2D (pos,text,3000),range(r),damage(d){
    this->room = ro;
}

float Torche::GetRange(){
    return range;
}

int Torche::OnTriggerEnter(Interactable2D* other){
    Boss_torche* bt;
    bt = dynamic_cast<Boss_torche*> (other);
    if(bt != nullptr){
        startTimer();
        canCollide = false;
        bt->Damage(this->GetDamage());
        /*QVector3D pos_ennemi = this->position;
        QVector3D pos_player = other->position;
        QVector3D ennemi_to_player = QVector3D(pos_player.x() - pos_ennemi.x(), pos_player.y() - pos_ennemi.y(), 0);
        ennemi_to_player.normalize();
        QVector2D dirJoueur = QVector2D(ennemi_to_player.x()*0.8, ennemi_to_player.y()*0.8);
        bt->Move(dirJoueur);//collision checvk a faire*/
        if (bt->position.x() <= 161) //a gauche
        {
            bt->position = QVector3D(170, 98, 0);
        }
        else // à droite
        {
            bt->position = QVector3D(152, 98, 0);
        }

        if (room->CollisionCheck(bt->getCollider())){//si la collision amene le joueur dans le mur, la reset
            bt->ResetMove();
        }
        return 0;
    }
    return 1;
}

int Torche::GetDamage(){
    return damage;
}
