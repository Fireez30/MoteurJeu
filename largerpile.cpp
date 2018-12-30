#include "largerpile.h"

LargerPile::LargerPile(QVector2D pos,QVector2D text):Pile(pos,7,90,90,1,text,0,QVector3D(0,3,0)){
}

LargerPile::LargerPile(QVector2D pos,float r,float c,float l,int d,QVector2D text):Pile(pos,r,c,l,d,text,0,QVector3D(0,3,0)){

}

void LargerPile::Affect(Movable *m){
     m->ChangeSpeed(m->GetSpeed()*0.5);
}
