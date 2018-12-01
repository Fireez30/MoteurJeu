#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <string>
#include <QVector2D>
#include <QTimer>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

class SpriteRenderer : protected QOpenGLFunctions{
protected :
    std::string spritePath;
    QTimer timer;
    QVector2D spriteCoords;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    float time;

public :
    SpriteRenderer();
    SpriteRenderer(std::string p,QVector2D coords,float time);
    ~SpriteRenderer();
    std::string GetSpritePath();
    void ChangeSprite();
    float GetXCoord();
    float GetYCoord();
    void CreateGeometry();
    void Render(QOpenGLShaderProgram *program);
};

#endif // SPRITERENDERER_H
