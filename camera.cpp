#include <iostream>

#include "camera.h"

Camera::Camera():position(-162.5,-83.5,-7.5){}
Camera::Camera(std::vector<UiObject*>* u):position(-162.5,-83.5,-7.5),ui(u){}

//transition caméra lorsqu'on change de salle
void Camera::moveCamera(QVector3D trans){
    position+=trans;
    for(int i=0;i<ui->size();i++){
        UiObject* u = ui->at(i);
        u->position -=trans;
        u->renderer.SetPosition(u->position);
        u->renderer.CreateGeometry();
    }
}

QVector3D Camera::getPosition(){return position;}
void Camera::setRooms(std::vector<Room*> r){
    scene = r;
}

int Camera::getCurrentRoom(){
    return indexRoom;
}

//change la salle actuellement affichée selon un vecteur
void Camera::setCurrentRoom(QVector3D trans){
   int i=0;
    QVector2D pos = scene[indexRoom]->getPos();
    int x = pos.x() - trans.x(),y = pos.y() - trans.y();
    while(i<scene.size() && !scene[i]->isThisRoom(x,y))
        i++;
    if(i>=scene.size())
        std::cout << "On a pas trouvé la salle à charger\n";
    else
    {
        indexRoom = i;
        scene[i]->CreateGeometry();
    }
}

//change la salle actuellement affichée selon des coordonées
void Camera::setCurrentRoom(int x, int y){
    int i=0;
    while(i<scene.size() && !scene[i]->isThisRoom(x,y))
        i++;
    if(i>=scene.size())
        std::cout << "On a pas trouvé la salle à charger\n";
    else
    {
        indexRoom = i;
        scene[i]->CreateGeometry();
    }
}
