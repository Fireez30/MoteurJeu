#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QVector3D>

class BoundingBox {
public :
    QVector3D leftCorner;
    int height;
    int width;
    int type; //0 = no collision, 1 = collision
    int xmax,xmin,ymax,ymin;
    BoundingBox(QVector3D p,int h, int w, int t = 0);

    void Translate(QVector3D q);
    bool CollisionTest(BoundingBox b);
}

#endif // BOUNDINGBOX_H
