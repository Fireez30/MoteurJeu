#include "projectile.h"
#include "player.h"
#include <iostream>

Projectile::Projectile():Interactable2D(QVector2D(0,0),QVector2D(0,0),0),lifespan(3),damage(1),speed(2),direction(QVector2D(0,0)){
    alive=true;
    initspeed = speed;
    setCollider(Hitbox(QVector2D(0,0),1,1));//porte ont un collider spécial
    timer.start(lifespan*1000,this);
}

Projectile::Projectile(QVector2D pos,QVector2D text,int collidetime,int lifetime,int dmg,float sp,QVector2D dir): Interactable2D (pos,text,collidetime),lifespan(lifetime),damage(dmg),speed(sp),direction(dir){
    alive = true;
    initspeed = speed;
    setCollider(Hitbox(pos,0.5,0.5));
    timer.start(lifespan*1000,this);
}

//timer durée de vie de la pile
void Projectile::timerEvent(QTimerEvent *e){
    alive = false;
    timer.stop();
}

//permet de réduire la vitesse des projectiles
void Projectile::changeSpeed(float factor){
    speed = initspeed * factor;
}

//permet de supprimer un projectile ayant atteint sa portée, sinon de le bouger dans sa direction
int Projectile::Update(){
    //std::cout << "projectile update" << std::endl;
    if (!alive){
        return -1;
    }

    QVector2D movement = direction * speed;
    movement.normalize();
    movement *= speed;
    position += (movement * 0.166667);
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
    speed = initspeed;
    return 0;

}

//lorsqu'on trigger avec un joueur, se détruire en lui faisant des dégats
int Projectile::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        if (player->canCollide)
            player->PlayDamageSound();
        player->Damage(1);
        alive = false;
    }
}
