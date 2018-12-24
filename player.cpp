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

Player::Player():Movable(1,0,0.2,QVector2D(162,83),QVector2D(0.0,8.0/16.0)),quellePile(true){
    sprites.push_back(QVector2D(0.0,11.0/16.0));//facing up
    sprites.push_back(QVector2D(0.0,10.0/16.0));//facing right
    sprites.push_back(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    sprites.push_back(QVector2D(0.0,9.0/16.0));//facing left
}

Player::Player(float x,float y, float sp,QVector2D dir):Movable(x,y,sp,dir,QVector2D(0.0,8.0/16.0)),quellePile(true){
    sprites.push_back(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    sprites.push_back(QVector2D(0.0,9.0/16.0));//facing left
    sprites.push_back(QVector2D(0.0,10.0/16.0));//facing right
    sprites.push_back(QVector2D(0.0,11.0/16.0));//facing up
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


void Player::ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj,QVector2D size){
    QVector3D mousePos = QVector3D(s.x(),s.y(),0);//pos souris
    QRect vp = QRect(0,0,size.x(),size.y());
    QVector3D worldpos = position.project(m,proj,vp);

    //std::cout << "player position " << worldpos.x() << " " << worldpos.y() << " " << worldpos.z() << std::endl;
    //USELESSstd::cout << "mouse position " << vIn.x() << " " << vIn.y() << " " << vIn.z() << std::endl;
    //std::cout << "mouse position " << mousePos.x() << " " << mousePos.y() << " " << mousePos.z() << std::endl;

    this->direction = QVector2D(worldpos - mousePos).normalized();
    std::cout << "direction = " << direction.x() << " " << direction.y() << std::endl;
    if (mousePos.x() > worldpos.x()){
        //std::cout << "sprite tourné vers la droite \n";
        renderer.spriteCoords = sprites[1];
    }
    else  if (mousePos.x() < worldpos.x()){
        //std::cout << "sprite tourné vers la gauche \n";
        renderer.spriteCoords = sprites[3];
    }
    else if (mousePos.y() > worldpos.y()){
        //std::cout << "sprite tourné vers le bas \n";
        renderer.spriteCoords = sprites[2];
    }
    else  if (mousePos.y() < worldpos.y()){
        //std::cout << "sprite tourné vers le hait \n";
        renderer.spriteCoords = sprites[0];
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

void Player::SetPileSecondaire(Pile *s){
    secondaire = s;
    secondaire->getCollider().~Hitbox();
}

Pile* Player::getPileSecondaire(){
    return secondaire;
}

bool Player::utilisePilePrincipale(){
    return quellePile;
}

void Player::setUtilisation(bool b){
    quellePile = b;
}

QVector2D Player::GetDirection(){
    return this->direction;
}

