#include "rangedpile.h"
#include "boss_torche.h"
#include <iostream>

RangedPile::RangedPile(Player* p,QVector2D pos,QVector2D text):Pile(p,pos,6.25,50,60,1,text,0,QVector3D(3,0,0)){

}

RangedPile::RangedPile(Player* p,QVector2D pos,float r, float c, int l, int d,QVector2D text):Pile(p,pos,r,c,l,d,text,0,QVector3D(3,0,0)){

}

void RangedPile::Affect(Movable* m){
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(bt == nullptr){
        m->ChangeSpeed(m->GetSpeed()*0.01f);
    }
}
