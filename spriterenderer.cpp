#include "spriterenderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(QVector3D pos):spritePath("sprites.png"),spriteCoords(0,0),indexBuf(QOpenGLBuffer::IndexBuffer),position(pos),width(1){
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
    initText=spriteCoords;
}

SpriteRenderer::SpriteRenderer(std::string p,QVector2D coords,QVector3D pos):spritePath(p),spriteCoords(coords),indexBuf(QOpenGLBuffer::IndexBuffer),position(pos),width(1){
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
    initText=spriteCoords;
}
void SpriteRenderer::ReleaseBuffers(){
    arrayBuf.release();
    indexBuf.release();
}

SpriteRenderer::~SpriteRenderer(){
    arrayBuf.destroy();
    indexBuf.destroy();
}

float SpriteRenderer::getWidth()
{
    return width;
}

void SpriteRenderer::SetPosition(QVector3D pos){
    position = pos;
}

std::string SpriteRenderer::GetSpritePath(){
    return spritePath;
}
void SpriteRenderer::ChangeSprite(){

}

QVector2D SpriteRenderer::GetTextCoords(){
    return spriteCoords;
}

void SpriteRenderer::SetXSpriteCoord(float x){
    spriteCoords.setX(x);
}

void SpriteRenderer::SetYSpriteCoord(float y){
    spriteCoords.setY(y);
}

//construit la "géométrie" du sprite
void SpriteRenderer::CreateGeometry(){
    //compute a VertexData array
    VertexData v2[4] ={
        {position,QVector2D(spriteCoords.x(),spriteCoords.y()+1.0/16.0)},
        {QVector3D(position.x()+width,position.y(),0),QVector2D(spriteCoords.x()+1.0/16.0,spriteCoords.y()+1.0/16.0)},
        {QVector3D(position.x(),position.y()+1,0),spriteCoords },
        {QVector3D(position.x()+width,position.y()+1,0),QVector2D(spriteCoords.x()+1.0/16.0,spriteCoords.y())}
    };
    //compute indices
    GLushort indices[6] = {//2 triangles
      0 , 1 , 2 , 1 , 3, 2
    };//(top left top right bottom left) (top right bottom left bottom right)
    // Transfer vertex data to VBO 0

    arrayBuf.bind();
    arrayBuf.allocate(v2, 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 6 * sizeof(GLushort));
}

//s'occupe du rendu opengl
void SpriteRenderer::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    // Tell OpenGL which VBOs to use
    if (arrayBuf.bind() == false){
        std::cout << "Sprite renderer : arrayBind bug" << std::endl;
    };
    if (indexBuf.bind() == false){
        std::cout << "Sprite renderer : indexBind bug" << std::endl;
    };
    // Offset for position
    quintptr offset = 0;


    text->bind();

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);
    program->setUniformValue("objectwcoord",QVector4D(position.x(),position.y(),0,0));
    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void SpriteRenderer::setWidth(float f){
    width = f;
    this->CreateGeometry();
}
