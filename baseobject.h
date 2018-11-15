#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>
#include "geometryengine.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

using namespace std;

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

class BaseObject : protected QOpenGLFunctions
{

protected:
    QQuaternion rotation;
    QVector3D position;
    QQuaternion realRotation;
    QVector3D realPosition;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    vector<BaseObject> childs;
    BaseObject* parent;
    int lod;
    string meshFile;
    int meshSize;
public :
    static int id;

    BaseObject();
    BaseObject(QQuaternion rot,QVector3D geo);
    ~BaseObject();
    void SetRotation(QQuaternion r);
    QQuaternion GetRotation();
    void SetPosition(QVector3D p);
    QVector3D GetPosition();
    void Rotate(QQuaternion r);
    void Translate(QVector3D v);
    void SetChilds(vector<BaseObject> v);
    vector<BaseObject> GetChilds();
    BaseObject GetChildAtIndex(unsigned i);
    BaseObject* GetParent();
    void chooseLOD(QVector3D cam);
    void SetParent(BaseObject* b);
    void SetChildAtIndex(BaseObject c, unsigned i);
    void CreateGeometry(QVector3D cam);
    void Render(QOpenGLShaderProgram *program);
    void UpdatePositionInSpace();
};

#endif // BASEOBJECT_H
