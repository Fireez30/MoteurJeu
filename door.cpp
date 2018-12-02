#include "door.h"

Door::Door(QVector2D pos):Interactable2D(pos),locked(false){

}
Door::Door(QVector2D pos, bool state):Interactable2D(pos),locked(state){

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
