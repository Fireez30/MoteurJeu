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

void Player::OnTriggerEnter(Hitbox e){

}

void Player::ChangeOrientation(int x, int y,QMatrix4x4 m){
    QVector3D pos = position;
    QVector3D f = m*pos;
    if (x > f.x()){
        renderer.spriteCoords = sprites[1];
    }
    else  if (x < f.x()){
        renderer.spriteCoords = sprites[3];
    }
    if (y > f.y()){
        renderer.spriteCoords = sprites[0];
    }
    else  if (y < f.y()){
        renderer.spriteCoords = sprites[2];
    }
}
