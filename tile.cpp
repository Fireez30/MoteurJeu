#include "tile.h"

Tile::Tile(QVector2D pos,QVector2D text): Base2D(pos.x(),pos.y(),0,text){

}

Tile::Tile(const Tile& t): Base2D(t.position.x(),t.position.y(),0,t.renderer.spriteCoords){

}
