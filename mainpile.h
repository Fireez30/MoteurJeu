#ifndef MAINPILE_H
#define MAINPILE_H

#include "pile.h"

class MainPile : public Pile{

public :
   MainPile(Player* p,QVector2D pos,QVector2D text);
   MainPile(Player* p,QVector2D pos,float r,float c, float l,int d,QVector2D text);
   void Affect(Movable* m) override;
};

#endif // MAINPILE_H
