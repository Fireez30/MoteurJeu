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

void Room::ReadFile(std::vector<Rooms> r,int index){
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(("C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\Rooms\\"+r[index].path).data()) == false){
                std::cout << "ca plante !" << std::endl;
            }//pose soucis !

        std::cout << "Path jusqu'au fichier level :" << r[index].path << std::endl;
        std::cout << "avant de print le doc" << std::endl;
        doc.Print();
        tinyxml2::XMLElement* d = doc.RootElement();//<level width height>
       // std::cout << "Size doc : " << d->
        std::cout << " Succes " << std::endl;//appelé
        std::cout << "layer mur1 :" << d->FirstChild()->Value() << std::endl; //renvoie "Murs"
         std::cout << "layer mur1 :" << d->FirstChild()->FirstChild()->Value() << std::endl; //CE UQIL FAUT
        const char* walls = d->FirstChild()->FirstChild()->Value();//wall layer
        std::cout << "récupération du layer mur réussie" << std::endl;
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
