#ifndef DOOR_H
#define DOOR_H

#include "interactable2d.h"

class Door : public Interactable2D{

protected:
    bool locked;

public :
    Door(const Door&d);
    Door(QVector2D pos,QVector2D text);
    Door(QVector2D pos,QVector2D text, bool state);
    bool IsLocked();
    void Unlock();
    void OnTriggerEnter(Hitbox other) override;
};

#endif // DOOR_H
