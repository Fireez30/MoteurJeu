#include "room.h"
#include <iostream>
#include "rangedpile.h"
#include "ennemi.h"
#include <math.h>
#include "largerpile.h"
#include "key.h"
#include "boss_torche.h"
#include "torche.h"

Room::Room(std::vector<LightSource*>* l):lights(l){
    tiles = std::vector<Tile>();
    collisions  = std::vector<Hitbox>();
    pickups = std::vector<Interactable2D*>();
    entities = std::vector<Movable*>();
    player = nullptr;
    boss = nullptr;
    boss2 = nullptr;
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
            std::cout << "collision ennemi mur" << std::endl;
            entities[i]->ResetMove();
            //entities[i]->Move((entities[i]->GetLastMove()+QVector2D(0,1))*0.0166);
        }
    }
    if (boss2 != nullptr){
        TriggerCheck(boss2);
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
    if (boss != nullptr && player->getHoldKey()){
        boss->Unlock();
        //std::cout << "unlocked " << std::endl;
    }

    for (int i = 0; i < tiles.size(); i++){
        tiles[i].renderer.CreateGeometry();
    }

    for(int i=0;i<pickups.size();i++){
        pickups[i]->renderer.CreateGeometry();
    }

    for (int i = 0 ; i < entities.size(); i++){
        entities[i]->ResetPos();
        entities[i]->CreateProjectileGeometry();
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
        Door* d = new Door(QVector2D(x+xRoom,y+yRoom),QVector2D(e3->IntAttribute("tx")/16.0,e3->IntAttribute("ty")/16.0),QVector2D(e3->IntAttribute("tx")/16.0,e3->IntAttribute("ty")/16.0),false,dir,p,c);
        d->setCollider(Hitbox(QVector2D(d->position.x(),d->position.y()),1,1));//porte ont un collider spécial
        pickups.push_back(d);
    }//Fin construction doors !
//Fin construction doors !

        tinyxml2::XMLElement* d4 = doc2->FirstChildElement("Entite");
        if (d4){//si la salle que l'on lit contient une entité

            for (tinyxml2::XMLElement* e4 = d4->FirstChildElement("RangedPile"); e4 != nullptr; e4 = e4->NextSiblingElement("RangedPile")){//Liste des piles
            RangedPile *r = new RangedPile(QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0));
            r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
            pickups.push_back(r);
            }
            for (tinyxml2::XMLElement* e4 = d4->FirstChildElement("LargerPile"); e4 != nullptr; e4 = e4->NextSiblingElement("LargerPile")){//Liste des piles
            LargerPile *r = new LargerPile(QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0));
            r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
            pickups.push_back(r);
            }
                //for (int i = 0; i < interacts.size(); i++){
                //    std::cout << "interact at : " << i << " xcord = " << interacts[i]->GetPosition().x() << " ycord = " << interacts[i]->GetPosition().y() <<  std::endl;
                //}
            //fin for piles, rajouter des fors pour les autres entités

            for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Ghost"); e5 != nullptr; e5 = e5->NextSiblingElement("Ghost")){//Liste des Ghosts
                Ennemi* e =new Ennemi(this,p,10,0,0,2.5,QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),200,3,false);
                e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
                entities.push_back(e);
                //for (int i = 0; i < interacts.size(); i++){
                //    std::cout << "interact at : " << i << " xcord = " << interacts[i]->GetPosition().x() << " ycord = " << interacts[i]->GetPosition().y() <<  std::endl;
                //}
            }//fin for piles, rajouter des fors pour les autres entités

            for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Key"); e5 != nullptr; e5 = e5->NextSiblingElement("Key")){//Liste des Key
                //float x = (float)e5->IntAttribute("x"), y = (float)(-1*e5->IntAttribute("y"));
                Key* e =new Key(p,QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),false, QVector2D(e5->IntAttribute("xatltext")/16.0,e5->IntAttribute("yalttext")/16.0));
                e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
                pickups.push_back(e);
                //std::cout << "clé cree lol " << std::endl;
                //for (int i = 0; i < interacts.size(); i++){
                //    std::cout << "interact at : " << i << " xcord = " << interacts[i]->GetPosition().x() << " ycord = " << interacts[i]->GetPosition().y() <<  std::endl;
                //}
            }//si clé lol

            for (tinyxml2::XMLElement* e3 = d4->FirstChildElement("BossDoor"); e3 != nullptr; e3 = e3->NextSiblingElement("BossDoor")){//y
                float x = (float)e3->IntAttribute("x"), y = (float)(-1*e3->IntAttribute("y"));
                QVector2D dir;
                dir.setY(-1);
                Door* d = new Door(QVector2D(x/16.0+xRoom,y/16.0+yRoom),QVector2D(e3->IntAttribute("xtextcoord")/16.0,e3->IntAttribute("ytextcoord")/16.0),QVector2D(e3->IntAttribute("xalttext")/16.0,e3->IntAttribute("yalttext")/16.0),!player->getHoldKey(),dir,p,c);
                d->setCollider(Hitbox(QVector2D(d->position.x(),d->position.y()),1,1));//porte ont un collider spécial
                pickups.push_back(d);
                boss = d;
            }

            for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Boss_torche"); e5 != nullptr; e5 = e5->NextSiblingElement("Boss_torche")){//Liste des Boss_torche
                Boss_torche* e =new Boss_torche(this,p,e5->IntAttribute("health"),0,0,e5->IntAttribute("speed"),QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),200,2,false);
                e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
                entities.push_back(e);
                boss2 = e;
            }//fin for piles, rajouter des fors pour les autres entités

            for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Torche"); e5 != nullptr; e5 = e5->NextSiblingElement("Torche")){//Liste des Torches
                int trange = e5->IntAttribute("range");
                Torche* e =new Torche(QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom), trange, 1, QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),this);
                e->setCollider(Hitbox(QVector2D(e->position.x()-trange/2,e->position.y()+trange/2),trange,trange));
                pickups.push_back(e);
            }//si clé lol
        }
    doc.Clear();//vider le doc
}

