#include "object2d.h"
#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <math.h>
#include <iostream>
#include <QVector3D>
#include <QTime>

Object2D::Object2D(){
    CreateGeometry();
    CreateCollider();
}
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};


void Object2D::CreateCollider(){
    //compute leftcorner pos
    // create the bounding box (give leftcorner, height, width and 1 if obstacle
}

void Object2D::CreateGeometry()
{
    std::cout << "Object2D, create geometry " << std::endl;
    std::vector<GLushort> indices;
    std::vector<QVector2D> textureCoords;
    std::vector<QVector3D> vertexCoords;
    std::string file = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\0" + meshFile;//LOD 0.
    QFile f(file.data());
    if (!f.open(QIODevice::ReadOnly)){
         std::cout<< " Erreur lors de l'ouverture du fichier" << endl;
         return;
    }
    char filter[256];
    f.readLine(filter,256);
    while (filter[0] == '#')//remove commentaries lines at the begginin
    {
        f.readLine(filter,256);
    }
    char line[64];
    int res = f.readLine(line,64);
    while (res != EOF){
        std::cout << "res = " << res << endl;
    if (line[0] == 'v' && line[1] == 't'){//case of the texture
        float x,y;
        //sscanf(line,"%s%s ",NULL,NULL);//dont read v and t
        sscanf(line,"vt %f %f %f",&x,&y,NULL);//read both coordinates
        textureCoords.push_back(QVector2D(x,y));
    }//end of texture
    else if (line[0] == 'v' && line[1] != 'n'){//case of coord
        float x,y,z;
        //sscanf(line,"%s ",NULL);//dont read v
        sscanf(line,"v %f %f %f",&x,&y,&z);//read the 3 coordinates
        std::cout << " coord " << x << y << z << std::endl;
        vertexCoords.push_back(QVector3D(x,y,z));
    }//end of coords
    else if (line[0] == 'f'){//if its indices of a triangle (f vertx/vertex/vertex text/text/text norm/norm/norm)
        GLushort v1,v2,v3;//vertex indices
       // sscanf(line,"%s ",NULL);//dont read f
       // sscanf(line,"%hu/",&v1);//read the indice 1
       // sscanf(line,"%hu/",&v2);
       // sscanf(line,"%hu/ ",&v3);
        sscanf(line,"f %hu %hu %hu %hu %hu %hu %hu %hu %hu",&v1,&v2,&v3,NULL,NULL,NULL,NULL,NULL,NULL);
        indices.push_back(v1);
        indices.push_back(v2);
        indices.push_back(v3);
    }//end of faces, the rest of the lines are not used
    res = f.readLine(line,64);
    }
    //end of lines
    for (int i = 0; i < vertexCoords.size(); i++){
        std::cout << "v " << vertexCoords[i].x() << " " << vertexCoords[i].y() << " " << vertexCoords[i].x() << std::endl;
    }
    //now we create vertexdata structure
    VertexData vertexs[vertexCoords.size()];
    for (unsigned i = 0; i < vertexCoords.size(); i++){
        vertexs[i] = {vertexCoords[i],textureCoords[i]};
    }

    arrayBuf.bind();
    arrayBuf.allocate(vertexs, vertexCoords.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices.data(), indices.size() * sizeof(GLushort));//data() = array of vector elements

    meshSize = indices.size();

    f.close();

    for (unsigned i = 0; i < childs.size(); i++){
        //we can optimize here (view dependant, too far from camera , ...)
        childs[i]->CreateGeometry();
    }
}

void Object2D::Render(QOpenGLShaderProgram *program,QMatrix4x4 projection){
    std::cout << "Object2D, Render " << std::endl;
   int size = 0;
   // Tell OpenGL which VBOs to use
   switch (lod){
       case 0:
           arrayBuf.bind();
           indexBuf.bind();
           size = meshSize;
           break;
/*
       case 1:
           arrayBuf1.bind();
           indexBuf1.bind();
           size = meshSize1;
           break;

       case 2:
           arrayBuf2.bind();
           indexBuf2.bind();
           size = meshSize2;
           break;
           */
   }


   // Offset for position
   quintptr offset = 0;

   QMatrix4x4 matrix;
   matrix.translate(realPosition.x(), realPosition.y(), realPosition.z());
   matrix.rotate(realRotation);
   // Set modelview-projection matrix
   program->setUniformValue("mvp_matrix", projection * matrix);
//! [6]
//!
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
   //manque le draw la
   //Render stuff here (render the loaded mesh)
   for (unsigned i = 0; i < childs.size(); i++){
       //we can optimize here (view dependant, too far from camera , ...)
       childs[i]->Render(program,projection);
   }
}
