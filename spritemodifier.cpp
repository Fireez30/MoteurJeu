#include "spritemodifier.h"
#include <iostream>

SpriteModifier::SpriteModifier(Interactable2D* obj):Object(obj){

}

//contient une liste de sprites
//passer a une machine a état (std::map avec string) quand on a le temps
void SpriteModifier::AddSprite(QVector2D spr){
    for (unsigned i = 0; i < sprites.size(); i++){
        if (sprites[i] == spr){
            return;
        }
    }

    sprites.push_back(spr);
}

//change le sprite de l'objet auquel il est rattaché
void SpriteModifier::ChangeSprite(int index){
    Object->renderer.SetYSpriteCoord(sprites[index].y());
    Object->renderer.CreateGeometry();
}


int SpriteModifier::nbOfSprites(){
    return sprites.size();
}

QVector2D SpriteModifier::GetSprite(int i){
    if (i < sprites.size()){
        return sprites[i];
    }
}
