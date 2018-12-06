#include "rangedpile.h"


RangedPile::RangedPile(QVector2D pos,QVector2D text):Pile(pos,1.5,30,60,1,text){

}

RangedPile::RangedPile(QVector2D pos,float r, float c, float l, int d,QVector2D text):Pile(pos,r,c,l,d,text){

}

void RangedPile::OnTriggerEnter(Interactable2D* other){

}

void RangedPile::Affect(){

}
