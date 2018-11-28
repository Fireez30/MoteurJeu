#include "terrain.h"
#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <math.h>
#include <iostream>
#include <QVector3D>
#include <QTime>
#include <QFile>

Terrain::Terrain(){
    initializeOpenGLFunctions();
    CreateGeometry();
    v = vector<VertexData*>();
}

void Terrain::CreateGeometry(std::vector<Room> r)
{
    for(unsigned i = 0; i < r.size(); i++){
        char* t = r[i].path.data();
        QFile file(t);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
               return;

    }
}

void Terrain::CreateCollider(){

}

void Terrain::Render(QOpenGLShaderProgram *program,QMatrix4x4 projection){

}
