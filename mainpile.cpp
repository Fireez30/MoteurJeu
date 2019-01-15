#include "mainpile.h"
#include "boss_torche.h"
#include <iostream>

MainPile::MainPile(Player* p,QVector2D pos,QVector2D text):Pile(p,pos,4,50,-42,-42,0,text,0,QVector3D(3,3,1)){

}

MainPile::MainPile(Player* p,QVector2D pos,float r, float c, int l, int d,QVector2D text):Pile(p,pos,r,c,l,l,d,text,0,QVector3D(3,3,1)){

}

void MainPile::Affect(Movable* m){
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(! (bt != nullptr)){
        m->ChangeSpeed(m->GetSpeed()*0.01f);
    }
}
