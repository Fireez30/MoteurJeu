#ifndef PLAYER_H
#define PLAYER_H

#include "movable.h"
#include "pile.h"

class Player : public Movable
{
protected:
    std::vector<QVector2D> sprites;
    int h, w;
    Pile *principale;
    Pile *secondaire = nullptr;

public :
    Player();
    Player(float x,float y, float sp,QVector2D dir);
    void Input();
    int OnTriggerEnter(Interactable2D* other) override;
    void ChangePile(Pile *p);
    void ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj);
    void SetPilePrincipale(Pile *p);
    void SetPileSecondaire(Pile *s);
    Pile* getPileSecondaire();
};

#endif // PLAYER_H
