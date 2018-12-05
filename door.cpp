#include "door.h"
#include <iostream>
Door::Door(QVector2D pos,QVector2D text):Interactable2D(pos,text),locked(false){

}
Door::Door(QVector2D pos,QVector2D text, bool state):Interactable2D(pos,text),locked(state){

}

bool Door::IsLocked(){
    return locked;
}

void Door::Unlock(){
    locked = true;
}

void Door::OnTriggerEnter(Hitbox other){
    //if hitbox player
    //if its open
    //change room
}

Door::Door(const Door& d):Interactable2D(QVector2D(d.position.x(),d.position.y()),d.renderer.spriteCoords),locked(d.locked){

}
