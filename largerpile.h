#ifndef LARGERPILE_H
#define LARGERPILE_H

#include "movable.h"
#include "pile.h"
#include "gamemanager.h"

class LargerPile : public Pile{

public :

   LargerPile(QVector2D pos);
   LargerPile(QVector2D pos,float r,float c, int l,int maxL,int d,QVector2D text);
   void Affect(Movable* m) override;
};

#endif // LARGERPILE_H
