#include "tile.h"

Tile::Tile(QVector2D pos): Base2D(pos.x(),pos.y(),0),wall(false){

}

Tile::Tile(bool b, QVector2D pos): Base2D(pos.x(),pos.y(),0),wall(b){

}

Tile::Tile(const Tile& t): Base2D(t.position.x(),t.position.y(),0),wall(t.wall){

}
