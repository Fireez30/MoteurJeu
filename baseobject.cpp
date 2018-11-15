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
}

BaseObject::BaseObject(QQuaternion rot,QVector3D geo) : indexBuf(QOpenGLBuffer::IndexBuffer){
    initializeOpenGLFunctions();
    rotation = rot;
    position = geo;
    arrayBuf.create();
    indexBuf.create();
    id++;
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
}

void BaseObject::Translate(QVector3D v){
    position += v;
}

void BaseObject::SetChilds(vector<BaseObject> v){
    childs = v;
}

vector<BaseObject> BaseObject::GetChilds(){
    return childs;
}

BaseObject BaseObject::GetChildAtIndex(unsigned i){
    if (i < childs.size())
        return childs[i];
    else
        return BaseObject();
}

void BaseObject::SetChildAtIndex(BaseObject c, unsigned i){
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

    realRotation *= parent->GetRotation();
    realPosition += parent->GetPosition();
    for (unsigned i = 0; i < childs.size(); i++){
        childs[i].UpdatePositionInSpace();
    }
}

void BaseObject::Render(QOpenGLShaderProgram *program)
{
     //PAS A JOUR, CF GEOMETRY ENGINE
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
    glDrawElements(GL_TRIANGLE_STRIP, meshSize * 3, GL_UNSIGNED_SHORT, 0);
    //Render stuff here (render the loaded mesh)
    for (unsigned i = 0; i < childs.size(); i++){
        //we can optimize here (view dependant, too far from camera , ...)
        childs[i].Render(program);
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

void BaseObject::CreateGeometry(QVector3D cam){
    //PAS A JOUR, CF GEOMETRY ENGINE
    std::vector<GLushort> indices;
    std::vector<QVector2D> textureCoords;
    std::vector<QVector3D> vertexCoords;
    string file = ""+lod+meshFile;//a file has different LODs represented as different files with the lod before the name of the file
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
    while (res != -1){
        std::cout << "Ligne lue = " << line << endl;
    if (line[0] == 'v' && line[1] == 't'){//case of the texture
        float x,y;
        sscanf_s(line,"%s%s ",NULL,NULL);//dont read v and t
        sscanf_s(line,"%f %f",&x,&y);//read both coordinates
        textureCoords.push_back(QVector2D(x,y));
    }//end of texture
    else if (line[0] == 'v' && line[1] != 'n'){//case of coord
        float x,y,z;
        sscanf_s(line,"%s ",NULL);//dont read v
        sscanf_s(line,"%f %f %f",&x,&y,&z);//read the 3 coordinates
        vertexCoords.push_back(QVector3D(x,y,z));
    }//end of coords
    else if (line[0] == 'f'){//if its indices of a triangle (f vertx/vertex/vertex text/text/text norm/norm/norm)
        GLushort v1,v2,v3;//vertex indices
        sscanf_s(line,"%s ",NULL);//dont read f
        sscanf_s(line,"%d/",&v1);//read the indice 1
        sscanf_s(line,"%d/",&v2);
        sscanf_s(line,"%d/ ",&v3);
        indices.push_back(v1);
        indices.push_back(v2);
        indices.push_back(v3);
    }//end of faces
    }//end of lines

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

    meshSize = vertexCoords.size();

    f.close();

    for (unsigned i = 0; i < childs.size(); i++){
        childs[i].CreateGeometry(cam);
    }
}
