#include "room.h"
#include "door.h"
#include <iostream>
#include "rangedpile.h"

Room::Room(){
    tiles = std::vector<Tile>();
    collisions  = std::vector<Hitbox>();
    interacts = std::vector<Interactable2D*>();
}

Room::~Room(){
    tiles.clear();
    collisions.clear();
    for (int i = 0; i < interacts.size();i++){
        delete interacts[i];
    }
    interacts.clear();
}

bool Room::isThisRoom(int _x, int _y){
    return x == _x && y == _y;
}

QVector2D Room::getPos(){
    return QVector2D(x,y);
}
void Room::setPosition(int _x, int _y){
    x=_x;
    y=_y;
}
std::vector<Tile> Room::GetTiles(){
    return tiles;
}

void Room::CreateGeometry(){
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].renderer.CreateGeometry();
    }

    for(int i=0;i<interacts.size();i++){
        std::cout << "interacts create geometry" << std::endl;
        interacts[i]->renderer.CreateGeometry();
    }
}

void Room::ReadFile(std::vector<Rooms>* r,int index, std::string path, Player* p, Camera* c){
        tinyxml2::XMLDocument doc;
        std::string s = path+"\\"+r->at(index).path;
        doc.LoadFile(s.c_str());
        //+r[index].path).data()

        tinyxml2::XMLElement* doc2 = doc.FirstChildElement("level");//Structure XML : récupérer la racine

        tinyxml2::XMLElement* d = doc2->FirstChildElement("Sol");// Depuis la racine, récupération du noeud contenant la liste des  sols
        int xRoom = r->at(index).x*25;
        int yRoom = r->at(index).y*15;
         for (tinyxml2::XMLElement* e = d->FirstChildElement("tile"); e != nullptr; e = e->NextSiblingElement("tile")){//y
            tiles.push_back(Tile(QVector2D((float)e->IntAttribute("x")+xRoom,(float)(-1*e->IntAttribute("y"))+yRoom),QVector2D(e->IntAttribute("tx")/16.0,e->IntAttribute("ty")/16.0)));
        }//Fin construction sol !

        tinyxml2::XMLElement* d2 = doc2->FirstChildElement("Murs");//Depuis la racine, récupération du noeuf contenant la liste des murs
        for (tinyxml2::XMLElement* e2 = d2->FirstChildElement("tile"); e2 != nullptr; e2 = e2->NextSiblingElement("tile")){//y
            Tile t= Tile(true,QVector2D((float)e2->IntAttribute("x")+xRoom,(float)(-1*e2->IntAttribute("y"))+yRoom),QVector2D(e2->IntAttribute("tx")/16.0,e2->IntAttribute("ty")/16.0));
            tiles.push_back(t);
            collisions.push_back(Hitbox(QVector2D(t.GetPosition().x(),t.GetPosition().y()),1,1));//les murs ont une hitbox
        }//Fin construction Murs !

        tinyxml2::XMLElement* d3 = doc2->FirstChildElement("Doors");//noeuds portes
        for (tinyxml2::XMLElement* e3 = d3->FirstChildElement("tile"); e3 != nullptr; e3 = e3->NextSiblingElement("tile")){//y
            float x = (float)e3->IntAttribute("x"), y = (float)(-1*e3->IntAttribute("y"));
            QVector2D dir;
            if(x == 0)
                dir.setX(1);
            else if(x == 24)
                dir.setX(-1);
            else if (y==0)
                dir.setY(-1);
            else if (y==-14)
                dir.setY(1);
            Door* d = new Door(QVector2D(x+xRoom,y+yRoom),QVector2D(e3->IntAttribute("tx")/16.0,e3->IntAttribute("ty")/16.0),false,dir,p,c);
            d->setCollider(Hitbox(QVector2D(d->position.x(),d->position.y()),1,1));//porte ont un collider spécial
            interacts.push_back(d);
        }//Fin construction doors !

        tinyxml2::XMLElement* d4 = doc2->FirstChildElement("Entite");
        if (d4){//si la salle que l'on lit contient une entité
            for (tinyxml2::XMLElement* e4 = d4->FirstChildElement("Pile"); e4 != nullptr; e4 = e4->NextSiblingElement("Pile")){//Liste des piles
                if (e4->IntAttribute("type") == 0){//type 0 = pile a portée améliorée
                    interacts.push_back(new RangedPile(QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0)));
                }
                //for (int i = 0; i < interacts.size(); i++){
                //    std::cout << "interact at : " << i << " xcord = " << interacts[i]->GetPosition().x() << " ycord = " << interacts[i]->GetPosition().y() <<  std::endl;
                //}
            }//fin for piles, rajouter des fors pour les autres entités
        }
        doc.Clear();//vider le doc
}

void Room::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    for (int i = 0; i < interacts.size(); i++){
        interacts[i]->Render(program,text);//contient entités + portes
    }
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].Render(program,text);//contient sol + murs
    }
}

bool Room::CollisionCheck(Hitbox h){//collisions des murs unqiuements
    for (int i = 0; i < collisions.size();i++){
        if (collisions[i].TestCollision(h)){
            return true;
        }
    }
    return false;
}
bool Room::TriggerCheck(Interactable2D* other){//collisions portes et entités
    for(int i=0;i<interacts.size();i++){
        if(interacts[i]->getCollider().TestCollision(other->getCollider())){
            interacts[i]->OnTriggerEnter(other);
            return true;
        }
    }
    return false;
}
