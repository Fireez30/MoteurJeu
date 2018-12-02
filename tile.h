#ifndef TILE_H
#define TILE_H

#include "base2d.h"

class Tile : public Base2D{
    bool wall;

public :
    Tile(QVector2D pos);
    Tile(bool type,QVector2D pos);
};

#endif // TILE_H
