#include "mainpile.h"
#include "boss_torche.h"
#include <iostream>

MainPile::MainPile(QVector2D pos,QVector2D text):Pile(pos,7,50,60,0,text,0){

}

MainPile::MainPile(QVector2D pos,float r, float c, float l, int d,QVector2D text):Pile(pos,r,c,l,d,text,0){

}

void MainPile::Affect(Movable* m){
    Boss_torche* bt = dynamic_cast<Boss_torche*> (m);
    if(bt == nullptr){
        m->ChangeSpeed(m->GetSpeed()*0.01);
        //std::cout << "Pile principale affect " << std::endl;
    }
}
