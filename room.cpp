#include "room.h"
#include "door.h"
#include <iostream>
#include "rangedpile.h"
#include "ennemi.h"
#include <math.h>
#include "largerpile.h"

Room::Room(){
    tiles = std::vector<Tile>();
    collisions  = std::vector<Hitbox>();
    pickups = std::vector<Interactable2D*>();
    entities = std::vector<Movable*>();
    player = nullptr;
}

Room::~Room(){
    tiles.clear();
    collisions.clear();
    for (int i = 0; i < pickups.size();i++){
        delete pickups[i];
    }
    pickups.clear();

    for (int i = 0; i < entities.size(); i++){
        delete entities[i];
    }
    entities.clear();
}

bool Room::isThisRoom(int _x, int _y){
    return x == _x && y == _y;
}

void Room::UpdateEntities(){
    for (int i = 0; i < entities.size(); i++){
        entities[i]->Update();
        if (CollisionCheck(entities[i]->getCollider())){//si l'entité a collide avec un mur, reset sa position
            entities[i]->Move(-(entities[i]->GetLastMove()*0.0166));
        }
    }
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

void Room::setPlayer(Player* _p){
    player = _p;
}

void Room::CreateGeometry(){
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].renderer.CreateGeometry();
    }

    for(int i=0;i<pickups.size();i++){
        pickups[i]->renderer.CreateGeometry();
    }

    for (int i = 0 ; i < entities.size(); i++){
        entities[i]->ResetPos();
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
            pickups.push_back(d);
        }//Fin construction doors !

        tinyxml2::XMLElement* d4 = doc2->FirstChildElement("Entite");
        if (d4){//si la salle que l'on lit contient une entité
            for (tinyxml2::XMLElement* e4 = d4->FirstChildElement("Pile"); e4 != nullptr; e4 = e4->NextSiblingElement("Pile")){//Liste des piles
                if (e4->IntAttribute("type") == 0){//type 0 = pile a portée améliorée
                    {
                        RangedPile *r = new RangedPile(QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0));
                        r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
                        pickups.push_back(r);
                    }
                }
                if (e4->IntAttribute("type") == 1){//type 1 = Pile à cone plus grand
                    {
                        LargerPile *r = new LargerPile(QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0));
                        r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
                        pickups.push_back(r);
                    }
                }
                //for (int i = 0; i < interacts.size(); i++){
                //    std::cout << "interact at : " << i << " xcord = " << interacts[i]->GetPosition().x() << " ycord = " << interacts[i]->GetPosition().y() <<  std::endl;
                //}
            }//fin for piles, rajouter des fors pour les autres entités

            for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Ghost"); e5 != nullptr; e5 = e5->NextSiblingElement("Ghost")){//Liste des Ghosts
                Ennemi* e =new Ennemi(this,p,10,0,0,0.8,QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0));
                e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
                entities.push_back(e);
                //for (int i = 0; i < interacts.size(); i++){
                //    std::cout << "interact at : " << i << " xcord = " << interacts[i]->GetPosition().x() << " ycord = " << interacts[i]->GetPosition().y() <<  std::endl;
                //}
            }//fin for piles, rajouter des fors pour les autres entités
        }
        doc.Clear();//vider le doc
}

