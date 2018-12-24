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

void Player::ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj){
    QVector3D pos = QVector3D(s.x(),s.y(),0);//pos souris
    QVector3D screenplayer = (m*proj)*position;//mouse world pos
    std::cout << "Player position : " << screenplayer.x() << " " << screenplayer.y() << std::endl;
    if (pos.x() > screenplayer.x()){
        //std::cout << "sprite tourné vers la droite \n";
        renderer.spriteCoords = sprites[1];
        direction = QVector2D(1,0);
    }
    else  if (pos.x() < screenplayer.x()){
         //std::cout << "sprite tourné vers la gauche \n";
        renderer.spriteCoords = sprites[3];
        direction = QVector2D(-1,0);
    }
    else if (pos.y() > screenplayer.y()){
         //std::cout << "sprite tourné vers le bas \n";
        renderer.spriteCoords = sprites[2];
        direction = QVector2D(0,1);
    }
    else  if (pos.y() < screenplayer.y()){
         //std::cout << "sprite tourné vers le hait \n";
        renderer.spriteCoords = sprites[0];
        direction = QVector2D(0,-1);
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


float Player::CalcTriArea(QVector3D *v1, QVector3D *v2, QVector3D *v3)
{
  float det = 0.0f;
  det = ((v1->x() - v3->x()) * (v2->y() - v3->y())) - ((v2->x() - v3->x()) * (v1->y() - v3->y()));
  return (det / 2.0f);
}


bool Player::IsPointInTri(QVector3D *pt, QVector3D *v1, QVector3D *v2, QVector3D *v3)
{
  float TotalArea = CalcTriArea(v1, v2, v3);
  float Area1 = CalcTriArea(pt, v2, v3);
  float Area2 = CalcTriArea(pt, v1, v3);
  float Area3 = CalcTriArea(pt, v1, v2);

  if((Area1 + Area2 + Area3) > TotalArea)
    return false;
  else
    return true;
}

bool Player::utilisePilePrincipale(){
    return quellePile;
}

void Player::setUtilisation(bool b){
    quellePile = b;
}

bool Player::CheckColl(float rayon, float angle, QVector3D point)
{
    QVector3D origin = this->GetPosition();
    QVector3D A;
    QVector3D B;
    QVector3D centre;
    float oppose = rayon + tanf(angle/2);

    if( origin.x() == 1 && origin.y() == 0)
    {
        centre.setX( origin.x() + rayon);
        centre.setY( origin.y() );

        A.setX( centre.x() );
        A.setY( centre.y() + oppose );

        B.setX( centre.x() );
        B.setX( centre.x() - oppose );
    }
    else if( origin.x() == -1 && origin.y() == 0)
    {
        centre.setX( origin.x() - rayon);
        centre.setY( origin.y() );

        A.setX( centre.x() );
        A.setY( centre.y() + oppose );

        B.setX( centre.x() );
        B.setX( centre.x() - oppose );
    }
    else if( origin.x() == 0 && origin.y() == 1)
    {
        centre.setX( origin.x());
        centre.setY( origin.y() + rayon);

        A.setX( centre.x() - oppose);
        A.setY( centre.y() );

        B.setX( centre.x() + oppose);
        B.setX( centre.x() );
    }
    else if( origin.x() == 0 && origin.y() == -1)
    {
        centre.setX( origin.x());
        centre.setY( origin.y() - rayon);

        A.setX( centre.x() - oppose);
        A.setY( centre.y() );

        B.setX( centre.x() + oppose);
        B.setX( centre.x() );
    }

    // on a donc A, B et origin qui font un triangle

    if( IsPointInTri(&point, &A, &B, &origin) == true ) return true;
    else return false;
}

