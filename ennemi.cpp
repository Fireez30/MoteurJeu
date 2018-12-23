#include "ennemi.h"
#include <iostream>

Ennemi::Ennemi(float x, float y, float s,QVector2D pos,QVector2D text): Movable(x,y,s,pos,text),ennemitoplayer(0,0,0){
}

Ennemi::Ennemi(Player* p, float x, float y, float s,QVector2D pos,QVector2D text): Movable(x,y,s,pos,text),ennemitoplayer(0,0,0){
    this->player = p;
    //startTimer();
}

void Ennemi::startTimer(){
    timer.start(1000,this);
}

void Ennemi::timerEvent(QTimerEvent *){
    std::cout << "timer ia " << std::endl;
    ennemitoplayer = QVector3D(player->position.x() - position.x(), player->position.y() - position.y(), 0);
    ennemitoplayer.normalize();
    ennemitoplayer *= speed;
    timer.stop();
    timer.start(1000,this);
}

void Ennemi::IA(){
     ennemitoplayer = QVector3D(player->position.x() - position.x(), player->position.y() - position.y(), 0);
     ennemitoplayer.normalize();
     ennemitoplayer *= speed;
     this->Move(ennemitoplayer * 0.0166);//collision checvk a faire
}

QVector3D Ennemi::GetLastMove(){
    return ennemitoplayer;
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
        player->Move(dirJoueur);//collision checvk a faire
        return 0;
    }
    else return 1;
}
