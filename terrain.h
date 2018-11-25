#ifndef TERRAIN_H
#define TERRAIN_H
#include "baseobject.h"

 class Terrain : public BaseObject
 {

public :
     Terrain();
     void CreateGeometry() override;
     void CreateCollider() override;
     void Render(QOpenGLShaderProgram *program,QMatrix4x4 projection) override;
 };

#endif // TERRAIN_H
