#include "rangedpile.h"
#include "boss_torche.h"
#include <iostream>

RangedPile::RangedPile(Player* p,QVector2D pos,QVector2D text):Pile(p,pos,6.25,50,60,1,text,0,QVector3D(3,0,0)){

}

RangedPile::RangedPile(Player* p,QVector2D pos,float r, float c, float l, int d,QVector2D text):Pile(p,pos,r,c,l,d,text,0,QVector3D(3,0,0)){

}

void RangedPile::Affect(Movable* m){
    std::cout << "Affect ranged pile" << std::endl;
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    std::cout << "apres le dynamic cast" << std::endl;
    if(bt == nullptr){
        std::cout << "Affect ranged pile pas un boss" << std::endl;
        m->ChangeSpeed(m->GetSpeed()*0.01f);
         std::cout << "Affect ranged pile apres chnage speed" << std::endl;
        //std::cout << "Pile principale affect " << std::endl;
    }
     std::cout << "Affect FIN" << std::endl;
}
