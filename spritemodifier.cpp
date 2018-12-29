#include "spritemodifier.h"

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
}
