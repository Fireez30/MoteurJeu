#include "boundingbox.h"

BoundingBox::BoundingBox(QVector3D p, int h, int w, int t): leftCorner(p.x(),p.y(),p.z()),height(h),width(w),type(t){
}

void BoundingBox::Translate(QVector3D q){
    leftCorner += q;
    xmin = leftCorner.x();
    ymin = leftCorner.y();
    xmax = xmin + width;
    ymax = ymin + height;
}

bool BoundingBox::CollisionTest(BoundingBox b){//basic collision
    return (!(xmax < b.xmin || xmin > b.xmax || ymax < b.ymin || ymin > b.ymax));
}

