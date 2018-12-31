#ifndef ROOM_H
#define ROOM_H

class Door;
class Camera;

#include <stdlib.h>
#include <stdio.h>
#include "tile.h"
#include "player.h"
#include "camera.h"
#include <QOpenGLShaderProgram>
#include "tinyxml2.h"
#include "movable.h"
#include "door.h"

class Room {

protected:
    std::vector<Tile> tiles;
    std::vector<Hitbox> collisions;
    std::vector<Interactable2D*> pickups;
    std::vector<Movable*> entities;
    Player* player;
    Door* boss;
    Movable* boss2;

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
    void UpdateEntities();
    QVector2D getPos();
    bool IsPointInCircle(QVector2D *pt, QVector2D *center, float rayon);
    bool CheckColl(float rayon, float angle, QVector2D point);
    void setPlayer(Player* _p);
    void affectEnemiesInRange();
    bool wallOnTheVector(QVector2D vect);

private:
    int x, y;
};

#endif // ROOM_H
