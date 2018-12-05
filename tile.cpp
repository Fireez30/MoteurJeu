#include "tile.h"

Tile::Tile(QVector2D pos,QVector2D text): Base2D(pos.x(),pos.y(),0,text),wall(false){

}

Tile::Tile(bool b, QVector2D pos,QVector2D text): Base2D(pos.x(),pos.y(),0,text),wall(b){

}

Tile::Tile(const Tile& t): Base2D(t.position.x(),t.position.y(),0,t.renderer.spriteCoords),wall(t.wall){

}
