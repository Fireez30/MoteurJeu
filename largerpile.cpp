#include "largerpile.h"
#include "boss_torche.h"
#include <iostream>

LargerPile::LargerPile(Player* p,QVector2D pos,QVector2D text):Pile(p,pos,7,30,90,1,text,1,QVector3D(0,3,0)){
}

LargerPile::LargerPile(Player* p,QVector2D pos,float r,float c,float l,int d,QVector2D text):Pile(p,pos,r,c,l,d,text,1,QVector3D(0,3,0)){

}

void LargerPile::Affect(Movable *m){
    std::cout << "affect large pile" << std::endl;
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(bt == nullptr){
        m->ChangeSpeed(m->GetSpeed()*0.01f);
        //std::cout << "Pile principale affect " << std::endl;
    }
}
