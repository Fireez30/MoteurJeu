#include "room.h"
#include "door.h"
#include <iostream>

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

std::vector<Tile> Room::GetTiles(){
    return tiles;
}

void Room::CreateGeometry(){
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].renderer.CreateGeometry();
    }
}
void Room::ReadFile(std::vector<Rooms> r,int index){
        tinyxml2::XMLDocument doc;
        doc.LoadFile(("C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\Rooms\\"+r[index].path).data());
        tinyxml2::XMLElement* d = doc.RootElement();//<level width height>
        int xmlIndex = 0;
        int xRoom = r[index].x;
        int yRoom = r[index].y;

        for (int i = 0; i < 15;i++){//y
            for (int j = 0; j < 25;j++){//x
                if (d->FirstChild()->FirstChild()->Value()[xmlIndex] == '1'){//sol
                   tiles.push_back(Tile(QVector2D(j+xRoom,i+yRoom)));//pas un mur
                }
                else if (d->FirstChild()->FirstChild()->Value()[xmlIndex] == '2'){//mur
                   tiles.push_back(Tile(true,QVector2D(j+xRoom,i+yRoom)));//un mur
                }
                else if (d->FirstChild()->FirstChild()->Value()[xmlIndex] == '3'){//portes
                    doors.push_back(Door(QVector2D(j+yRoom,i+xRoom)));//default unlocked
                 }
                xmlIndex++;
            }
            xmlIndex++;//retour chariot
        }
        doc.Clear();
}
void Room::Render(QOpenGLShaderProgram *program){
    //qDebug() << "Room - Render" << "size = " << tiles.size();
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].Render(program);
    }
   /* for (int i = 0; i < doors.size(); i++){
        doors[i].Render(program);
    }
    for (int i = 0; i < interacts.size(); i++){
        interacts[i].Render(program);
    }*/
}

void Room::ComputeHitboxes(){

}
