#ifndef PLAYER_H
#define PLAYER_H

#include "baseobject.h"
#include "movable.h"
#include "pile.h"

class Player : public Movable
{
protected:
    std::vector<QVector2D> sprites;
    int h, w;
    Pile *principale;
    Pile *secondaire;

public :
    Player();
    Player(float x,float y, float sp,QVector2D dir);
    void Input();
    int OnTriggerEnter(Interactable2D* other) override;
    void ChangePile(Pile *p);
    void ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj);
    void SetPilePrincipale(Pile *p);
    void SetPileSecondaire(Pile *s);
};

#endif // PLAYER_H
