#include "windowsinput.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QtWidgets>
#include <QGridLayout>
#include <math.h>
#include <iostream>
#include <QTime>

windowsinput::windowsinput(QWidget *p) : widget(0,100,0), w(0),QOpenGLWidget(p){
}

void windowsinput::showF(){
    QGridLayout *lay = new QGridLayout();
    lay->addWidget(&widget);
    setMinimumSize(QSize(800,800));
    setLayout(lay);
    show();
    //saison.start(3000,this);
}

windowsinput::~windowsinput(){
}

void windowsinput::timerEvent(QTimerEvent *)
{

}

void windowsinput::keyPressEvent (QKeyEvent * event)
{
    if(event->key() == Qt::Key_Q){
        widget.x++;
 }

    if(event->key() == Qt::Key_D){
        widget.x--;
 }
    if(event->key() == Qt::Key_Z){
        widget.y--;
 }
    if(event->key() == Qt::Key_S){
        widget.y++;
 }


    if(event->key() == Qt::Key_W){
        widget.z++;
 }

    if(event->key() == Qt::Key_X){
        widget.z--;
 }

    if(event->key() == Qt::Key_U){
        widget.angularSpeed = 0;
 }

    if (event->key() == Qt::Key_Up){
        widget.angularSpeed += 0.1;

    }

    if (event->key() == Qt::Key_Down){
        widget.angularSpeed -= 0.1;
    }
    if(event->key() == Qt::Key_Right){
        widget.rotationAxis = QVector3D(0,1,0);
        widget.angularSpeed = 0.5;
    }

    if(event->key() == Qt::Key_Left){
        widget.rotationAxis = QVector3D(0,-1,0);
        widget.angularSpeed = 0.5;
    }
}
