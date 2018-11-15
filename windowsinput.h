#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTime>
#include "mainwidget.h"

class windowsinput : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public :
    MainWidget widget;
    QBasicTimer saison;
    QWidget w;
    void keyPressEvent ( QKeyEvent * event ) override;

    void showF();
    windowsinput(QWidget *p);
    ~windowsinput();
    void timerEvent(QTimerEvent *) override;

signals:
        void timerSeason(int valuechanged);

};

#endif // WINDOWSINPUT_H
