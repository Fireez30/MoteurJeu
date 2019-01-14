#ifndef KEY_H
#define KEY_H

#include "interactable2d.h"
#include "player.h"

class Key : public Interactable2D{

protected:
    bool picked;
    QVector2D altText;
    Player* player;
    QMediaPlayer *splayer;
public :
    Key(const Key&k);
    Key(Player* p,QVector2D pos,QVector2D text);
    Key(Player* p,QVector2D pos,QVector2D text, bool state, QVector2D d);
    int OnTriggerEnter(Interactable2D* other) override;
    void setTrueText(QVector2D ttext);
    void setAltText(QVector2D atext);
};

#endif
// KEY_H
