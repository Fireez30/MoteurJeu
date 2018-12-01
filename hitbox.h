#ifndef HITBOX_H
#define HITBOX_H

#include <QVector2D>

class Hitbox {
protected :
    QVector2D leftCorner;
    int width;
    int height;
public :
    Hitbox();
    Hitbox(QVector2D p,int w,int h);
    bool TestCollision(Hitbox other);
};

#endif // HITBOX_H
