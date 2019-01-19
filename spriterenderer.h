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
    QVector3D position;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    float width;

public :
    QVector2D spriteCoords;
    QVector2D initText;
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer(QVector3D pos);
    SpriteRenderer(std::string p,QVector2D coords,QVector3D pos);
    ~SpriteRenderer();
    void ReleaseBuffers();
    void ChangeSprite();
    void CreateGeometry();
    void Render(QOpenGLShaderProgram *program,QOpenGLTexture *text);
    QVector2D GetTextCoords();
    std::string GetSpritePath();
    float getWidth();
    void SetPosition(QVector3D pos);
    void SetXSpriteCoord(float x);
    void SetYSpriteCoord(float y);
    void setWidth(float f);
};

#endif // SPRITERENDERER_H
