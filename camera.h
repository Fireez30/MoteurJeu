#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Camera{

public :
    Camera();
    void moveCamera(QVector3D trans);
    QVector3D getPosition();

private :
    QVector3D position;
};

#endif // CAMERA_H
