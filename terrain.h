#ifndef TERRAIN_H
#define TERRAIN_H
#include "baseobject.h"

 class Terrain : public BaseObject
 {

public :
     Terrain();
     void CreateGeometry() override;
     void Render(QOpenGLShaderProgram *program) override;
 };

#endif // TERRAIN_H
