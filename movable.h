#ifndef MOVABLE_H
#define MOVABLE_H

class MovAnimator;

#include "interactable2d.h"
#include "movanimator.h"
#include "projectile.h"

class Movable: public Interactable2D
{
protected :
    float speed;
    float initSpeed;
    QVector2D direction;
    QVector2D initPos;
    QVector2D lastMove;
    int health;
    bool dead;
    std::vector<Projectile*> projectiles;
    bool affected;

public :
    MovAnimator* movAnim;

    Movable();
    Movable(int health,float x, float y,float sp,int damagecooldown,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus);
    QVector2D GetDirection();
    float GetSpeed();
    void ChangeSpeed(float s);
    void ChangeDirection(QVector2D dir);
    void Move(QVector2D dir);
    void ResetPos();
    virtual void Update() = 0;
    QVector2D GetLastMove();
    void ResetMove();
    int getHealth();
    void setHealth(int h);
    void Damage(int d);
    bool isDead();
    void RenderProjectile(QOpenGLShaderProgram *program,QOpenGLTexture *text);
    void CreateProjectileGeometry();
    std::vector<Projectile*> getProjectiles();
    bool getAffected();
    QVector2D getDirection();
    void setAffected(bool);
};

#endif // MOVABLE_H
