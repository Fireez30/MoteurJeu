#include "base2d.h"
#include <iostream>

Base2D::Base2D():position(0,0,0),renderer(position){
}

Base2D::Base2D(float x, float y, float z,QVector2D text):position(x,y,z),renderer("sprites.png",text,100,position){
}

void Base2D::SetPosition(QVector3D pos){
    position = pos;
}

QVector3D Base2D::GetPosition(){
    return position;
}

void Base2D::DisplayCoords(){
    std::cout<< "Base2D at x : " << position.x() << " and y : " << position.y() << std::endl;
}

void Base2D::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    //qDebug() << "Base2D - Render x " << position.x() << " y " << position.y();
    renderer.Render(program,text);
}
