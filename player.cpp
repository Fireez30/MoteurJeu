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

Player::Player():Movable(1,0,0.2,QVector2D(162,83),QVector2D(0.0,8.0/16.0)){
    sprites.push_back(QVector2D(0.0,11.0/16.0));//facing up
    sprites.push_back(QVector2D(0.0,10.0/16.0));//facing right
    sprites.push_back(QVector2D(0.0,8.0/16.0));//basic sprite orientation (facing down)
    sprites.push_back(QVector2D(0.0,9.0/16.0));//facing left
}

Player::Player(float x,float y, float sp,QVector2D dir):Movable(x,y,sp,dir,QVector2D(0.0,8.0/16.0)){
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

void Player::ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj){
    QVector3D pos = QVector3D(s.x(),s.y(),0);
    QVector3D f = (m.inverted()*pos)*proj.inverted();//mouse world pos
    //std::cout << "Player position : " << position.x() << " " << position.y() << std::endl;
    //std::cout << "Mouse position : " << f.x() << " " << f.y() << std::endl;
    if (-f.x() > position.x()){
        //std::cout << "sprite tourné vers la droite \n";
        renderer.spriteCoords = sprites[1];
    }
    else  if (-f.x() < position.x()){
         //std::cout << "sprite tourné vers la gauche \n";
        renderer.spriteCoords = sprites[3];
    }
    else if (-f.y() > position.y()){
         //std::cout << "sprite tourné vers le bas \n";
        renderer.spriteCoords = sprites[2];
    }
    else  if (-f.y() < position.y()){
         //std::cout << "sprite tourné vers le hait \n";
        renderer.spriteCoords = sprites[0];
    }
}

void Player::SetPilePrincipale(Pile *p){
    principale = p;
    principale->getCollider().~Hitbox();
}

void Player::SetPileSecondaire(Pile *s){
    secondaire = s;
    secondaire->getCollider().~Hitbox();
}

Pile* Player::getPileSecondaire(){
    return secondaire;
}
