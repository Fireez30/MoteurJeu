﻿#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <math.h>
#include <iostream>
#include <QVector3D>
#include <QTime>
#include <QtMultimedia/QMediaPlayer>
#include "player.h"
#include "rangedpile.h"
#include "largerpile.h"
#include "mainpile.h"

Player::Player():Movable(3,1,0,6,1,QVector2D(162,83),QVector2D(0.0,8.0/16.0),200,3,false),usePilePrincipale(true),holdKey(false),spriteModif(this),light(QVector2D(0,0),QVector3D(1,1,1),0.0005f,0.5f,180.0f,180.0f,QVector3D(1,0,0),3.5f,3.7f){
    spriteModif.AddSprite(QVector2D(0.0,11.0/16.0));//facing up
    spriteModif.AddSprite(QVector2D(0.0,10.0/16.0));//facing right
    spriteModif.AddSprite(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    spriteModif.AddSprite(QVector2D(0.0,9.0/16.0));//facing left
    orientationRatio = 0.7;
    principale = new MainPile(QVector2D(0,0),QVector2D(0,0));
    principale->changeLight();
    secondaire = nullptr;
    movAnim->StartAnimator();
    splayer = new QMediaPlayer;
    splayer->setMedia(QUrl::fromLocalFile("damage.wav"));
    splayer->setVolume(50);
}

Player::Player(int h,float x,float y, float sp,int cd,QVector2D dir,int animtime,int nbframes,bool animstatus):Movable(h,x,y,sp,cd,dir,QVector2D(0.0,8.0/16.0),animtime,nbframes,animstatus),usePilePrincipale(true),holdKey(false),spriteModif(this),light(QVector2D(x,y),QVector3D(1,1,1),0.0005f,0.5f,180.0f,180.0f,QVector3D(1,0,0),1.2f,1.5f){
    spriteModif.AddSprite(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    spriteModif.AddSprite(QVector2D(0.0,9.0/16.0));//facing left
    spriteModif.AddSprite(QVector2D(0.0,10.0/16.0));//facing right
    spriteModif.AddSprite(QVector2D(0.0,11.0/16.0));//facing up
    orientationRatio = 0.7;
    principale = new MainPile(QVector2D(0,0),QVector2D(0,0));
    secondaire = nullptr;
    movAnim->StartAnimator();
    splayer = new QMediaPlayer;
    splayer->setMedia(QUrl::fromLocalFile("damage.wav"));
    splayer->setVolume(50);
}

int Player::OnTriggerEnter(Interactable2D* other){

}

//retourne la pile en fonction de laquelle est utilisée
Pile* Player::getPileEnCours(){
    //qDebug("getPileEncours");
    if (!usePilePrincipale && secondaire != nullptr){
        return secondaire;
    }
    return principale;
}

//perd la référence a la pile secondaire
void Player::RemovePileSecondaire(){
    usePilePrincipale = true;
    if(secondaire!=nullptr)
        secondaire = nullptr;
}

//update la pile et la change si jamais le timer de celle ci arrive a 0
void Player::Update(){
    if (!usePilePrincipale && secondaire != nullptr){
         if(!secondaire->Update())
             RemovePileSecondaire();
    }
}

void Player::PickKey(){
    holdKey = true;
}

bool Player::getHoldKey(){
    return holdKey;
}

//détermine le vecteur de direction du joueur / souris pour changer le composant qui gère les états d'animations
void Player::ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj,QVector2D size){
    QVector3D mousePos = QVector3D(s.x(),s.y(),0);//pos souris
    QRect vp = QRect(0,0,size.x(),size.y());
    QVector3D worldpos = position.project(m,proj,vp);
    worldpos.setY(720-worldpos.y());
    this->direction = QVector2D(worldpos - mousePos).normalized();

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

//change pile secondaire
void Player::SetPilePrincipale(Pile *p){
    principale = p;
    principale->getCollider().~Hitbox();
}

Pile* Player::GetPilePrincipale(){
    return principale;
}

float Player::getRange(){
    if (!usePilePrincipale){
        return secondaire->GetRange();
    }
    else{
        return principale->GetRange();
    }
}

//appelé lorsqu'on passe une porte
void Player::changeRoom(QVector2D dir){
    lastMove = dir;
    position += lastMove;
    collider.setPoint(QVector2D(position.x(),position.y()));
    renderer.SetPosition(position);
    renderer.CreateGeometry();
}

float Player::getAngle(){
    if (!usePilePrincipale){
        return secondaire->GetConeAngle();
    }
    else{
        return principale->GetConeAngle();
    }
}

void Player::SetPileSecondaire(Pile *s){
    if(dynamic_cast<RangedPile*>(s)){
        Pile * p = new RangedPile(QVector2D(GetPosition().x(),GetPosition().y()),s->GetRange(),s->GetConeAngle(),s->getLifespan()/60,s->getMaxLifespan()/60,s->GetDamage(),s->renderer.GetTextCoords());
        p->canCollide = false;
        p->changeLight();
        secondaire = p;
    }
    else if(dynamic_cast<LargerPile*>(s)){
        Pile * p = new LargerPile(QVector2D(GetPosition().x(),GetPosition().y()),s->GetRange(),s->GetConeAngle(),s->getLifespan()/60,s->getMaxLifespan()/60,s->GetDamage(),s->renderer.GetTextCoords());
        p->canCollide = false;
        p->changeLight();
        secondaire = p;
    }
}

Pile* Player::getPileSecondaire(){
    //std::cout << "recuperation pile secondaire avant" << std::endl;
    if (secondaire != nullptr){
        //std::cout << "secondaire pas nulle" << std::endl;
        return secondaire;
    }
}

bool Player::utilisePilePrincipale(){
    return usePilePrincipale;
}

bool Player::utilisePileSecondaire(){
    return !usePilePrincipale;
}

void Player::setUtilisationSecondaire(bool b){
    if (secondaire != nullptr){
        usePilePrincipale = !b;
    }
}

LightSource* Player::getLight(){
    return &light;
}

//renvoie la lightsource correspondante
LightSource* Player::getLampeLight(){
    if(!usePilePrincipale && secondaire != nullptr)
        return secondaire->getLightSource();
    return principale->getLightSource();
}

//update les paramètres des lightsources
void Player::updateLights(){
    principale->getLightSource()->position = QVector2D(position.x(),position.y());
    if(secondaire != nullptr)
        secondaire->getLightSource()->position = QVector2D(position.x(),position.y());
}

void Player::PlayDamageSound(){
    splayer->play();
}
