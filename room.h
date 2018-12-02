#ifndef ROOM_H
#define ROOM_H

#include <stdlib.h>
#include <stdio.h>
#include "tile.h"
#include "door.h"
#include <QOpenGLShaderProgram>

class Room {
    std::vector<Tile> tiles;
    std::vector<Door> doors;
    std::vector<Hitbox> collisions;

    Room();
    Room(std::vector<Tile> t,std::vector<Door> d);
    void Render(QOpenGLShaderProgram *program);
    void ComputeHitboxes();
};

#endif // ROOM_H
