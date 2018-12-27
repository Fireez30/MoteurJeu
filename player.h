#ifndef PLAYER_H
#define PLAYER_H

#include "movable.h"
#include "pile.h"


class Player : public Movable
{
protected:
    std::vector<QVector2D> sprites;
    int h, w;
    bool usePilePrincipale;
    bool usePileSecondaire;
    Pile *principale;
    Pile *secondaire = nullptr;
    QVector3D lastMove;
    float orientationRatio;
    QVector2D direction;

public :
    Player();
    Player(int h,float x,float y, float sp,QVector2D dir);
    void Input();
    int OnTriggerEnter(Interactable2D* other) override;
    void ChangePile(Pile *p);
    void ChangeOrientation(QPoint s,QMatrix4x4 m,QMatrix4x4 proj,QVector2D size);
    void SetPilePrincipale(Pile *p);
    void SetPileSecondaire(Pile *s);
    void Update() override;
    QVector2D GetDirection();
    QVector3D GetLastMove() override;
    void SetLastMove(QVector3D p);
    Pile* getPileSecondaire();
    bool utilisePilePrincipale();
    bool utilisePileSecondaire();
    void setUtilisationPrincipale(bool b);
    void setUtilisationSecondaire(bool b);
    Pile* GetPilePrincipale();
};

#endif // PLAYER_H
