#include "hitbox.h"

Hitbox::Hitbox():leftCorner(0,0),width(1),height(1){

}

Hitbox::Hitbox(QVector2D p,int h, int w):leftCorner(p), width(w), height(h){

}

bool Hitbox::TestCollision(Hitbox other){
    return false;//a completer
}
