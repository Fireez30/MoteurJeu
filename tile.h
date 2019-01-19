#ifndef TILE_H
#define TILE_H

#include "base2d.h"

class Tile : public Base2D{

public :
    Tile(const Tile&);
    Tile(QVector2D pos,QVector2D text);
};

#endif // TILE_H
