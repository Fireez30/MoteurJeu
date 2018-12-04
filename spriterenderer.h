#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <string>
#include <QVector2D>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "vertexdata.h"

struct Rooms
{
    std::string path;
    int x;
    int y;
};


class SpriteRenderer : protected QOpenGLFunctions{

protected :
    std::string spritePath;
    QVector2D spriteCoords;
    QVector3D position;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLTexture *texture;
    float time;


public :
    SpriteRenderer(const SpriteRenderer&) = delete;
    QTimer timer;
    SpriteRenderer(QVector3D pos);
    SpriteRenderer(std::string p,QVector2D coords,float time,QVector3D pos);
    void initTextures();
    void ReleaseBuffers();
    ~SpriteRenderer();
    std::string GetSpritePath();
    void ChangeSprite();
    float GetXCoord();
    float GetYCoord();
    void CreateGeometry();
    void Render(QOpenGLShaderProgram *program);
};

#endif // SPRITERENDERER_H
