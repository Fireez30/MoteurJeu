/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <QImage>
#include <iostream>
#include <QFile>
#include <iostream>
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer),indexBuf1(QOpenGLBuffer::IndexBuffer),indexBuf2(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    arrayBuf1.create();
    indexBuf1.create();

    arrayBuf2.create();
    indexBuf2.create();

    lod = 0;
    // Initializes cube geometry and transfers it to VBOs
    initMeshGeometry("mesh.obj"); //still didn't find a 3 differents LOD model
    //initMeshGeometry1("mesh.obj");
    //initMeshGeometry2("mesh.obj");
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]
void GeometryEngine::initPlaneGeometry()
{
    QImage img;
    img.load("hmap3.png");
    std::cout << "Avant la génération des points " << std::endl;
    std::cout << qGray(img.pixel(0,0)) << std::endl;
    VertexData vertices[256];
     int index = 0;

    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
             vertices[index] = {QVector3D(-8.0+(float)(i), -8.0+(float)(j),qGray(img.pixel(i*(img.width()-1)/15.0,(j*(img.height()-1)/15.0)))/255.0), QVector2D((float)(i)/(float)(15.0), (float)(j)/(float)(15.0))};
             index++;
         }
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
 //! [1]
     // Transfer vertex data to VBO 0
     arrayBuf.bind();
     arrayBuf.allocate(vertices, 256 * sizeof(VertexData));

     // Transfer index data to VBO 1
     indexBuf.bind();
     indexBuf.allocate(indices, 15*15*6 * sizeof(GLushort));
}

void GeometryEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//! [1]
}

