#ifndef PLAYER_H
#define PLAYER_H

class Pile;

#include "movable.h"
#include "pile.h"
#include "spritemodifier.h"
#include "lightsource.h";

class Player : public Movable
{
protected:
    int h, w;
    bool usePilePrincipale;
    bool usePileSecondaire;
    Pile *principale;
    Pile *secondaire;
    float orientationRatio;
    bool holdKey;
    SpriteModifier spriteModif;
    LightSource light;

public :
    Player();
    Player(int h,float x,float y, float sp,QVector2D dir,int animtime,int nbframes,bool animstatus);
    void Input();
    int OnTriggerEnter(Interactable2D* other) override;
    void ChangePile(Pile *p);
    void ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj,QVector2D size);
    void SetPilePrincipale(Pile *p);
    void SetPileSecondaire(Pile *s);
    void Update() override;
    void changeRoom(QVector2D dir);
    Pile* getPileSecondaire();
    bool utilisePilePrincipale();
    bool utilisePileSecondaire();
    void setUtilisationPrincipale(bool b);
    float getRange();
    float getAngle();
    void RemovePileSecondaire();
    void setUtilisationSecondaire(bool b);
    void PickKey();
    bool getHoldKey();
    Pile* getPileEnCours();
    Pile* GetPilePrincipale();
    LightSource* getLight();
    LightSource* getLampeLight();
    void updateLights();
};

#endif // PLAYER_H
