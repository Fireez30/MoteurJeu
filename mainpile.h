#ifndef MAINPILE_H
#define MAINPILE_H

#include "pile.h"

class MainPile : public Pile{

public :
   MainPile(QVector2D pos,QVector2D text);
   MainPile(QVector2D pos,float r,float c, int l,int d,QVector2D text);
   void Affect(Movable* m) override;
};

#endif // MAINPILE_H
