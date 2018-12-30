#ifndef DOOR_H
#define DOOR_H

#include "interactable2d.h"
#include "player.h"
#include "camera.h"

class Door : public Interactable2D{

protected:
    bool locked;

private :
    Player* player;
    Camera* camera;
    QVector2D dir;
    QVector2D altText;

public :
    Door(const Door&d);
    Door(QVector2D pos,QVector2D text,QVector2D AltText);
    Door(QVector2D pos,QVector2D text,QVector2D AltText, bool state, QVector2D d, Player* p, Camera* c);
    bool IsLocked();
    void Unlock();
    void Lock();
    int OnTriggerEnter(Interactable2D* other) override;
    void setRoomAct(int i);
    void setRoomLink(int i);
};

#endif // DOOR_H
