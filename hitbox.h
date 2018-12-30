#ifndef HITBOX_H
#define HITBOX_H

#include <QVector2D>

class Hitbox {

protected :
    QVector2D leftCorner;
    float width;
    float height;
public :
    Hitbox();
    Hitbox(QVector2D p,float w,float h);
    bool TestCollision(Hitbox other);
    float getXMax();
    QVector2D getCorner();
    float getXMin();
    float getYMax();
    float getYMin();
    void setPoint(QVector2D p);
};

#endif // HITBOX_H
