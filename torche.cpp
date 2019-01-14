#include "torche.h"
#include "boss_torche.h"
#include <iostream>
#include <time.h>

Torche::Torche(QVector2D pos,QVector2D text):Interactable2D (pos,text,1),range(4),damage(1){

}

Torche::Torche(QVector2D pos,float r, int d, QVector2D text, Room* ro):Interactable2D (pos,text,3000),range(r),damage(d){
    this->room = ro;
}

float Torche::GetRange(){
    return range;
}

int Torche::OnTriggerEnter(Interactable2D* other){
    srand(time(NULL));
    Boss_torche* bt;
    bt = dynamic_cast<Boss_torche*> (other);
    if(bt != nullptr){
        //startTimer();
        //canCollide = false;
        bt->Damage(this->GetDamage());
        /*
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
        */
        QVector2D roomPos = bt->GetRoomPos();
        int nx = rand() % 22 + 1;// 1 -> 23
        int ny = rand() % 12 + 1;// 1 -> 13
        bt->SetPosition(QVector2D(roomPos.x()*25 + nx,roomPos.y()*15 - ny));
        while (room->CollisionCheck(bt->getCollider())){
            nx = rand() % 22 + 1;// 1 -> 23
            ny = rand() % 12 + 1;// 1 -> 13
            bt->SetPosition(QVector2D(roomPos.x()*25 + nx,roomPos.y()*15 - ny));
        }
        // bt->ResetMove();//pas super correct
        return 0;
    }
    return 1;
}

int Torche::GetDamage(){
    return damage;
}
