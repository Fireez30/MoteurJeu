#ifndef RANGEDPILE_H
#define RANGEDPILE_H

#include "movable.h"
#include "pile.h"
#include "gamemanager.h"
class RangedPile :public Pile{

public :
    RangedPile(QVector2D pos);
    RangedPile(QVector2D pos,float r, float c, int l,int maxL, int d,QVector2D text);
    void Affect(Movable* m) override;
};

#endif // RANGEDPILE_H
