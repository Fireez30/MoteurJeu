#include "boss_torche.h"
#include <iostream>


Boss_torche::Boss_torche(int h,float x, float y, float s,QVector2D pos,QVector2D text): Movable(h,x,y,s,pos,text,200,3,false),ennemitoplayer(0,0){
    movAnim->StartAnimator();
    movAnim->Walk();
}

Boss_torche::Boss_torche(Room* r,Player* p,int h, float x, float y, float s,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus): Movable(h,x,y,s,pos,text,animtime,nbframes,animstatus),ennemitoplayer(0,0){
    this->player = p;
    this->room = r;
    //startTimer();
    movAnim->StartAnimator();
    movAnim->Walk();
}

void Boss_torche::startTimer(){
    timer.start(1000,this);

}

void Boss_torche::timerEvent(QTimerEvent *){
    //std::cout << "timer ia " << std::endl;

    //ennemitoplayer = QVector3D(player->position.x() - position.x(), player->position.y() - position.y(), 0);
    //ennemitoplayer.normalize();
    //ennemitoplayer *= speed;
    //timer.stop();
    //timer.start(1000,this);
}

void Boss_torche::IA(){
     ennemitoplayer = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
     ennemitoplayer.normalize();
     ennemitoplayer *= speed;
     if( !affected ){
        this->Move(ennemitoplayer);//collision check a faire
     }
     else {
        this->Move(-ennemitoplayer);//collision check a faire
        if (room->CollisionCheck(this->getCollider())){//si la collision amene le joueur dans le mur, la reset
            this->ResetMove();
        }
     }
     affected = false;
}


void Boss_torche::Update(){
    IA();
    speed = initSpeed;
    std::cout<<"vie BOSS = "<<getHealth()<<std::endl;
    for (unsigned i = 0; i < projectiles.size(); i++){
        if (projectiles[i]->Update() == -1){
            Projectile* truc = projectiles[i];
            projectiles.erase(projectiles.begin()+i);
            delete truc;
        }
    }
}

int Boss_torche::OnTriggerEnter(Interactable2D* other){
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