void Room::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
   lights->erase(lights->begin()+2,lights->begin()+lights->size()-1);
    for (int i = 0; i < pickups.size(); i++){
        pickups[i]->Render(program,text);//contient piles + portes
        if(dynamic_cast<Pile*>(pickups[i])!=nullptr)
            lights->push_back(dynamic_cast<Pile*>(pickups[i])->getLightSource());
    }
    for (int i = 0; i < entities.size(); i++){
        entities[i]->Render(program,text);//contient entités
        entities[i]->RenderProjectile(program,text);
    }
    for (int i = 0; i < tiles.size(); i++){
        tiles[i].Render(program,text);//contient sol + murs
    }
}

bool Room::CollisionCheck(Hitbox h){//collisions des murs unqiuements
    for (int i = 0; i < collisions.size();i++){
        if (collisions[i].TestCollision(h)){
            //std::cout << "collisionChecktrue" << std::endl;
            return true;
        }
    }
   // std::cout << "collisionCheck false" << std::endl;
    return false;
}
bool Room::TriggerCheck(Interactable2D* other){//collisions portes et entités
    bool flag = false;
    int triggerCount = 0;
    for(int i=0;i<pickups.size();i++){
        if(pickups[i]->canCollide && pickups[i]->getCollider().TestCollision(other->getCollider())){
            triggerCount++;
            //std::cout << "triggercheck " << std::endl;
            Pile* pile = dynamic_cast<Pile*> (pickups[i]);
            Player* p= dynamic_cast<Player*> (other);
            if(pile != nullptr && p != nullptr){
                std::cout << "triggercheck if pile" << std::endl;
                int idPile = -1;
                Pile * pileJoueur = p->getPileSecondaire() ;
                if(pileJoueur!= nullptr)
                    idPile = pileJoueur->getID();
                std::cout << idPile << std::endl;
                if(idPile==0){
                    std::cout << "drop" << std::endl;
                    RangedPile *r = new RangedPile(QVector2D(pile->position.x(),pile->position.y()),pileJoueur->renderer.GetTextCoords());
                    r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
                    r->renderer.CreateGeometry();
                    r->canCollide = false;
                    r->startTimer();
                    r->setLifespan(pileJoueur->getLifespan());
                    pickups.push_back(r);
                }
                else if(idPile==1){
                     std::cout << "drop" << std::endl;
                    LargerPile *r = new LargerPile(QVector2D(pile->position.x(),pile->position.y()),pileJoueur->renderer.GetTextCoords());
                    r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
                    r->renderer.CreateGeometry();
                    r->canCollide = false;
                    r->startTimer();
                    r->setLifespan(pileJoueur->getLifespan());
                    pickups.push_back(r);
                }
            }//fin traitement des piles
            if (pickups[i]->OnTriggerEnter(other) == -1){
                std::cout << "suprresion" << std::endl;
                Interactable2D* truc = pickups[i];
                pickups.erase(pickups.begin()+i);
                delete truc;
            }//par défaut (si -1 alors supprimer)
        flag = true;
        }
    }
    //std::cout << "Nb triggers = " << triggerCount << std::endl;
    //std::cout << "Nb interactable " << pickups.size() << std::endl;
    for (int i = 0; i <  entities.size(); i++){
        for (unsigned j = 0; j < entities[i]->getProjectiles().size(); j++){
            if (entities[i]->getProjectiles()[j]->getCollider().TestCollision(other->getCollider())){
                //std::cout << "Collision projectile ! " << std::endl;
                entities[i]->getProjectiles()[j]->OnTriggerEnter(other);
                flag = true;
            }
        }

        if(entities[i]->canCollide && entities[i]->getCollider().TestCollision(other->getCollider())){
            flag = true;
            if (entities[i]->OnTriggerEnter(other) == -1){
                Movable* truc = entities[i];
                entities.erase(entities.begin()+i);
                delete truc;
            }
        }
    }//fin collision entité
    return flag;
}