void GeometryEngine::initQuadTreeGeometry()
{

}
void GeometryEngine::initMeshGeometry1(std::string meshFile){//LOD 1
    std::vector<GLushort> indices;
    std::vector<QVector2D> textureCoords;
    std::vector<QVector3D> vertexCoords;
    std::string file = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurDeJeuxM2\\1" + meshFile;//LOD1.
    std::cout << "File :" << file << std::endl;
    QFile f(file.data());
    if (!f.open(QIODevice::ReadOnly)){
         std::cout<< " Erreur lors de l'ouverture du fichier" << endl;
         return;
    }
    std::cout << "Fichier ouvert ! " << std::endl;
    char filter[256];
    f.readLine(filter,256);
    while (filter[0] == '#')//remove commentaries lines at the begginin
    {
        f.readLine(filter,256);
    }
    std::cout << "avant lecture premiere ligne" << std::endl;
    char line[64];
    int res = f.readLine(line,64);
     std::cout << "apres lecture premiere ligne res = " << res << std::endl;
    while (res != EOF){
        std::cout << "res = " << res << endl;
    if (line[0] == 'v' && line[1] == 't'){//case of the texture
        float x,y;
        sscanf(line,"%s%s ",NULL,NULL);//dont read v and t
        sscanf(line,"%f %f",&x,&y);//read both coordinates
        textureCoords.push_back(QVector2D(x,y));
    }//end of texture
    else if (line[0] == 'v' && line[1] != 'n'){//case of coord
        float x,y,z;
        sscanf(line,"%s ",NULL);//dont read v
        sscanf(line,"%f %f %f",&x,&y,&z);//read the 3 coordinates
        vertexCoords.push_back(QVector3D(x,y,z));
    }//end of coords
    else if (line[0] == 'f'){//if its indices of a triangle (f vertx/vertex/vertex text/text/text norm/norm/norm)
        GLushort v1,v2,v3;//vertex indices
        sscanf(line,"%s ",NULL);//dont read f
        sscanf(line,"%hu/",&v1);//read the indice 1
        sscanf(line,"%hu/",&v2);
        sscanf(line,"%hu/ ",&v3);
        indices.push_back(v1);
        indices.push_back(v2);
        indices.push_back(v3);
    }//end of faces
    res = f.readLine(line,64);
    }
    //end of lines
    std::cout << "Fin de lecture des lignes" << std::endl;
    //now we create vertexdata structure
    VertexData vertexs[vertexCoords.size()];
    for (unsigned i = 0; i < vertexCoords.size(); i++){
        vertexs[i] = {vertexCoords[i],textureCoords[i]};
    }

    arrayBuf1.bind();
    arrayBuf1.allocate(vertexs, vertexCoords.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf1.bind();
    indexBuf1.allocate(indices.data(), indices.size() * sizeof(GLushort));//data() = array of vector elements

    meshSize1 = indices.size();

    f.close();
}

void GeometryEngine::initMeshGeometry2(std::string meshFile){//LOD 2
    std::vector<GLushort> indices;
    std::vector<QVector2D> textureCoords;
    std::vector<QVector3D> vertexCoords;
    std::string file = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurDeJeuxM2\\2" + meshFile;//LOD2.
    std::cout << "File :" << file << std::endl;
    QFile f(file.data());
    if (!f.open(QIODevice::ReadOnly)){
         std::cout<< " Erreur lors de l'ouverture du fichier" << endl;
         return;
    }
    std::cout << "Fichier ouvert ! " << std::endl;
    char filter[256];
    f.readLine(filter,256);
    while (filter[0] == '#')//remove commentaries lines at the begginin
    {
        f.readLine(filter,256);
    }
    std::cout << "avant lecture premiere ligne" << std::endl;
    char line[64];
    int res = f.readLine(line,64);
     std::cout << "apres lecture premiere ligne res = " << res << std::endl;
    while (res != EOF){
        std::cout << "res = " << res << endl;
    if (line[0] == 'v' && line[1] == 't'){//case of the texture
        float x,y;
        sscanf(line,"%s%s ",NULL,NULL);//dont read v and t
        sscanf(line,"%f %f",&x,&y);//read both coordinates
        textureCoords.push_back(QVector2D(x,y));
    }//end of texture
    else if (line[0] == 'v' && line[1] != 'n'){//case of coord
        float x,y,z;
        sscanf(line,"%s ",NULL);//dont read v
        sscanf(line,"%f %f %f",&x,&y,&z);//read the 3 coordinates
        vertexCoords.push_back(QVector3D(x,y,z));
    }//end of coords
    else if (line[0] == 'f'){//if its indices of a triangle (f vertx/vertex/vertex text/text/text norm/norm/norm)
        GLushort v1,v2,v3;//vertex indices
        sscanf(line,"%s ",NULL);//dont read f
        sscanf(line,"%hu/",&v1);//read the indice 1
        sscanf(line,"%hu/",&v2);
        sscanf(line,"%hu/ ",&v3);
        indices.push_back(v1);
        indices.push_back(v2);
        indices.push_back(v3);
    }//end of faces
    res = f.readLine(line,64);
    }
    //end of lines
    std::cout << "Fin de lecture des lignes" << std::endl;
    //now we create vertexdata structure
    VertexData vertexs[vertexCoords.size()];
    for (unsigned i = 0; i < vertexCoords.size(); i++){
        vertexs[i] = {vertexCoords[i],textureCoords[i]};
    }

    arrayBuf2.bind();
    arrayBuf2.allocate(vertexs, vertexCoords.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf2.bind();
    indexBuf2.allocate(indices.data(), indices.size() * sizeof(GLushort));//data() = array of vector elements

    meshSize2 = indices.size();

    f.close();
}

void GeometryEngine::initMeshGeometry(std::string meshFile){//LOD 0
    std::vector<GLushort> indices;
    std::vector<QVector2D> textureCoords;
    std::vector<QVector3D> vertexCoords;
    std::string file = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurDeJeuxM2\\0" + meshFile;//LOD 0.
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
}

void GeometryEngine::drawMeshGeometry(QOpenGLShaderProgram *program)
{
    int size = 0;
    // Tell OpenGL which VBOs to use
    switch (lod){
        case 0:
            arrayBuf.bind();
            indexBuf.bind();
            size = meshSize;
            break;

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
    }


    // Offset for position
    quintptr offset = 0;

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

    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::drawQuadTreeGeometry(QOpenGLShaderProgram *program)
{

}
void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

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
}
//! [2]
void GeometryEngine::drawCubeGeometry(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

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
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}
//! [2]
