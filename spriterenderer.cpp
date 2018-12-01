#include "spriterenderer.h"

SpriteRenderer::SpriteRenderer():spritePath("sprite.png"),timer(),spriteCoords(0,0),indexBuf(QOpenGLBuffer::IndexBuffer),time(100){
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
}

SpriteRenderer::SpriteRenderer(std::string p,QVector2D coords,float t):spritePath(p),timer(),spriteCoords(coords),indexBuf(QOpenGLBuffer::IndexBuffer),time(t){
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
}

SpriteRenderer::~SpriteRenderer(){
    arrayBuf.destroy();
    indexBuf.destroy();
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
    VertexData v2[25*15];
    //compute indices
    GLushort indices[15*15*6];
    // Transfer vertex data to VBO 0

    arrayBuf.bind();
    arrayBuf.allocate(v2, 25*15 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 15*15*6 * sizeof(GLushort));
}

void SpriteRenderer::Render(QOpenGLShaderProgram *program){
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
