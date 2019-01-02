#ifndef RANGEDPILE_H
#define RANGEDPILE_H

#include "movable.h"
#include "pile.h"
#include "gamemanager.h"
class RangedPile :public Pile{

public :
    RangedPile(Player* p,QVector2D pos,QVector2D text);
    RangedPile(Player* p,QVector2D pos,float r, float c, float l, int d,QVector2D text);
    void Affect(Movable* m) override;
};

#endif // RANGEDPILE_H
