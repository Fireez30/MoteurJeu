#ifndef MOVABLE_H
#define MOVABLE_H

class MovAnimator;

#include "interactable2d.h"
#include "movanimator.h"
#include "projectile.h"

class Movable: public Interactable2D
{
protected :
    float speed, initSpeed;
    QVector2D direction, initPos,lastMove;
    int health, maxHealth;
    bool dead, affected;
    std::vector<Projectile*> projectiles;

public :
    MovAnimator* movAnim;

    Movable();
    Movable(int health,float x, float y,float sp,int damagecooldown,QVector2D pos,QVector2D text,int animtime,int nbframes,bool animstatus);
    void Move(QVector2D dir);
    void ResetPos();
    virtual void Update() = 0;
    void ResetMove();
    void Damage(int d);
    bool isDead();
    void RenderProjectile(QOpenGLShaderProgram *program,QOpenGLTexture *text);
    void CreateProjectileGeometry();
    std::vector<Projectile*> getProjectiles();
    bool getAffected();
    QVector2D getDirection();
    int getHealth();
    QVector2D GetLastMove();
    QVector2D GetDirection();
    float GetSpeed();
    float getHealthRatio();
    void ChangeSpeed(float s);
    void ChangeDirection(QVector2D dir);
    void setHealth(int h);
    void setAffected(bool);
};

#endif // MOVABLE_H
