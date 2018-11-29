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
#include "tinyxml2.h"

Terrain::Terrain(){
    initializeOpenGLFunctions();
    v = vector<VertexData*>();
    CreateGeometry();
}

void Terrain::CreateGeometry(std::vector<Room> r)
{
    for(unsigned i = 0; i < r.size(); i++){
        VertexData v2[25*15];//construction des positions des points dans la scène
        tinyxml2::XMLDocument doc;
        doc.LoadFile(r[i].path.data());
        tinyxml2::XMLElement* d = doc.RootElement();//<level width height>
        const char* walls = d[0].GetText();
        int index = 0;
        for (int i = 0; i < 15;i++){
            for (int j = 0; j < 25;j++){
                if (walls[0] == 1){
                    v2[index] = {QVector3D(i,j,0),QVector2D(0,0)};
                }
                index++;
            }
        }
        v.push_back(v2);
    }

    GLushort indices[15*15*6];
    for(int j=0;j<15;j++){
            for(int i=0; i<15;i++){
                indices[6*(j*15 + i)] = i+j*16;
                indices[6*(j*15 + i)+2] = i+16*(j+1);
                indices[6*(j*15 + i)+1] = i+1+16*(j+1);
                indices[6*(j*15 + i)+3] = i+j*16;
                indices[6*(j*15 + i)+4] = i+1+j*16;
                indices[6*(j*15 + i)+5] = i+1+16*(j+1);
            }
        }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(v[roomIndexes], 25*15 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 15*15*6 * sizeof(GLushort));
}

void Terrain::CreateGeometry()
{

}

void Terrain::CreateCollider(){

}

void Terrain::bindArrays(){
    arrayBuf.bind();
    arrayBuf.allocate(v[roomIndexes], 25*15 * sizeof(VertexData));
}

void Terrain::Render(QOpenGLShaderProgram *program,QMatrix4x4 projection){
           // std::cout << "Terrain, render " << std::endl;
      // Tell OpenGL which VBOs to use
      arrayBuf.bind();
      indexBuf.bind();

      // Offset for position
      quintptr offset = 0;

      QMatrix4x4 matrix;
      matrix.translate(realPosition.x(), realPosition.y(), realPosition.z());
      matrix.rotate(realRotation);
      // Set modelview-projection matrix
      program->setUniformValue("mvp_matrix", projection * matrix);
      // Tell OpenGL programmable pipeline how to locate vertex position data
      int vertexLocation = program->attributeLocation("a_position");
      program->enableAttributeArray(vertexLocation);
      program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

      // Offset for texture coordinate
      offset += sizeof(QVector3D);

      // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
      int texcoordLocation = program->attributeLocation("a_texcoord");
      program->enableAttributeArray(texcoordLocation);
      program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

      // Draw cube geometry using indices from VBO 1
      glDrawElements(GL_TRIANGLES, 15*15*6, GL_UNSIGNED_SHORT, 0);

      for (unsigned i = 0; i < childs.size(); i++){
          //we can optimize here (view dependant, too far from camera , ...)
          childs[i]->Render(program,projection);
      }
}
