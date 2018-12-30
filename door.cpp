#include "door.h"

#include <iostream>

Door::Door(QVector2D pos,QVector2D text,QVector2D Alt):Interactable2D(pos,text,0),locked(false),altText(Alt){

}
Door::Door(QVector2D pos,QVector2D text,QVector2D Alt, bool state, QVector2D d, Player*p, Camera* c):Interactable2D(pos,text,0),locked(state), dir(d),player(p),camera(c),altText(Alt){

}

bool Door::IsLocked(){
    return locked;
}

void Door::Unlock(){
    locked = false;
    this->renderer.spriteCoords = altText;
}

void Door::Lock(){
    locked = true;
}

int Door::OnTriggerEnter(Interactable2D* other){
	//Si memory leak, regarder ici
    if (!locked){
        if(dynamic_cast<Player*> (other)!=NULL){
            camera->moveCamera(QVector3D(dir.x()*25,dir.y()*15,0));
            camera->setCurrentRoom(dir);
            QVector3D dirJoueur = QVector3D(dir.x()*-3,dir.y()*-3,0);
            player->Move(dirJoueur);
        }
    }
    else{
        //std::cout << "RECULE" << std::endl;
        player->Move(-player->GetLastMove());
    }
}

Door::Door(const Door& d):Interactable2D(QVector2D(d.position.x(),d.position.y()),d.renderer.spriteCoords,0),locked(d.locked){

}
