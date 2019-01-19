#ifndef PLAYER_H
#define PLAYER_H

class Pile;

#include "movable.h"
#include "pile.h"
#include "spritemodifier.h"
#include "lightsource.h";
#include <QtMultimedia/QMediaPlayer>

class Player : public Movable
{
protected:
    int h, w;
    bool usePilePrincipale;
    Pile *principale;
    Pile *secondaire;
    float orientationRatio;
    bool holdKey;
    QMediaPlayer *splayer;
    SpriteModifier spriteModif;
    LightSource light;

public :
    Player();
    Player(int h,float x,float y, float sp,int cd,QVector2D dir,int animtime,int nbframes,bool animstatus);
    int OnTriggerEnter(Interactable2D* other) override;
    void ChangePile(Pile *p);
    void ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj,QVector2D size);
    void Update() override;
    void changeRoom(QVector2D dir);
    void RemovePileSecondaire();
    void PickKey();
    void updateLights();
    void PlayDamageSound();
    bool getHoldKey();
    Pile* getPileEnCours();
    Pile* GetPilePrincipale();
    LightSource* getLight();
    LightSource* getLampeLight();
    bool utilisePilePrincipale();
    bool utilisePileSecondaire();
    float getRange();
    float getAngle();
    Pile* getPileSecondaire();
    void SetPilePrincipale(Pile *p);
    void SetPileSecondaire(Pile *s);
    void setUtilisationSecondaire(bool b);
};

#endif // PLAYER_H
