#include <QVector2D>
#include <QVector3D>
#include <GL/gl.h>
#include <QQuaternion>
#include <iostream>
#include "geometryengine.h"
#include "baseobject.h"
#include <QImage>
#include <QFile>
#include <math.h>

int BaseObject::id = 0;

BaseObject::BaseObject() : indexBuf(QOpenGLBuffer::IndexBuffer){
    initializeOpenGLFunctions();
    rotation = QQuaternion(0,0,0,0);
    position = QVector3D(0,0,0);
    arrayBuf.create();
    indexBuf.create();
    id++;
    realRotation = rotation;
    realPosition = position;
    meshFile = "mesh.obj";
}


BaseObject::BaseObject(QQuaternion rot,QVector3D geo) : indexBuf(QOpenGLBuffer::IndexBuffer){
    initializeOpenGLFunctions();
    rotation = rot;
    position = geo;
    arrayBuf.create();
    indexBuf.create();
    id++;
    realRotation = rotation;
    realPosition = position;
}

BaseObject::~BaseObject(){
    arrayBuf.destroy();
    indexBuf.destroy();
}

void BaseObject::SetRotation(QQuaternion r){
    rotation = r;
}

QQuaternion BaseObject::GetRotation(){
    return rotation;
}

void BaseObject::SetPosition(QVector3D p){
    position = p;
}

QVector3D BaseObject::GetPosition(){
    return position;
}

void BaseObject::Rotate(QQuaternion r){
    rotation *= r;
    UpdatePositionInSpace();
}

void BaseObject::Translate(QVector3D v){
    position += v;
    UpdatePositionInSpace();
}

void BaseObject::SetChilds(vector<BaseObject*> v){
    childs = v;
}

void BaseObject::AddChild(BaseObject* c){
    childs.push_back(c);
}

vector<BaseObject*> BaseObject::GetChilds(){
    return childs;
}

BaseObject* BaseObject::GetChildAtIndex(unsigned i){
        return childs[i];
}

void BaseObject::SetChildAtIndex(BaseObject* c, unsigned i){
    if (i < childs.size()){
        childs[i] = c;
    }
}

BaseObject* BaseObject::GetParent(){
    return parent;
}
void BaseObject::SetParent(BaseObject* b){
    parent = b;
}

void BaseObject::UpdatePositionInSpace(){
    realRotation = rotation;//used not to update several times the rotation
    realPosition = position;//used not to update several times the position
    if (parent){
        realRotation *= parent->GetRotation();
        realPosition += parent->GetPosition();
    }
    for (unsigned i = 0; i < childs.size(); i++){
        childs[i]->UpdatePositionInSpace();
    }
}


void BaseObject::chooseLOD(QVector3D cam){
    this->UpdatePositionInSpace();
    double d = sqrt(pow(cam.x() - position.x(),2)+pow(cam.y() - position.y(),2)+pow(cam.z() - position.z(),2));
    if (d < 7)//values may change, just used these at the beggining
        lod = 2;
    else if (d < 20)
        lod = 1;
    else
        lod = 0;
}

