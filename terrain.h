#ifndef TERRAIN_H
#define TERRAIN_H
#include "baseobject.h"
#include <vector>
struct Room
{
    std::string path;
    int x;
    int y;
};

 class Terrain : public BaseObject
 {
     vector<vector<VertexData>> v;
public :
     Terrain();
     void CreateGeometry() override;
     void CreateCollider() override;
     void Render(QOpenGLShaderProgram *program,QMatrix4x4 projection) override;
 };

#endif // TERRAIN_H
