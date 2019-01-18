#include "rangedpile.h"
#include "boss_torche.h"
#include <iostream>

RangedPile::RangedPile(QVector2D pos):Pile(pos,10,40,60000,60000,1,QVector2D(13.0/16.0,14.0/16.0),0,QVector3D(3,1,1)){

}

RangedPile::RangedPile(QVector2D pos,float r, float c, int l,int maxL, int d,QVector2D text):Pile(pos,r,c,l,maxL,d,text,0,QVector3D(3,1,1)){

}

//Pile qui fait des dégats
void RangedPile::Affect(Movable* m){
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(bt == nullptr){
        m->ChangeSpeed(m->GetSpeed()*0.01f);
        m->Damage(GetDamage());
    }
}
