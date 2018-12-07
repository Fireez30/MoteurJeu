#include "door.h"
#include <iostream>
Door::Door(QVector2D pos,QVector2D text):Interactable2D(pos,text),locked(false){

}
Door::Door(QVector2D pos,QVector2D text, bool state, QVector2D d, Player*p, Camera* c):Interactable2D(pos,text),locked(state), dir(d),player(p),camera(c){

}

bool Door::IsLocked(){
    return locked;
}

void Door::Unlock(){
    locked = true;
}

int Door::OnTriggerEnter(Interactable2D* other){
	//Si memory leak, regarder ici
    if(dynamic_cast<Player*> (other)!=NULL){
        camera->moveCamera(QVector3D(dir.x()*25,dir.y()*15,0));
        camera->setCurrentRoom(dir);
        QVector3D dirJoueur = QVector3D(dir.x()*-3,dir.y()*-3,0);
        player->Move(dirJoueur);
    }
}

Door::Door(const Door& d):Interactable2D(QVector2D(d.position.x(),d.position.y()),d.renderer.spriteCoords),locked(d.locked){

}