void Room::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    for (int i = 0; i < pickups.size(); i++){
        pickups[i]->Render(program,text);//contient piles + portes
    }
    for (int i = 0; i < entities.size(); i++){
        entities[i]->Render(program,text);//contient entités
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
    for(int i=0;i<pickups.size();i++){
        if(pickups[i]->canCollide && pickups[i]->getCollider().TestCollision(other->getCollider())){
            Pile* pile = dynamic_cast<Pile*> (pickups[i]);
            Player* p= dynamic_cast<Player*> (other);
            if(pile != nullptr && p != nullptr){
                int idPile = -1;
                Pile * pileJoueur = p->getPileSecondaire() ;
                if(pileJoueur!= nullptr)
                    idPile = pileJoueur->getID();
                if(idPile==0){
                    RangedPile *r = new RangedPile(QVector2D(pile->position.x(),pile->position.y()),pileJoueur->renderer.GetTextCoords());
                    r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
                    r->renderer.CreateGeometry();
                    r->canCollide = false;
                    r->startTimer();
                    r->setLifespan(pileJoueur->getLifespan());
                    pickups.push_back(r);
                }
            }//fin traitement des piles
            if (pickups[i]->OnTriggerEnter(other) == -1){
                Interactable2D* truc = pickups[i];
                pickups.erase(pickups.begin()+i);
                delete truc;
            }//par défaut (si -1 alors supprimer)
            return true;
        }
    }

    for (int i = 0; i <  entities.size(); i++){
        if(entities[i]->canCollide && entities[i]->getCollider().TestCollision(other->getCollider())){
            if (entities[i]->OnTriggerEnter(other) == -1){
                Movable* truc = entities[i];
                entities.erase(entities.begin()+i);
                delete truc;
            }
            return true;
        }
    }
    return false;
}


bool Room::IsPointInCircle(QVector2D *pt, QVector2D *center, float rayon)
{   float distPlayerToEnnemi = sqrt( pow(pt->x() - center->x(),2) + pow(pt->y() - center->y(),2) );
    if( distPlayerToEnnemi < rayon )
        return true;
    else
        return false;
}

bool Room::CheckColl(float rayon, float angle, QVector2D point)
{
    // Position du joueur in screen
    QVector2D center = QVector2D(player->GetPosition().x(),player->GetPosition().y());
    if( !IsPointInCircle(&point, &center, rayon) )
        return false;
    else
    {
        // vectDirecteur de la lampe
        QVector2D vectDirect = player->GetDirection();
        vectDirect.normalize();
        vectDirect.setX( -vectDirect.x()*3 );
        vectDirect.setY( vectDirect.y()*3 );
        //std::cout<<"vectDirect = "<<vectDirect.x()<<" "<<vectDirect.y()<<std::endl;
        // Vecteur qui va du joueur -> ennemi
        QVector2D center_ennemi = QVector2D(point.x() - center.x() , point.y() - center.y());
        //std::cout<<"center_ennemi = "<<center_ennemi.x()<<" "<<center_ennemi.y()<<std::endl;

        float produitScalaire = (vectDirect.x() * center_ennemi.x()) + (vectDirect.y() * center_ennemi.y());
        float produitNorme = vectDirect.length() * center_ennemi.length();
        float cosTeta = produitScalaire / produitNorme;
        float angleRadian = acos(cosTeta);
        float angleDegree = angleRadian*(180/3.14159265358979323846); // radian to degree
        //std::cout<<"Radian = "<<angleRadian<<std::endl;
        //std::cout<<"Degree = "<<angleDegree<<std::endl;
        if ( angleDegree < angle/2 ) return true;
        else return false;
    }
}

void Room::affectEnemiesInRange(){
    float rayon;
    float angle;
    bool isUsingMainLamp = false;
    bool isUsingSecondLamp = false;

    if (player->utilisePilePrincipale()){
        rayon = player->GetPilePrincipale()->GetRange();
        angle = player->GetPilePrincipale()->GetConeAngle();
        isUsingMainLamp = true;
        //std::cout << "utilise pile principale " << std::endl;
    }
    else if (player->utilisePileSecondaire()){
        rayon = player->getPileSecondaire()->GetRange();
        angle = player->getPileSecondaire()->GetConeAngle();
        isUsingSecondLamp = true;
    }

    if (isUsingSecondLamp || isUsingMainLamp)
        for (int i = 0; i < entities.size(); i++){
            if (CheckColl(rayon,angle,QVector2D(entities[i]->GetPosition().x(),entities[i]->GetPosition().y())))
            {
                if (isUsingMainLamp){
                 player->GetPilePrincipale()->Affect(entities[i]);
                }
                else {
                   player->getPileSecondaire()->Affect(entities[i]);
                }
            }
        }
}
