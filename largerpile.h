#ifndef LARGERPILE_H
#define LARGERPILE_H

#include "movable.h"
#include "pile.h"
#include "gamemanager.h"

class LargerPile : public Pile{

public :

   LargerPile(Player* p,QVector2D pos,QVector2D text);
   LargerPile(Player* p,QVector2D pos,float r,float c, float l,int d,QVector2D text);
   void Affect(Movable* m) override;
};

#endif // LARGERPILE_H
