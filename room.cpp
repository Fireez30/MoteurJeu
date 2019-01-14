#include "room.h"
#include <iostream>
#include "rangedpile.h"
#include "ennemi.h"
#include <math.h>
#include "largerpile.h"
#include "key.h"
#include "boss_torche.h"
#include "torche.h"
#include "turretennemy.h"
#include "tinyxml2.h"

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
    for (unsigned i = 0; i < pickups.size();i++){
        delete pickups[i];
    }
    pickups.clear();

    for (unsigned i = 0; i < entities.size(); i++){
        delete entities[i];
    }
    for (unsigned i = lights->size(); i < 1; i--){
        delete lights->at(i);
    }
    entities.clear();
}

bool Room::isThisRoom(int _x, int _y){
    return x == _x && y == _y;
}

void Room::Turn(int i){
    float x = entities[i]->GetLastMove().x();
    float y = entities[i]->GetLastMove().y();
    QVector2D result = -entities[i]->GetLastMove();
      // QVector2D result;
    if (x >= 0){
        if (y >= 0)
        {
            result += QVector2D(0,-1.5);
        }
        else {
            result += QVector2D(1.5,0);
        }
    }
    if (x <= 0 ){
        if (y >= 0)
        {
            result += QVector2D(1.5,0);
        }
        else {
            result += QVector2D(-1.5,0);
        }
    }
    result.normalize();
    result *= entities[i]->GetSpeed();
    entities[i]->Move(result);
}

