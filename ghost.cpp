#include "ghost.h"
#include <iostream>

Ghost::Ghost(int h,float x, float y, float s,QVector2D pos,QVector2D text): Ennemi(pos,QVector2D(0,0),text,0,0,0,0,0,true){
    movAnim->StartAnimator();
    movAnim->Walk();
}

Ghost::Ghost(Room* r,Player* p,QVector2D pos): Ennemi(pos, QVector2D(0,0), QVector2D(6.0/16.0,12.0/16.0),10, 2.5, 1, 200,3,true){
    this->player = p;
    this->room = r;
    movAnim->StartAnimator();
    movAnim->Walk();
}

void Ghost::IA(){
    //se déplacer vers le joueur
    direction = QVector2D(player->position.x() - position.x(), player->position.y() - position.y());
    direction.normalize();
    direction *= speed;
    this->Move(direction);
    //si le joueur a la clé sur lui, capacité de tirer 1 projectile a la fois
    if (player->getHoldKey()){
        if (projectiles.size() == 0){
            projectiles.push_back(new Projectile(QVector2D(position.x(),position.y()),QVector2D(4/16.0,13/16.0),0,1,1,1,QVector2D(direction.x(),direction.y())));
        }
    }
}

void Ghost::Update(){
    IA();
    speed = initSpeed;

    if (affected)//ralentir les projectiles si le joueur éclaire
    {
        for (unsigned i = 0; i < projectiles.size(); i++){
            projectiles[i]->changeSpeed(0.5);
        }
    }
    //gestion de la destruction des projectiles
    for (unsigned i = 0; i < projectiles.size(); i++){
        if (projectiles[i]->Update() == -1){
            Projectile* truc = projectiles[i];
            projectiles.erase(projectiles.begin()+i);
            delete truc;
        }
    }
}

//gestion collision avec autre entité
int Ghost::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        if (player->canCollide)
            player->PlayDamageSound();
        player->Damage(1);
        //repousser joueur
        QVector3D pos_ennemi = this->position;
        QVector3D pos_player = other->position;
        QVector3D ennemi_to_player = QVector3D(pos_player.x() - pos_ennemi.x(), pos_player.y() - pos_ennemi.y(), 0);
        ennemi_to_player.normalize();
        QVector2D dirJoueur = QVector2D(ennemi_to_player.x()*0.8, ennemi_to_player.y()*0.8);
        player->Move(dirJoueur);
        if (room->CollisionCheck(player->getCollider())){//si la collision amene le joueur dans le mur, la reset
            player->ResetMove();
        }
        return 0;
    }

    return 1;
}
