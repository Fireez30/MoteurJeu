#ifndef PILE_H
#define PILE_H

class Player;

#include "interactable2d.h"
#include "movable.h"
#include "lightsource.h"
#include "player.h"
#include <QTimer>

class Pile : public Interactable2D {


protected:
    float range,coneAngle;
    int lifespan,maxLife, damage, idPile;
    QVector3D color;
    LightSource ls;


public :
    Pile(QVector2D pos,QVector2D text);
    Pile(QVector2D pos,float r, float c, int l, int maxL, int d,QVector2D text, int id,QVector3D thecolor);
    virtual void Affect(Movable* m) = 0;
    int OnTriggerEnter(Interactable2D* other) override;
    void EndOfTimer();
    void changeLight();
    bool Update();
    float GetRange();
    float GetConeAngle();
    float GetLifepan();
    float getRatioLife();
    int GetDamage();
    int getID();
    int getLifespan();
    int getMaxLifespan();
    QVector3D getColor();
    LightSource* getLightSource();
    void setLifespan(int life);
};

#endif // PILE_H
