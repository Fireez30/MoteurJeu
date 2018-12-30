#include "projectile.h"
#include "player.h"


Projectile::Projectile():Interactable2D(QVector2D(0,0),QVector2D(0,0),0),lifespan(3),damage(1),speed(2),direction(QVector2D(0,0)){
    alive=true;
    setCollider(Hitbox(QVector2D(0,0),1,1));//porte ont un collider spécial
    timer.start(lifespan*1000,this);
}

Projectile::Projectile(QVector2D pos,QVector2D text,int collidetime,int lifetime,int dmg,float sp,QVector2D dir): Interactable2D (pos,text,collidetime),lifespan(lifetime),damage(dmg),speed(sp),direction(dir){
    alive = true;
    setCollider(Hitbox(pos,1,1));
    timer.start(lifespan*1000,this);
}

void Projectile::timerEvent(QTimerEvent *e){
    alive = false;
}

int Projectile::Update(){
    if (!alive){
        return -1;
    }

    QVector2D movement = direction * speed;
    movement.normalize();
    movement *= speed;
    position += movement * 1.66667;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
    return 0;
}


int Projectile::OnTriggerEnter(Interactable2D* other){
    Player* player = dynamic_cast<Player*> (other);
    if(player != nullptr){
        player->Damage(1);
        alive = false;
    }
}
