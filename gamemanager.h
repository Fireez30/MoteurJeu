#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTime>
#include "room.h"
#include "player.h"

class GameManager : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    QElapsedTimer et;
    QTime m_time;
    int m_frameCount;
    int max_fps;

    float x = -13.0;//move this to a camera
    float y = -8.0;
    float z = -25.0;

    explicit GameManager(QWidget *parent = 0,int maxfps=60);
    ~GameManager() override;


    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;

    protected:
    void mousePressEvent(QMouseEvent *e) override;//input manager
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;
    void keyPressEvent ( QKeyEvent * event ) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void initShaders();
    void initTextures();

private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;
    std::vector<Room*> scene;
    Player *player;
    QMatrix4x4 projection;

};

#endif // GAMEMANAGER_H
