#include "lightsource.h"

LightSource::LightSource(QVector2D pos, QVector3D c, float at, float ambient, float angle, float angleMax, QVector3D dir, float d, float maxD):
    position(pos),color(c),attenuation(at),ambientCoefficient(ambient),coneAngle(angle),maxAngle(angleMax),direction(dir),dist(d),maxDist(maxD){}

LightSource::~LightSource(){

}
