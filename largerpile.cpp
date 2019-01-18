#include "largerpile.h"
#include "boss_torche.h"
#include <iostream>

LargerPile::LargerPile(QVector2D pos):Pile(pos,7,90,60000,60000,1,QVector2D(13.0/16.0,13.0/16.0),1,QVector3D(1,3,1)){

}

LargerPile::LargerPile(QVector2D pos,float r,float c,int l,int maxL,int d,QVector2D text):Pile(pos,r,c,l,maxL,d,text,1,QVector3D(1,3,1)){

}

//Pile fait des dégats + ralentit l'entité
void LargerPile::Affect(Movable *m){
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(bt == nullptr){
        m->ChangeSpeed(m->GetSpeed()*0.01f);
        m->Damage(GetDamage());
    }
}
