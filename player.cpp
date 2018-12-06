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

Player::Player():Movable(1,0,0.5,QVector2D(162,83),QVector2D(0.0,8.0/16.0)){

}

Player::Player(float x,float y, float sp,QVector2D dir):Movable(x,y,sp,dir,QVector2D(0.0,8.0/16.0)){

}

void Player::Input(){
    //change direction using keyboard
}

void Player::OnTriggerEnter(Interactable2D* other){

}
