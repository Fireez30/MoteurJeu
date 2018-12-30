#include "spritemodifier.h"
#include <iostream>

SpriteModifier::SpriteModifier(Interactable2D* obj):Object(obj){

}
void SpriteModifier::AddSprite(QVector2D spr){
    for (unsigned i = 0; i < sprites.size(); i++){
        if (sprites[i] == spr){
            return;
        }
    }

    sprites.push_back(spr);
}

void SpriteModifier::ChangeSprite(int index){
    Object->renderer.spriteCoords = sprites[index];
    Object->renderer.initText = sprites[index];
    Object->renderer.CreateGeometry();
   // std::cout << "(changesprite) Renderer: " << Object->renderer.spriteCoords.x() << " " << Object->renderer.spriteCoords.y() << std::endl;
}

int SpriteModifier::nbOfSprites(){
    return sprites.size();
}

QVector2D SpriteModifier::GetSprite(int i){
    if (i < sprites.size()){
        return sprites[i];
    }
}
