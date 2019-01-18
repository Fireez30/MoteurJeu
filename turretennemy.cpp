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

//cooldown de tirs entre projectiles
void TurretEnnemi::startTimer(){
    shoottimer.start(timerTime*1000,this);
}

//permet de tirer a nouveau après le cooldown
void TurretEnnemi::timerEvent(QTimerEvent *){
    canShoot = true;
    shoottimer.stop();
}


void TurretEnnemi::IA(){
    if (targetPlayer){//2 versions, avec ou sans visée du joueur, ici calcul direction selon le joueur
        direction = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
        direction.normalize();
    }
    if (canShoot){//si je peux tirer
        projectiles.push_back(new Projectile(QVector2D(position.x(),position.y()),QVector2D(4/16.0,13/16.0),0,projectileTravelTime,1,projectileSpeed,QVector2D(direction.x(),direction.y())));
        canShoot = false;//start le cooldown de tir
        shoottimer.start(timerTime*1000,this);
    }
}

void TurretEnnemi::Update(){
    if (affected && !player->getHoldKey())//si jamais le joueur tient pas la clé et qu'il m'affecte, ralentit la vitesse de tir
    {
        timerTime = initTime * 5;
    }
    else {
        timerTime = initTime;
    }
    IA();
    //gestion destruction des projectiles
    for (unsigned i = 0; i < projectiles.size(); i++){
        if (projectiles[i]->Update() == -1){
            Projectile* truc = projectiles[i];
            projectiles.erase(projectiles.begin()+i);
            delete truc;
        }
    }
}

//empeche de se faire marcher dessus par le joueur
int TurretEnnemi::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        player->ResetMove();
    }

    return 1;
}
