#include "turretennemy.h"
#include <iostream>

TurretEnnemi::TurretEnnemi(int h,float x, float y, float s,QVector2D pos,QVector2D text): Ennemi(pos,QVector2D(0,0),text,0,0,0,0,0,false),targetPlayer(false),timerTime(0.5),projectileSpeed(1),projectileTravelTime(1){
    canShoot = true;
    startTimer();
    initTime = timerTime;
}

TurretEnnemi::TurretEnnemi(Room* r,Player* p,QVector2D pos,QVector2D dir,bool targplayer): Ennemi(pos, dir,QVector2D(6.0/16.0,8.0/16.0),1000,0,100,200,0,false),targetPlayer(targplayer),timerTime(2),projectileSpeed(1),projectileTravelTime(2){
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
        direction = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
        direction.normalize();
    }
    if (canShoot){
        projectiles.push_back(new Projectile(QVector2D(position.x(),position.y()),QVector2D(4/16.0,13/16.0),0,projectileTravelTime,1,projectileSpeed,QVector2D(direction.x(),direction.y())));
        canShoot = false;
        shoottimer.start(timerTime*1000,this);
    }
}

void TurretEnnemi::Update(){
    if (affected && !player->getHoldKey())
    {
        timerTime = initTime * 5;
    }
    else {
        timerTime = initTime;
    }
    IA();
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
