#ifndef TERRAIN_H
#define TERRAIN_H
#include "baseobject.h"

 class Terrain : public BaseObject
 {

public :
     Terrain();
     virtual void CreateGeometry();
     virtual void Render(QOpenGLShaderProgram *program);
 };

#endif // TERRAIN_H