void Room::UpdateEntities(){
    for (int i = 0; i < entities.size(); i++){
        entities[i]->Update();
       // std::cout << "Vie de lentite : " <<i << " : " << entities[i]->getHealth() << std::endl;
        //std::cout << "Entity " << i << " timer ? " << entities[i]->isTimerActive() << std::endl;
        if (CollisionCheck(entities[i]->getCollider())){//si l'entité a collide avec un mur, reset sa position
            entities[i]->ResetMove();
            Turn(i);
            //entities[i]->Move((entities[i]->GetLastMove()+QVector2D(0,1))*0.0166);
        }
        if (entities[i]->isDead()){
            std::cout << " ENTITE MORTE !!!!!!!!!!!!!" << std::endl;
            //Movable* truc = entities[i];
            entities.erase(entities.begin()+i);
            //delete truc;
            std::cout << "APRES DELETE" << std::endl;
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

    tinyxml2::XMLElement* d4 = doc2->FirstChildElement("Entite");
    if (d4){//si la salle que l'on lit contient une entité

        for (tinyxml2::XMLElement* e4 = d4->FirstChildElement("RangedPile"); e4 != nullptr; e4 = e4->NextSiblingElement("RangedPile")){//Liste des piles
            RangedPile *r = new RangedPile(p,QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),e4->FloatAttribute("range"),e4->FloatAttribute("coneangle"),e4->IntAttribute("lifetime"),e4->IntAttribute("damage"),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0));
            r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
            pickups.push_back(r);
        }
        for (tinyxml2::XMLElement* e4 = d4->FirstChildElement("LargerPile"); e4 != nullptr; e4 = e4->NextSiblingElement("LargerPile")){//Liste des piles
            LargerPile *r = new LargerPile(p,QVector2D(e4->IntAttribute("x")/16.0+xRoom,(-1*e4->IntAttribute("y")/16.0)+yRoom),e4->FloatAttribute("range"),e4->FloatAttribute("coneangle"),e4->IntAttribute("lifetime"),e4->IntAttribute("damage"),QVector2D(e4->IntAttribute("xtextcoord")/16.0,e4->IntAttribute("ytextcoord")/16.0));
            r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
            pickups.push_back(r);
        }
        for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Ghost"); e5 != nullptr; e5 = e5->NextSiblingElement("Ghost")){//Liste des Ghosts
            Ennemi* e =new Ennemi(this,p,e5->IntAttribute("vie"),e5->IntAttribute("directionx"),e5->IntAttribute("directiony"),e5->FloatAttribute("vitesse"),e5->IntAttribute("damagecd"),QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),e5->IntAttribute("animtime"),e5->IntAttribute("nbFrames"),false);
            e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
            entities.push_back(e);
        }//fin for piles, rajouter des fors pour les autres entités

        for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("TurretEnnemi"); e5 != nullptr; e5 = e5->NextSiblingElement("TurretEnnemi")){//Liste des Ghosts
            int tir = e5->IntAttribute("targetplayer");
            //std::cout << "tirate" << e5->DoubleAttribute("shootcooldown") << std::endl;
            TurretEnnemi* e =new TurretEnnemi(this,p,e5->IntAttribute("vie"),e5->IntAttribute("directionx"),e5->IntAttribute("directiony"),e5->FloatAttribute("vitesse"),e5->IntAttribute("damagecd"),QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),e5->IntAttribute("animtime"),e5->IntAttribute("nbFrames"),false,(tir == 1),e5->FloatAttribute("shootcooldown"),e5->FloatAttribute("projspeed"),e5->IntAttribute("projtime"));
            e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
            entities.push_back(e);
        }

        for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Key"); e5 != nullptr; e5 = e5->NextSiblingElement("Key")){//Liste des Key
            //float x = (float)e5->IntAttribute("x"), y = (float)(-1*e5->IntAttribute("y"));
            Key* e =new Key(p,QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),false, QVector2D(e5->IntAttribute("xatltext")/16.0,e5->IntAttribute("yalttext")/16.0));
            e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
            pickups.push_back(e);
        }//si clé

        for (tinyxml2::XMLElement* e3 = d4->FirstChildElement("BossDoor"); e3 != nullptr; e3 = e3->NextSiblingElement("BossDoor")){//y
            float x = (float)e3->IntAttribute("x"), y = (float)(-1*e3->IntAttribute("y"));
            QVector2D(dir);
            int dirx = e3->IntAttribute("dirx");
            int diry = e3->IntAttribute("diry");
            dir.setX(dirx);
            dir.setY(diry);
            Door* d = new Door(QVector2D(x/16.0+xRoom,y/16.0+yRoom),QVector2D(e3->IntAttribute("xtextcoord")/16.0,e3->IntAttribute("ytextcoord")/16.0),QVector2D(e3->IntAttribute("xalttext")/16.0,e3->IntAttribute("yalttext")/16.0),!player->getHoldKey(),dir,p,c);
            d->setCollider(Hitbox(QVector2D(d->position.x(),d->position.y()),1,1));//porte ont un collider spécial
            pickups.push_back(d);
            boss = d;
        }

        for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Boss_torche"); e5 != nullptr; e5 = e5->NextSiblingElement("Boss_torche")){//Liste des Boss_torche
            Boss_torche* e =new Boss_torche(this,p,e5->IntAttribute("health"),e5->IntAttribute("directionx"),e5->IntAttribute("directiony"),e5->IntAttribute("speed"),e5->IntAttribute("damagecd"),QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom),QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),e5->IntAttribute("animtime"),e5->IntAttribute("nbFrames"),false);
            e->setCollider(Hitbox(QVector2D(e->position.x(),e->position.y()),1,1));
            entities.push_back(e);
            boss2 = e;
        }//fin for piles, rajouter des fors pour les autres entités

        for (tinyxml2::XMLElement* e5 = d4->FirstChildElement("Torche"); e5 != nullptr; e5 = e5->NextSiblingElement("Torche")){//Liste des Torches
            int trange = e5->IntAttribute("range");
            Torche* e =new Torche(QVector2D(e5->IntAttribute("x")/16.0+xRoom,(-1*e5->IntAttribute("y")/16.0)+yRoom), trange, e5->IntAttribute("damage"), QVector2D(e5->IntAttribute("xtextcoord")/16.0,e5->IntAttribute("ytextcoord")/16.0),this);
            e->setCollider(Hitbox(QVector2D(e->position.x()-trange/2,e->position.y()+trange/2),trange,trange));
            pickups.push_back(e);
        }//si clé lol
    }
    doc.Clear();//vider le doc
}

