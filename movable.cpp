#include "movable.h"
#include <iostream>

Movable::Movable():Interactable2D(QVector2D(0,0),QVector2D(0,0),1000),speed(1),direction(0,0),initPos(0,0),health(3),dead(false),lastMove(0,0){
    movAnim = new MovAnimator(this);
    initSpeed = speed;
    projectiles =  std::vector<Projectile*>();
    affected = false;
}

Movable::Movable(int h,float x, float y,float sp,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus):Interactable2D(pos,text,1000),speed(sp),direction(x,y),initPos(pos),health(h),dead(false),lastMove(0,0){
    movAnim = new MovAnimator(this,animtime,nbframes,animstatus);
    initSpeed = speed;
    projectiles =  std::vector<Projectile*>();
    affected = false;
}

QVector2D Movable::GetDirection(){
    return direction;
}

float Movable::GetSpeed(){
    return speed;
}

void Movable::CreateProjectileGeometry(){
    for (unsigned i = 0; i < projectiles.size(); i++){
        projectiles[i]->renderer.CreateGeometry();
    }
}

void Movable::RenderProjectile(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    for (unsigned i = 0; i < projectiles.size(); i++){
        projectiles[i]->Render(program,text);
    }
}


void Movable::ChangeSpeed(float s){
    speed = s;
}

void Movable::ChangeDirection(QVector2D dir){
    direction = dir;
}

void Movable::Move(QVector2D dir){
    lastMove = dir;
    position += lastMove*0.01667f;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
}

void Movable::ResetMove(){
    position += -lastMove*0.01667f;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
}

std::vector<Projectile*> Movable::getProjectiles(){
    return projectiles;
}

void Movable::ResetPos(){
    position = initPos;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
}

int Movable::getHealth(){
    return health;
}

void Movable::setHealth(int h){
    health = h;
}

void Movable::Damage(int d){
    if (canCollide){
        if (health > d){
            health -= d;
        }
        else {
            health = 0;
            dead = true;
        }
    }
    canCollide = false;
    startTimer();
}

QVector2D Movable::GetLastMove(){
    return lastMove;
}

bool Movable::isDead(){
    return dead;
}

bool Movable::getAffected()
{
    return affected;
}

void Movable::setAffected(bool a)
{
    affected = a;
}

QVector2D Movable::getDirection(){
    return direction;
}
