#include "turretennemy.h"
#include <iostream>

TurretEnnemi::TurretEnnemi(int h,float x, float y, float s,QVector2D pos,QVector2D text): Movable(h,x,y,s,3,pos,text,200,3,false),shootdir(0,0),targetPlayer(false),timerTime(0.5),projectileSpeed(1),projectileTravelTime(1){
    canShoot = true;
    startTimer();
    initTime = timerTime;
}

TurretEnnemi::TurretEnnemi(Room* r,Player* p,int h, float x, float y, float s,int cd,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus,bool targplayer,float cooldown,float projspeed,int projtime): Movable(h,x,y,s,cd,pos,text,animtime,nbframes,animstatus),shootdir(x,y),targetPlayer(targplayer),timerTime(cooldown),projectileSpeed(projspeed),projectileTravelTime(projtime){
    this->player = p;
    this->room = r;
    canShoot = true;
    startTimer();
    initTime = timerTime;
}

void TurretEnnemi::startTimer(){
    shoottimer.start(timerTime*1000,this);
}

void TurretEnnemi::timerEvent(QTimerEvent *){
    canShoot = true;
    shoottimer.stop();
}

void TurretEnnemi::IA(){
    //std::cout << "taille projectiles " << projectiles.size() << std::endl;
    if (targetPlayer){
        //std::cout << "je tire vers l'ennemi peut etre" << std::endl;
        shootdir = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
        shootdir.normalize();
    }
    if (canShoot){
        projectiles.push_back(new Projectile(QVector2D(position.x(),position.y()),QVector2D(4/16.0,13/16.0),0,projectileTravelTime,1,projectileSpeed,QVector2D(shootdir.x(),shootdir.y())));
        canShoot = false;
        shoottimer.start(timerTime*1000,this);
    }
}

void TurretEnnemi::Update(){
    IA();
    if (affected)
    {
        timerTime = initTime * 5;
    }
    else {
        timerTime = initTime;
    }

    for (unsigned i = 0; i < projectiles.size(); i++){
        if (projectiles[i]->Update() == -1){
            Projectile* truc = projectiles[i];
            projectiles.erase(projectiles.begin()+i);
            delete truc;
        }
    }
}

int TurretEnnemi::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        player->ResetMove();
    }

    return 1;
}
