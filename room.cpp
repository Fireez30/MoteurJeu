#include "room.h"

Room::Room(){
    tiles = std::vector<Tile>();
    doors = std::vector<Door>();
    collisions  = std::vector<Hitbox>();
}

Room::Room(std::vector<Tile> t,std::vector<Door> d):tiles(t),doors(d){
    collisions  = std::vector<Hitbox>();
}

void Room::Render(QOpenGLShaderProgram *program){

}

void Room::ComputeHitboxes(){

}
