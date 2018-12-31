#include "player.h"
#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <math.h>
#include <iostream>
#include <QVector3D>
#include <QTime>
#include "rangedpile.h"
#include "mainpile.h"

Player::Player():Movable(3,1,0,6,QVector2D(162,83),QVector2D(0.0,8.0/16.0),200,3,false),usePilePrincipale(false),usePileSecondaire(false),holdKey(false),spriteModif(this),light(QVector2D(0,0),QVector3D(1,1,1),0.0005f,0.5f,180.0f,180.0f,QVector3D(1,0,0),150.0f,200.0f){
    spriteModif.AddSprite(QVector2D(0.0,11.0/16.0));//facing up
    spriteModif.AddSprite(QVector2D(0.0,10.0/16.0));//facing right
    spriteModif.AddSprite(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    spriteModif.AddSprite(QVector2D(0.0,9.0/16.0));//facing left
    orientationRatio = 0.7;
    principale = new MainPile(QVector2D(0,0),QVector2D(0,0));
    movAnim->StartAnimator();
    std::cout << "spriteModif size " << spriteModif.nbOfSprites() << std::endl;
}

Player::Player(int h,float x,float y, float sp,QVector2D dir,int animtime,int nbframes,bool animstatus):Movable(h,x,y,sp,dir,QVector2D(0.0,8.0/16.0),animtime,nbframes,animstatus),usePilePrincipale(false),usePileSecondaire(false),holdKey(false),spriteModif(this),light(QVector2D(x,y),QVector3D(1,1,1),0.0005f,0.5f,180.0f,180.0f,QVector3D(1,0,0),150.0f,200.0f){
    spriteModif.AddSprite(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    spriteModif.AddSprite(QVector2D(0.0,9.0/16.0));//facing left
    spriteModif.AddSprite(QVector2D(0.0,10.0/16.0));//facing right
    spriteModif.AddSprite(QVector2D(0.0,11.0/16.0));//facing up
    orientationRatio = 0.7;
    principale = new MainPile(QVector2D(0,0),QVector2D(0,0));
    movAnim->StartAnimator();
    std::cout << "spriteModif size " << spriteModif.nbOfSprites() << std::endl;
}

void Player::Input(){
    //change direction using keyboard
}

int Player::OnTriggerEnter(Interactable2D* other){

}

void Player::SetLastMove(QVector3D p){
    lastMove = p;
}

QVector3D Player::GetLastMove(){
    return lastMove;
}

void Player::Update(){

}

void Player::PickKey(){
    holdKey = true;
}

bool Player::getHoldKey(){
    return holdKey;
}

void Player::ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj,QVector2D size){
    QVector3D mousePos = QVector3D(s.x(),s.y(),0);//pos souris
    QRect vp = QRect(0,0,size.x(),size.y());
    QVector3D worldpos = position.project(m,proj,vp);

    //std::cout << "player position " << worldpos.x() << " " << worldpos.y() << " " << worldpos.z() << std::endl;
    //USELESSstd::cout << "mouse position " << vIn.x() << " " << vIn.y() << " " << vIn.z() << std::endl;
    //std::cout << "mouse position " << mousePos.x() << " " << mousePos.y() << " " << mousePos.z() << std::endl;

    this->direction = QVector2D(worldpos - mousePos).normalized();
    //std::cout << "direction = " << direction.x() << " " << direction.y() << std::endl;
    if (direction.x() > orientationRatio && renderer.spriteCoords != spriteModif.GetSprite(3)){
        //std::cout << "sprite tourné vers la droite \n";
        spriteModif.ChangeSprite(3);
    }
    else  if (direction.x() < -orientationRatio && renderer.spriteCoords != spriteModif.GetSprite(1)){
        //std::cout << "sprite tourné vers la gauche \n";
        spriteModif.ChangeSprite(1);
    }
    else if (direction.y() > orientationRatio && renderer.spriteCoords != spriteModif.GetSprite(0)){
        //std::cout << "sprite tourné vers le bas \n";
        spriteModif.ChangeSprite(0);
    }
    else  if (direction.y() < -orientationRatio && renderer.spriteCoords != spriteModif.GetSprite(2)){
        //std::cout << "sprite tourné vers le hait \n";
       spriteModif.ChangeSprite(2);
    }

    renderer.CreateGeometry();
}

void Player::SetPilePrincipale(Pile *p){
    principale = p;
    principale->getCollider().~Hitbox();
}

Pile* Player::GetPilePrincipale(){
    return principale;
}

float Player::getRange(){
    if (usePilePrincipale){
        return principale->GetRange();
    }
    else if (usePileSecondaire) {
        return secondaire->GetRange();
    }

    return 0;
}

float Player::getAngle(){
    if (usePilePrincipale){
        return principale->GetConeAngle();
    }
    else if (usePileSecondaire) {
        return secondaire->GetConeAngle();
    }

    return 0;
}

void Player::SetPileSecondaire(Pile *s){
    secondaire = s;
    secondaire->getCollider().~Hitbox();
}

Pile* Player::getPileSecondaire(){
    return secondaire;
}

bool Player::utilisePilePrincipale(){
    return usePilePrincipale;
}

bool Player::utilisePileSecondaire(){
    return usePileSecondaire;
}

void Player::setUtilisationPrincipale(bool b){
    usePilePrincipale = b;
}

void Player::setUtilisationSecondaire(bool b){
    usePileSecondaire = b;
}

LightSource* Player::getLight(){
    return &light;
}

LightSource* Player::getLampeLight(){
    if(usePilePrincipale)
        return principale->getLightSource();
    return secondaire->getLightSource();
}

void Player::updateLights(){
    principale->getLightSource()->position = QVector2D(position.x(),position.y());
    if(secondaire != nullptr)
        secondaire->getLightSource()->position = QVector2D(position.x(),position.y());
}
