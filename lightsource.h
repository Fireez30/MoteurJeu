#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QVector2D>
#include <QVector3D>

class LightSource{

public :
    QVector2D position;
    QVector3D color, direction;
	float attenuation, ambientCoefficient;
	float coneAngle,maxAngle;
	float dist, maxDist;

    LightSource(QVector2D pos, QVector3D c, float at, float ambient, float angle, float angleMax, QVector3D dir, float d, float maxD);
};

#endif // LIGHTSOURCE_H
