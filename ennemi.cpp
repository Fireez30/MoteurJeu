#include "ennemi.h"
#include <iostream>

Ennemi::Ennemi(int h,float x, float y, float s,QVector2D pos,QVector2D text): Movable(h,x,y,s,3,pos,text,200,3,false),ennemitoplayer(0,0){
    movAnim->StartAnimator();
    movAnim->Walk();
}

Ennemi::Ennemi(Room* r,Player* p,int h, float x, float y, float s,int cd,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus): Movable(h,x,y,s,cd,pos,text,animtime,nbframes,animstatus),ennemitoplayer(0,0){
    this->player = p;
    this->room = r;
    //startTimer();
    movAnim->StartAnimator();
    movAnim->Walk();
}

void Ennemi::startTimer(){
    timer.start(1000,this);

}


void Ennemi::IA(){
    ennemitoplayer = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
    ennemitoplayer.normalize();
    ennemitoplayer *= speed;
    this->Move(ennemitoplayer);//collision checvk a faire
    if (player->getHoldKey()){
        if (projectiles.size() == 0){
            projectiles.push_back(new Projectile(QVector2D(position.x(),position.y()),QVector2D(4/16.0,13/16.0),0,1,1,1,QVector2D(ennemitoplayer.x(),ennemitoplayer.y())));
        }
    }
}

void Ennemi::Update(){
    IA();
    speed = initSpeed;

    if (affected)
    {
        for (unsigned i = 0; i < projectiles.size(); i++){
            projectiles[i]->changeSpeed(0.5);
        }
    }
    for (unsigned i = 0; i < projectiles.size(); i++){
        if (projectiles[i]->Update() == -1){
            Projectile* truc = projectiles[i];
            projectiles.erase(projectiles.begin()+i);
            delete truc;
        }
    }
}

int Ennemi::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        player->Damage(1);
        QVector3D pos_ennemi = this->position;
        QVector3D pos_player = other->position;
        QVector3D ennemi_to_player = QVector3D(pos_player.x() - pos_ennemi.x(), pos_player.y() - pos_ennemi.y(), 0);
        ennemi_to_player.normalize();
        QVector2D dirJoueur = QVector2D(ennemi_to_player.x()*0.8, ennemi_to_player.y()*0.8);
        player->Move(dirJoueur);//collision checvk a faire
        if (room->CollisionCheck(player->getCollider())){//si la collision amene le joueur dans le mur, la reset
            player->ResetMove();
        }
        return 0;
    }

    return 1;
}
