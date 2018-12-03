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

void Room::ReadFile(std::vector<Rooms> r,int index){
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(("C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\Rooms\\"+r[index].path).data()) == false){
                std::cout << "ca plante !" << std::endl;
            }//pose soucis !

        doc.Print();
        tinyxml2::XMLElement* d = doc.RootElement();//<level width height>
        const char* walls = d->FirstChild()->FirstChild()->Value();//wall layer
        int xmlIndex = 0;
        int xRoom = 0, yRoom = 0;
        for (int i = 0; i < 15;i++){
            for (int j = 0; j < 25;j++){
                if (walls[xmlIndex] == '1'){//sol
                   Tile t = Tile(QVector2D(i+xRoom,j+yRoom));
                   tiles.push_back(t);//pas un mur
                }
                else if (walls[xmlIndex] == '2'){//mur
                   Tile t = Tile(true,QVector2D(i+xRoom,j+yRoom));
                   tiles.push_back(t);//un mur
                }
                else if (walls[xmlIndex] == '3'){//portes
                    Door d = Door(QVector2D(i+xRoom,j+yRoom),false);
                    doors.push_back(d);//default unlocked
                 }
                xmlIndex++;
            }
        }
        std::cout << "XmlIndex = " << xmlIndex << std::endl;
}
void Room::Render(QOpenGLShaderProgram *program){
    qDebug() << "Room - Render" << "size = " << tiles.size();
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
