#include "rangedpile.h"
#include "boss_torche.h"

RangedPile::RangedPile(QVector2D pos,QVector2D text):Pile(pos,300,50,60,1,text,0,QVector3D(3,0,0)){

}

RangedPile::RangedPile(QVector2D pos,float r, float c, float l, int d,QVector2D text):Pile(pos,r,c,l,d,text,0,QVector3D(3,0,0)){

}

void RangedPile::Affect(Movable* m){
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(bt == nullptr){
        m->ChangeSpeed(m->GetSpeed()*0.01f);
        //std::cout << "Pile principale affect " << std::endl;
    }
}
