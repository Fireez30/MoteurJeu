#include "camera.h"

Camera::Camera():position(-162.5,-83.5,-7.5){}

void Camera::moveCamera(QVector3D trans){
    position+=trans;
}

QVector3D Camera::getPosition(){return position;}
