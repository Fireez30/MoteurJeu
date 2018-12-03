#include "room.h"
#include "door.h"

Room::Room(){
    tiles = std::vector<Tile>();
    doors = std::vector<Door>();
    collisions  = std::vector<Hitbox>();
    interacts = std::vector<Interactable2D>();
}

Room::~Room(){
    tiles.clear();
    doors.clear();
    collisions.clear();
    interacts.clear();
}

void Room::ReadFile(std::vector<Rooms> r,int index){
        tinyxml2::XMLDocument doc;
        doc.LoadFile(r[index].path.data());
        tinyxml2::XMLElement* d = doc.RootElement();//<level width height>
        const char* walls = d[0].GetText();//wall layer
        int xmlIndex = 0;
        for (int i = 0; i < 15;i++){
            for (int j = 0; j < 25;j++){
                if (walls[xmlIndex] == 1){//sol
                   Tile t = Tile(QVector2D(i,j));
                   tiles.push_back(t);//pas un mur
                }
                if (walls[xmlIndex] == 2){//sol
                   Tile t = Tile(true,QVector2D(i,j));
                   tiles.push_back(t);//un mur
                }
                if (walls[xmlIndex] == 3){//portes
                    Door d = Door(QVector2D(i,j),false);
                    doors.push_back(d);//default unlocked
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
