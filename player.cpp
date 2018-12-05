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

Player::Player():Movable(162,83,1,QVector2D(1,0),QVector2D(0.0,8.0/16.0)){

}

Player::Player(float x,float y, float sp,QVector2D dir):Movable(x,y,sp,dir,QVector2D(0.0,8.0/16.0)){

}

void Player::Input(){
    //change direction using keyboard
}

void Player::Move(){
 //test hitboxes
//move in the direction of the player
}
void Player::OnTriggerEnter(Hitbox e){

}
