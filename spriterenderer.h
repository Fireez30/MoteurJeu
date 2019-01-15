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
    //QOpenGLTexture *texture;

    float time;


public :
    QVector2D spriteCoords;
    int addXCoord;
    QVector2D initText;
    SpriteRenderer(const SpriteRenderer&) = delete;
    QTimer timer;
    SpriteRenderer(QVector3D pos);
    SpriteRenderer(std::string p,QVector2D coords,float time,QVector3D pos);
    //void initTextures();
    void ReleaseBuffers();
    ~SpriteRenderer();
    QVector2D GetTextCoords();
    std::string GetSpritePath();
    void ChangeSprite();
    void SetPosition(QVector3D pos);
    float GetXCoord();
    float GetYCoord();
    void SetXSpriteCoord(float x);
    void SetYSpriteCoord(float y);
    void CreateGeometry();
    void Render(QOpenGLShaderProgram *program,QOpenGLTexture *text);
    void setWidth(float f);
};

#endif // SPRITERENDERER_H
