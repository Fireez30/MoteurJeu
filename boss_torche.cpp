#include "boss_torche.h"
#include <iostream>


Boss_torche::Boss_torche(int h,float x, float y, float s,QVector2D pos,QVector2D text): Ennemi(pos,QVector2D(0,0),text,0,0,0,0,0,true){
    movAnim->StartAnimator();
    movAnim->Walk();
}

Boss_torche::Boss_torche(Room* r,Player* p,QVector2D pos): Ennemi(pos,QVector2D(0,0),QVector2D(0.0/16.0,12.0/16.0),10,2.1,3,200,3,true){
    this->player = p;
    this->room = r;
    //startTimer();
    movAnim->StartAnimator();
    movAnim->Walk();
}

void Boss_torche::startTimer(){
    timer.start(1000,this);

}

void Boss_torche::IA(){
     direction = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
     direction.normalize();
     direction *= speed;
     if( !affected ){
        this->Move(direction);//collision check a faire
     }
     else {
        this->Move(-direction);//collision check a faire
        if (room->CollisionCheck(this->getCollider())){//si la collision amene le joueur dans le mur, la reset
            this->ResetMove();
        }
     }
     affected = false;
}


void Boss_torche::Update(){
    speed = initSpeed;
    if (health < maxHealth/2)
    {
        speed *= 3;
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

QVector2D Boss_torche::GetRoomPos(){
    return room->getPos();
}

int Boss_torche::OnTriggerEnter(Interactable2D* other){
    Player* player2 = dynamic_cast<Player*> (other);
    if(player2 != nullptr){
        if (player->canCollide)
            player->PlayDamageSound();
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