void Room::Render(QOpenGLShaderProgram *program,QOpenGLTexture *text){
    if(lights->size() > 2)
        lights->erase(lights->begin()+1,lights->begin()+lights->size()-1);
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
            return true;
        }
    }
    return false;
}
bool Room::TriggerCheck(Interactable2D* other){//collisions portes et entités
    bool flag = false;
    int triggerCount = 0;
    for(int i=0;i<pickups.size();i++){
        if(pickups[i]->canCollide && pickups[i]->getCollider().TestCollision(other->getCollider())){
            triggerCount++;
            Pile* pile = dynamic_cast<Pile*> (pickups[i]);
            Player* p= dynamic_cast<Player*> (other);
            if(pile != nullptr && p != nullptr){
                std::cout<< "Colision pile" << std::endl;
                int idPile = -1;
                Pile * pileJoueur = p->getPileSecondaire() ;
                if(pileJoueur!= nullptr){
                    idPile = pileJoueur->getID();
                }
                if(idPile==0){
                    RangedPile *r = new RangedPile(p,QVector2D(pile->position.x(),pile->position.y()),pileJoueur->renderer.GetTextCoords());
                    r->setCollider(Hitbox(QVector2D(r->position.x(),r->position.y()),1,1));
                    r->renderer.CreateGeometry();
                    r->canCollide = false;
                    r->startTimer();
                    r->setLifespan(pileJoueur->getLifespan());
                    pickups.push_back(r);
                }
                else if(idPile==1){
                    LargerPile *r = new LargerPile(p,QVector2D(pile->position.x(),pile->position.y()),pileJoueur->renderer.GetTextCoords());
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
            flag = true;
        }
    }
    for (int i = 0; i <  entities.size(); i++){
        for (unsigned j = 0; j < entities[i]->getProjectiles().size(); j++){
            if (entities[i]->getProjectiles()[j]->getCollider().TestCollision(other->getCollider())){
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

bool Room::wallOnTheVector(QVector2D vect, QVector2D player,QVector2D ennemi){
    for (unsigned i = 0; i < collisions.size(); i++){
        QVector2D p_to_wall = QVector2D(collisions[i].getCorner().x() - player.x() , collisions[i].getCorner().y() - player.y());
        if(vect.lengthSquared() > p_to_wall.lengthSquared()){
            QVector2D up = QVector2D(collisions[i].getXMin(),collisions[i].getYMin());
            QVector2D down = QVector2D(collisions[i].getXMax(),collisions[i].getYMax());
            if(player.x() != ennemi.x()){
                float a1 = (player.y()-ennemi.y())/(player.x()-ennemi.x());
                float a2 = (up.y()-down.y())/(up.x()-down.x());
                if(a1 != a2){   //Si segment pas parralèle
                    float b1 = player.y()-a1*player.x();
                    float b2 = up.y()-a2*up.x();
                    float xa = (b2-b1)/(a1-a2);
                    if(!(xa < std::max(std::min(player.x(),ennemi.x()),std::min(up.x(),down.x())) || xa > std::min(std::max(player.x(),ennemi.x()),std::max(up.x(),down.x()))))
                        return true;
                    else{
                        up = QVector2D(collisions[i].getXMax(),collisions[i].getYMin());
                        down = QVector2D(collisions[i].getXMin(),collisions[i].getYMax());
                        float a1 = (player.y()-ennemi.y())/(player.x()-ennemi.x());
                        float a2 = (up.y()-down.y())/(up.x()-down.x());
                        if(a1 != a2){   //Si segment pas parralèle
                            float b1 = player.y()-a1*player.x();
                            float b2 = up.y()-a2*up.x();
                            float xa = (b2-b1)/(a1-a2);
                            if(!(xa < std::max(std::min(player.x(),ennemi.x()),std::min(up.x(),down.x())) || xa > std::min(std::max(player.x(),ennemi.x()),std::max(up.x(),down.x()))))
                                return true;
                        }
                    }
                }
            }
        }
    }
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
       // Vecteur qui va du joueur -> ennemi
        QVector2D center_ennemi = QVector2D(point.x() - center.x() , point.y() - center.y());
        float produitScalaire = (vectDirect.x() * center_ennemi.x()) + (vectDirect.y() * center_ennemi.y());
        float produitNorme = vectDirect.length() * center_ennemi.length();
        float cosTeta = produitScalaire / produitNorme;
        float angleRadian = acos(cosTeta);
        float angleDegree = angleRadian*(180/3.14159265358979323846); // radian to degree
        if ( angleDegree <= angle ) {
            return !wallOnTheVector(center_ennemi,center, point);                                 //vérifier ca que si l'ennemi est dans le cone (economie de temps)
        }
        return false;
    }
}

void Room::affectEnemiesInRange(){
    float rayon = player->getRange();
    float angle = player->getAngle();
    for (int i = 0; i < entities.size(); i++){
        if (CheckColl(rayon,angle,QVector2D(entities[i]->GetPosition().x(),entities[i]->GetPosition().y())))
        {
            entities[i]->setAffected(true);
            player->getPileEnCours()->Affect(entities[i]);

        }
        else{
            entities[i]->setAffected(false);
        }
    }
}
