#include "key.h"
#include "player.h"
#include <iostream>
#include <QtMultimedia/QMediaPlayer>

Key::Key(Player* p,QVector2D pos,QVector2D text):Interactable2D (pos,text,0),picked(false),player(p),altText(2,1){
    splayer = new QMediaPlayer;
    splayer->setMedia(QUrl::fromLocalFile("key.wav"));
    splayer->setVolume(50);

}

Key::Key(Player* p,QVector2D pos,QVector2D text, bool state, QVector2D d): Interactable2D (pos,text,0),picked(state),altText(d),player(p){
    splayer = new QMediaPlayer;
    splayer->setMedia(QUrl::fromLocalFile("key.wav"));
    splayer->setVolume(50);
}

//on pickup par le joueur, jouer son, changer de sprite et donner la clé au joueur
int Key::OnTriggerEnter(Interactable2D* other){
    if (!picked){
        if(dynamic_cast<Player*> (other)!=NULL){
            picked = true;//empeche de collide plusieurs fois
            splayer->play();//son
            this->renderer.spriteCoords = altText;
            this->renderer.CreateGeometry();
            player->PickKey();//file la clé
        }
    }
}
