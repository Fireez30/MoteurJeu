#ifndef SPRITEMODIFIER_H
#define SPRITEMODIFIER_H

#include "interactable2d.h"
#include <vector>

class SpriteModifier {

protected :
    std::vector<QVector2D> sprites;
    Interactable2D* Object;

public :
    SpriteModifier(Interactable2D* obj);
    void AddSprite(QVector2D spr);
    void ChangeSprite(int index);
    int nbOfSprites();
    QVector2D GetSprite(int i);
};

#endif // SPRITEMODIFIER_H
