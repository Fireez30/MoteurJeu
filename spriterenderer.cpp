#include "spriterenderer.h"

SpriteRenderer::SpriteRenderer():spritePath("sprite.png"),timer(),spriteCoords(0,0),indexBuf(QOpenGLBuffer::IndexBuffer),time(100),texture(0){
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
    initTextures();
}

SpriteRenderer::SpriteRenderer(std::string p,QVector2D coords,float t,QVector3D pos):spritePath(p),timer(),spriteCoords(coords),indexBuf(QOpenGLBuffer::IndexBuffer),time(t),position(pos),texture(0){
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
    initTextures();
}

SpriteRenderer::~SpriteRenderer(){
    arrayBuf.destroy();
    indexBuf.destroy();
    delete texture;
}

void SpriteRenderer::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(spritePath.data())); //chargement de la sprite sheet ici

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

std::string SpriteRenderer::GetSpritePath(){
    return spritePath;
}
void SpriteRenderer::ChangeSprite(){

}

float SpriteRenderer::GetXCoord(){
    spriteCoords.x();
}

float SpriteRenderer::GetYCoord(){
    spriteCoords.y();
}

void SpriteRenderer::CreateGeometry(){
    //compute a VertexData array
    VertexData v2[4] ={
        {position,spriteCoords},{QVector3D(position.x()+1,position.y(),0),QVector2D(spriteCoords.x()+1.0,spriteCoords.y())},
        {QVector3D(position.x(),position.y()+1,0),QVector2D(spriteCoords.x(),spriteCoords.y()+1.0)},
        {QVector3D(position.x()+1,position.y()+1,0),QVector2D(spriteCoords.x()+1.0,spriteCoords.y()+1.0)}
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

void SpriteRenderer::Render(QOpenGLShaderProgram *program){
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();
    // Offset for position
    quintptr offset = 0;

    texture->bind();

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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}