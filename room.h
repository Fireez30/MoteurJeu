#ifndef ROOM_H
#define ROOM_H

class Door;
class Room;

#include <stdlib.h>
#include <stdio.h>
#include <QOpenGLShaderProgram>

#include "tile.h"
#include "player.h"
#include "camera.h"
#include "tinyxml2.h"
#include "movable.h"
#include "door.h"
#include "lightsource.h";

class Room {

protected:
    std::vector<Tile> tiles;
    std::vector<Hitbox> collisions;
    std::vector<Interactable2D*> pickups;
    std::vector<Movable*> entities;
    std::vector<LightSource*>* lights;
    Player* player;
    Door* boss;
    Movable* boss2;

public :
    Room(std::vector<LightSource*>* l);
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
