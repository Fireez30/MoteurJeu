#include "hitbox.h"

Hitbox::Hitbox():leftCorner(0,0),width(1),height(1){

}

Hitbox::Hitbox(QVector2D p,int h, int w):leftCorner(p), width(w), height(h){

}

bool Hitbox::TestCollision(Hitbox other){
    return !(other.getXMin() >= getXMax() ||
           other.getXMax() <= getXMin() ||
           other.getYMin() >= getYMax() ||
           other.getYMax() <=getYMin());
}

float Hitbox::getXMax(){
	return leftCorner.x() + width;
}
float Hitbox::getXMin(){
	return leftCorner.x();
}
float Hitbox::getYMax(){
	return leftCorner.y() + height;
}
float Hitbox::getYMin(){
	return leftCorner.y();
}

void Hitbox::setPoint(QVector2D p){
    leftCorner = p;
}
