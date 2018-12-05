#include "interactable2d.h"

Interactable2D::Interactable2D(QVector2D pos,QVector2D text): Base2D(pos.x(),pos.y(),0,text){

}
void Interactable2D::setCollider(Hitbox h){
    collider = h;
}

Hitbox Interactable2D::getCollider(){
    return collider;
}
