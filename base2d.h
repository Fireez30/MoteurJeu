#ifndef BASE2D_H
#define BASE2D_H

#include <QVector3D>
#include "spriterenderer.h"

class Base2D{

protected:


public :

    QVector3D position;
    SpriteRenderer renderer;
    Base2D();
    Base2D(float x, float y, float z,QVector2D text);
    //Base2D(Base2D const &);
    void SetPosition(QVector3D pos);
    QVector3D GetPosition();
    void Render(QOpenGLShaderProgram *program,QOpenGLTexture *text);
    void DisplayCoords();
};

#endif // BASE2D_H
