#ifndef DOOR_H
#define DOOR_H

#include "interactable2d.h"
#include "player.h"
#include "camera.h"

class Door : public Interactable2D{

protected:
    bool locked;

public :
    Door(const Door&d);
    Door(QVector2D pos,QVector2D text);
    Door(QVector2D pos,QVector2D text, bool state, QVector2D d, Player* p, Camera* c);
    bool IsLocked();
    void Unlock();
    void OnTriggerEnter(Interactable2D* other) override;

private :
    Player* player;
    Camera* camera;
    QVector2D dir;
};

#endif // DOOR_H
