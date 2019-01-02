#include "turretennemy.h"
#include <iostream>

TurretEnnemi::TurretEnnemi(int h,float x, float y, float s,QVector2D pos,QVector2D text): Movable(h,x,y,s,pos,text,200,3,false),shootdir(0,0),targetPlayer(false),timerTime(2),projectileSpeed(1),projectileTravelTime(1){
    canShoot = true;
    startTimer();
}

TurretEnnemi::TurretEnnemi(Room* r,Player* p,int h, float x, float y, float s,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus,bool targplayer,int cooldown,float projspeed,int projtime): Movable(h,x,y,s,pos,text,animtime,nbframes,animstatus),shootdir(x,y),targetPlayer(targplayer),timerTime(cooldown),projectileSpeed(projspeed),projectileTravelTime(projtime){
    this->player = p;
    this->room = r;
    canShoot = true;
    startTimer();
}

void TurretEnnemi::startTimer(){
    shoottimer.start(timerTime*1000,this);
}

void TurretEnnemi::timerEvent(QTimerEvent *){
    canShoot = true;
    std::cout << "jpeux retirer" << std::endl;
    //shoottimer.start(timerTime*1000,this);
}

void TurretEnnemi::IA(){
    //std::cout << "taille projectiles " << projectiles.size() << std::endl;
    if (targetPlayer){
        std::cout << "je tire vers l'ennemi peut etre" << std::endl;
        shootdir = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
        shootdir.normalize();
    }
    if (canShoot){
        projectiles.push_back(new Projectile(QVector2D(position.x(),position.y()),QVector2D(4/16.0,13/16.0),0,projectileTravelTime,1,projectileSpeed,QVector2D(shootdir.x(),shootdir.y())));
        std::cout << "shootdir = " << shootdir.x() << " " << shootdir.y() << std::endl;
        canShoot = false;
    }
}

void TurretEnnemi::Update(){
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