bool Room::IsPointInCircle(QVector2D *pt, QVector2D *center, float rayon)
{
    float distPlayerToEnnemi = sqrt( pow(pt->x() - center->x(),2) + pow(pt->y() - center->y(),2) );
    if( distPlayerToEnnemi < rayon )
        return true;
    else
        return false;
}

bool Room::wallOnTheVector(QVector2D vect){
    for (unsigned i = 0; i < collisions.size(); i++){
        QVector3D vec1 = QVector3D(collisions[i].getCorner().x() - player->GetPosition().x(),collisions[i].getCorner().y() - player->GetPosition().y(),0);//joueur -> mur
        //std::cout << "Joueur vers mur : " << vec1.x() << " " << vec1.y() << std::endl;
        QVector3D vec2 = QVector3D((player->GetPosition() + vect).x() - player->GetPosition().x(),(player->GetPosition() + vect).y() - player->GetPosition().y(),0);//joueur -> ennemi
        //std::cout << "Joueur vers ennemi : " << vec2.x() << " " << vec2.y() << std::endl;
        QVector3D cp = QVector3D::crossProduct(vec1,vec2);
        //std::cout << "CP = " << cp.x() << " " << cp.y() << " " << std::endl;
        if ((cp.x() < 0.001 && cp.x() > -0.001) && (cp.y() < 0.001 && cp.y() > -0.001) && vec1.length() < vec2.length()){ //si ils sont colinéaires,conflit
            //std::cout<< "Collision lampe mur" << std::endl;
            return true;
        }
    }
    //std::cout<< "pas Collision lampe mur" << std::endl;
    return false;
}

bool Room::CheckColl(float rayon, float angle, QVector2D point)
{
    // Position du joueur in world
    QVector2D center = QVector2D(player->GetPosition().x(),player->GetPosition().y());
    if( !IsPointInCircle(&point, &center, rayon) )
        return false;
    else
    {
        // vectDirecteur de la lampe
        QVector2D vectDirect = player->GetDirection();
        vectDirect.normalize();
        vectDirect.setX( -vectDirect.x()*100 );
        vectDirect.setY( vectDirect.y()*100 );
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
        if ( angleDegree < angle/2 ) {
            //std::cout << "collision ennemi possible " << std::endl;
            return !wallOnTheVector(center_ennemi);                                 //vérifier ca que si l'ennemi est dans le cone (economie de temps)
        }
        else return false;
    }
}

void Room::affectEnemiesInRange(){
    float rayon = player->getRange();
    //std::cout << "Rayon affect : " << rayon << std::endl;
    float angle = player->getAngle();
    //std::cout << "Angle affect : " << angle << std::endl;
    //bool isUsingMainLamp = false;
    bool isUsingSecondLamp = false;

    if (player->utilisePileSecondaire()){
        isUsingSecondLamp = true;
    }
        for (int i = 0; i < entities.size(); i++){
            if (CheckColl(rayon,angle,QVector2D(entities[i]->GetPosition().x(),entities[i]->GetPosition().y())))
            {
                //std::cout << "Debut collision lampe ennemie" << std::endl;
                entities[i]->setAffected(true);

                if (isUsingSecondLamp){
                    player->getPileSecondaire()->Affect(entities[i]);
                }
                else {
                    player->GetPilePrincipale()->Affect(entities[i]);
                }
                //std::cout << "avant change speed" << std::endl;
                for (unsigned j = 0; j < entities[i]->getProjectiles().size(); j++){
                    entities[i]->getProjectiles()[j]->changeSpeed(0.5);
                }
                //std::cout << "Fin collision lampe ennemie" << std::endl;
            }
            else{
                entities[i]->setAffected(false);
            }

        }
}
