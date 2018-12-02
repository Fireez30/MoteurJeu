#ifndef TERRAIN_H
#define TERRAIN_H
#include "baseobject.h"
#include <vector>
struct Rooms
{
    std::string path;
    int x;
    int y;
};

 class Terrain : public BaseObject
 {
     vector<VertexData*> v;
     int roomIndexes;
public :
     Terrain();
     void CreateGeometry() override;
     void CreateGeometry(std::vector<Rooms> r);
     void CreateCollider() override;
     void Render(QOpenGLShaderProgram *program,QMatrix4x4 projection) override;
     void bindArrays();
 };

#endif // TERRAIN_H
