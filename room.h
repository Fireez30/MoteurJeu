#ifndef ROOM_H
#define ROOM_H

class Room;

#include <stdlib.h>
#include <stdio.h>
#include "tile.h"
#include "door.h"
#include "player.h"
#include "camera.h"
#include <QOpenGLShaderProgram>
#include "tinyxml2.h"

class Room {

protected:
    std::vector<Tile> tiles;
    std::vector<Hitbox> collisions;
    std::vector<Interactable2D*> interacts;

public :
    Room();
    ~Room();
    void CreateGeometry();
    void Render(QOpenGLShaderProgram *program,QOpenGLTexture *text);
    void ComputeHitboxes();
    void ReadFile(std::vector<Rooms>* r,int index,std::string path, Player* p, Camera* c);
    std::vector<Tile> GetTiles();
    bool TriggerCheck(Interactable2D* other);
    bool CollisionCheck(Hitbox h);
    bool isThisRoom(int _x, int _y);
    void setPosition(int _x, int _y);
    QVector2D getPos();

private:
    int x, y;
};

#endif // ROOM_H
