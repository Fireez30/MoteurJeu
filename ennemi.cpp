#include "ennemi.h"

Ennemi::Ennemi(QVector2D pos, QVector2D dir, QVector2D text, int life, int v, int cdTrigger, int animTime, int nbFrame, bool animStatus):Movable (life,dir.x(),dir.y(),v,cdTrigger,pos,text,animTime, nbFrame, animStatus){

}
