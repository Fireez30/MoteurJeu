#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <vector>

class Room;

#include "room.h"

class Camera{

public :
    Camera();
    void moveCamera(QVector3D trans);
    QVector3D getPosition();
    int getCurrentRoom();
    void setCurrentRoom(QVector3D trans);
    void setCurrentRoom(int x, int y);
    void setRooms(std::vector<Room*> r);

private :
    QVector3D position;
    int indexRoom;
    std::vector<Room*> scene;
};

#endif // CAMERA_H
