#include "tile.h"

Tile::Tile(QVector2D pos): Base2D(pos.x(),pos.y(),0),wall(false){

}

Tile::Tile(bool b, QVector2D pos): Base2D(pos.x(),pos.y(),0),wall(b){

}
