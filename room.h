#ifndef ROOM_H
#define ROOM_H

#include <stdlib.h>
#include <stdio.h>
#include "tile.h"
#include "door.h"
#include <QOpenGLShaderProgram>
#include "tinyxml2.h"

class Room {

protected:
    std::vector<Tile> tiles;
    std::vector<Door> doors;
    std::vector<Hitbox> collisions;
    std::vector<Interactable2D> interacts;

public :
    Room();
    ~Room();
    void Render(QOpenGLShaderProgram *program);
    void ComputeHitboxes();
    void ReadFile(std::vector<Rooms> r,int index);
};

#endif // ROOM_H
