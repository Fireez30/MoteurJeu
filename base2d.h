#ifndef BASE2D_H
#define BASE2D_H

#include <QVector3D>
#include "spriterenderer.h"

class Base2D{

protected:
    QVector3D position;


public :
    SpriteRenderer renderer;
    Base2D();
    Base2D(float x, float y, float z);
    //Base2D(Base2D const &);
    void SetPosition(QVector3D pos);
    QVector3D GetPosition();
    void Translate(QVector3D dir);
    void Render(QOpenGLShaderProgram *program);
};

#endif // BASE2D_H
