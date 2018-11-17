#ifndef OBJECT2D_H
#define OBJECT2D_H

#include "baseobject.h"

class Object2D : public BaseObject
{

public :
    Object2D();
    void CreateGeometry() override;
    void Render(QOpenGLShaderProgram *program,QMatrix4x4 projection) override;
};

#endif // OBJECT2D_H
