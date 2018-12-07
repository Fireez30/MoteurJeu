#ifndef RANGEDPILE_H
#define RANGEDPILE_H

#include "pile.h"
#include "gamemanager.h"
class RangedPile :public Pile{

public :
    RangedPile(QVector2D pos,QVector2D text);
    RangedPile(QVector2D pos,float r, float c, float l, int d,QVector2D text);
    void Affect() override;
};

#endif // RANGEDPILE_H
