#include "room.h"
#include "door.h"

Room::Room(){
    tiles = std::vector<Tile>();
    doors = std::vector<Door>();
    collisions  = std::vector<Hitbox>();
    interacts = std::vector<Interactable2D>();
}

Room::Room(std::vector<Tile> t,std::vector<Door> d):tiles(t),doors(d){
    collisions  = std::vector<Hitbox>();
    interacts = std::vector<Interactable2D>();
}

void Room::ReadFile(std::vector<Rooms> r,int index){
        tinyxml2::XMLDocument doc;
        doc.LoadFile(r[index].path.data());
        tinyxml2::XMLElement* d = doc.RootElement();//<level width height>
        const char* walls = d[0].GetText();//wall layer
        int xmlIndex = 0;
        for (int i = 0; i < 15;i++){
            for (int j = 0; j < 25;j++){
                if (walls[xmlIndex] == 1){
                   tiles.push_back(Tile(QVector2D(i,j)));
                }
                if (walls[xmlIndex] == 2){
                    doors.push_back(Door(QVector2D(i,j),false));
                 }
                xmlIndex++;
            }
        }
}
void Room::Render(QOpenGLShaderProgram *program){
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].Render(program);
    }
    for (int i = 0; i < doors.size(); i++){
        doors[i].Render(program);
    }
    for (int i = 0; i < interacts.size(); i++){
        interacts[i].Render(program);
    }
}

void Room::ComputeHitboxes(){

}
