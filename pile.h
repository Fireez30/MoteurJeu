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
    float range;
    float coneAngle;
    int lifespan,maxLife;
    int damage;
    int idPile;
    QVector3D color;
    LightSource ls;
    Player* player;


public :
    Pile(Player* p,QVector2D pos,QVector2D text);
    Pile(Player* p,QVector2D pos,float r, float c, int l, int maxL, int d,QVector2D text, int id,QVector3D thecolor);
    virtual void Affect(Movable* m) = 0;
    float GetRange();
    int OnTriggerEnter(Interactable2D* other) override;
    float GetConeAngle();
    float GetLifepan();
    float getRatioLife();
    int GetDamage();
    int getID();
    void EndOfTimer();
    void setLifespan(int life);
    int getLifespan();
    int getMaxLifespan();
    QVector3D getColor();
    LightSource* getLightSource();
    void changeLight();
    bool Update();
};

#endif // PILE_H
