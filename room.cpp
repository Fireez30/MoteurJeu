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
    for(int i=0;i<doors.size();i++)
        doors[i].renderer.CreateGeometry();
}
void Room::ReadFile(std::vector<Rooms>* r,int index, std::string path){
        tinyxml2::XMLDocument doc;
        std::string s = path+"\\"+r->at(index).path;
        doc.LoadFile(s.c_str());
        //+r[index].path).data()
        tinyxml2::XMLElement* doc2 = doc.FirstChildElement("level");//<level width height>
        tinyxml2::XMLElement* d = doc2->FirstChildElement("Sol");//<level width height>
        int xRoom = r->at(index).x*25;
        int yRoom = r->at(index).y*15;
         for (tinyxml2::XMLElement* e = d->FirstChildElement("tile"); e != nullptr; e = e->NextSiblingElement("tile")){//y
            Tile t= Tile(QVector2D((float)e->IntAttribute("x")+xRoom,(float)(-1*e->IntAttribute("y"))+yRoom),QVector2D(e->IntAttribute("tx")/16.0,e->IntAttribute("ty")/16.0));
            tiles.push_back(t);
        }//Fin construction sol !
        tinyxml2::XMLElement* d2 = doc2->FirstChildElement("Murs");
        for (tinyxml2::XMLElement* e2 = d2->FirstChildElement("tile"); e2 != nullptr; e2 = e2->NextSiblingElement("tile")){//y
            Tile t= Tile(true,QVector2D((float)e2->IntAttribute("x")+xRoom,(float)(-1*e2->IntAttribute("y"))+yRoom),QVector2D(e2->IntAttribute("tx")/16.0,e2->IntAttribute("ty")/16.0));
            tiles.push_back(t);
            collisions.push_back(Hitbox(QVector2D(t.GetPosition().x(),t.GetPosition().y()),1,1));
        }//Fin construction Murs !
        tinyxml2::XMLElement* d3 = doc2->FirstChildElement("Doors");
        for (tinyxml2::XMLElement* e3 = d3->FirstChildElement("tile"); e3 != nullptr; e3 = e3->NextSiblingElement("tile")){//y
            doors.push_back(Door(QVector2D((float)e3->IntAttribute("x")+xRoom,(float)(-1*e3->IntAttribute("y"))+yRoom),QVector2D(e3->IntAttribute("tx")/16.0,e3->IntAttribute("ty")/16.0),false));
        }//Fin construction doors !
        doc.Clear();
        for (int i = 0; i < doors.size(); i++){
            std::cout << "Doors " << i << " at x : " << doors[i].GetPosition().x() << " and y : " << doors[i].GetPosition().y() << " text coords x :" << doors[i].renderer.GetXCoord() << "and y : " << doors[i].renderer.GetYCoord() << std::endl;
        }
}

void Room::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    //qDebug() << "Room - Render" << "size = " << tiles.size();
    for (int i = 0; i < interacts.size(); i++){
        interacts[i].Render(program,text);
    }
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].Render(program,text);
    }
    for (int i = 0; i < doors.size(); i++){
        doors[i].Render(program,text);
    }
}

void Room::RenderDoors(QOpenGLShaderProgram *program,QOpenGLTexture *text){

}
bool Room::TriggerCheck(Hitbox h){
    for (int i = 0; i < collisions.size();i++){
        if (collisions[i].TestCollision(h)){
            return true;
        }
    }
    return false;
}
