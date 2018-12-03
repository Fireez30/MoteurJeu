#include "base2d.h"

Base2D::Base2D():position(0,0,0),renderer(){
}

Base2D::Base2D(float x, float y, float z):position(x,y,z){
}

void Base2D::SetPosition(QVector3D pos){
    position = pos;
}

QVector3D Base2D::GetPosition(){
    return position;
}

void Base2D::Translate(QVector3D dir){
    position += dir;
}

void Base2D::Render(QOpenGLShaderProgram *program){
    renderer.Render(program);
}
