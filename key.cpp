#include "key.h"
#include "player.h"
#include <iostream>

Key::Key(Player* p,QVector2D pos,QVector2D text):Interactable2D (pos,text,0),picked(false),player(p),altText(2,1){

}

Key::Key(Player* p,QVector2D pos,QVector2D text, bool state, QVector2D d): Interactable2D (pos,text,0),picked(state),altText(d),player(p){

}

int Key::OnTriggerEnter(Interactable2D* other){
    if (!picked){
        if(dynamic_cast<Player*> (other)!=NULL){
            picked = true;
            std::cout << "init text = " << this->renderer.spriteCoords.x() << " " << this->renderer.spriteCoords.y() << std::endl;
            this->renderer.spriteCoords = altText;
            this->renderer.CreateGeometry();
            std::cout << "final text = " << this->renderer.spriteCoords.x() << " " << this->renderer.spriteCoords.y() << std::endl;
            player->PickKey();
        }
    }
}
