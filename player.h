#ifndef PLAYER_H
#define PLAYER_H

#include "baseobject.h"

class Player : public BaseObject
{

public :
    Player();
    void CreateGeometry() override;
    void Render(QOpenGLShaderProgram *program,QMatrix4x4 projection) override;
    void CreateCollider() override;
};

#endif // PLAYER_H
