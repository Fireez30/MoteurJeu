/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include "player.h"
#include "terrain.h"
#include "object2d.h"
#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <math.h>
#include <iostream>
#include <QVector3D>
#include <QTime>
#include <QDir>
#include <QString>
#include <time.h>

bool start = true;
int initial_time = time (NULL);
int final_time,frame_count;
int last_fps = 0;

MainWidget::MainWidget(QWidget *parent,int maxfps,int saison) :
    QOpenGLWidget(parent),
    texture(0),
    angularSpeed(0)
{
    std::cout << "debut constructeur main widget" << std::endl;
    if (maxfps == 0)
        max_fps = 1;
    else
        max_fps = maxfps;
    et.start();
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    doneCurrent();
}

//! [0]

void MainWidget::keyPressEvent (QKeyEvent * event)
{
    if(event->key() == Qt::Key_Q)
           x++;

    if(event->key() == Qt::Key_D)
           x--;

    if(event->key() == Qt::Key_Z)
          y--;

    if(event->key() == Qt::Key_S)
          y++;

    if(event->key() == Qt::Key_W)
          z++;

    if(event->key() == Qt::Key_X)
          z--;

    if(event->key() == Qt::Key_U)
          angularSpeed = 0;

    if(event->key() == Qt::Key_Right){
        rotationAxis = QVector3D(0,1,0);
        angularSpeed = 0.5;
    }

    if(event->key() == Qt::Key_Left){
        rotationAxis = QVector3D(0,-1,0);
        angularSpeed = 0.5;
    }
}

bool isDirectionNextToBoss(int x,int y, int dir){
    int offsetx = 0;
    int offsety = 0;
    switch (dir){//compute new coordinates using direction
        case 0:
            offsety = -25;
            break;

        case 1:
            offsetx = -25;
            break;

        case 2:
            offsety = 25;
            break;

        default:
            offsetx = 25;
            break;
    }
    return (x+offsetx+25 == 0 && y+offsety == -25) || (x+offsetx-25 == 0 && y+offsety == -25) || (x+offsetx == 0 && y+offsety+25 == -25) || (x+offsetx == 0 && y+offsety-25 == -25);
    //return (x+offsetx == 0 && y+offsety == -25);}
}
std::vector<Room> generateLevel(){//0 -> haut, 1 -> gauche , 2 -> bas , 3 -> droite
    std::vector<Room> result = std::vector<Room>();
    QString path = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\Rooms\\";
    QStringList rooms = QDir(path).entryList();
    for (int i = 0; i < rooms.size(); i++){
        string s = rooms[i].toStdString();
        if (s.find(".oel") != string::npos){//si c'est pas un fichier niveau ogmo
            rooms.removeAt(i);//removeit
        }
    }
    std::cout << "taille room : " << rooms.size() << std::endl;
    srand (time(NULL));//init random
    std::cout << "After  init random" << std::endl;
    int maxdist = 5;
    int dist = 0;
    int x = 0;//x in tile-coordinates (used for generation)
    int y = 0;//y in tile-coordinates (used for generation)
    std::cout << "Before start push" << std::endl;
    result.push_back({"start.oel",x,y});//stockage initial
    std::cout << "Before boss push" << std::endl;
    result.push_back({"boss.oel",x,y-25});//boss
    int direction = rand()%3 + 1;//start direction
    switch (direction){//compute new coordinates using direction
        case 1:
            x-=25;//right
            break;

        case 2:
            y+=25;//bottom
            break;

        default:
            x+=25;//left
            break;
    }
    std::cout << "Before while" << std::endl;
    while (dist < maxdist){
        dist++;
        if (dist == maxdist){//if we reached the end generate key room
            std::cout << "reached end" << std::endl;
            result.push_back({"key.oel",x,y});
            break;
        }
        int dir = direction;
        if (rand()%100 < 25){
            std::cout << "Si change direction" << std::endl;
            direction = rand()%4;//choose a new direction which is not the same
            while (direction-dir == 2 || direction-dir == -2 || direction==dir || isDirectionNextToBoss(x,y,direction)){//if we face down, dont go down or up, same for left right....
                std::cout << "PRINCIPAL mauvaise direction tirée " << std::endl;
                direction = rand()%4;
            }
        }//after decinding of the new direction
        //create new secondary way
        std::cout << "avant tirage test secondaire " << std::endl;
        if ((rand()%100) < (100*((maxdist-dist)/(float)maxdist)))//cant be 100/0
        {
            std::cout << "si secondaire" << std::endl;
            int newx = x;//dont use principal coordinates
            int newy = y;//dont use principal coordinates
            int secondarydir = rand()%4;//choose a direction
            while (secondarydir == direction || secondarydir-dir == 2 || secondarydir-dir == -2 || isDirectionNextToBoss(newx,newy,secondarydir)){//if we face down, dont go down or up, same for left right....
                secondarydir = rand()%4;
                std::cout << "SECONDAIRE mauvaise direction tirée " << std::endl;
            }
            std::cout << "SECONDAIRE Enfin trouvé la bonne direction " << std::endl;
            int offsetx = 0;
            int offsety = 0;
            switch (secondarydir){//compute new coordinates using direction
                case 0:
                    offsety = -25;
                    break;

                case 1:
                    offsetx = -25;
                    break;

                case 2:
                    offsety = 25;
                    break;

                default:
                    offsetx = 25;
                    break;
            }
            int max = rand()%3 + 1;
            int beg = 0;
            while (beg < max){
                beg++;
                if (isDirectionNextToBoss(newx,newy,secondarydir))//if were going next to the boss room stop
                    break;
                std::cout << "generation salle secondaire" << std::endl;
                newx+= offsetx;
                newy+= offsety;
                int room = rand() % rooms.size();
                bool flag = true;
                for (int i=0; i < result.size(); i++)//test if room doesnt exist
                {
                    std::cout << "secondary room test " << std::endl;
                    if (result[i].x == newx && result[i].y == newy)
                        flag = false;
                }

                if (flag){
                    std::cout << "secondary room is ok " << std::endl;
                    result.push_back({rooms[room].toStdString(),newx,newy});
                }
                else
                    break;//cancel while
            }//end secondary way generation
        }//end of secondary test
        std::cout << "secondary generation finie ou inexistante " << std::endl;
        //generate the actual room
        for (int i=0; i < result.size(); i++)//test if room doesnt exist
        {
            if (result[i].x == x && result[i].y == y && result[i].path != "start.oel" && result[i].path != "boss.oel")//if there is a room which is not boss/start
            {result.erase(result.begin()+i);//erase the secondary room
                std::cout << "necessaire de supprimer une salle " << std::endl;}
        }
        std::cout << "apres test existance principal " << std::endl;
        int room = rand() % rooms.size();// bug ici
        std::cout << "push next principal room " << std::endl;
        result.push_back({rooms[room].toStdString(),x,y});
        //update x y coordinates
        switch (direction){//compute new coordinates using direction
            case 0:
                y-=25;
                break;

            case 1:
                x-=25;
                break;

            case 2:
                y+=25;
                break;

            default:
                x+=25;
                break;
        }
    }

    return result;
}

void MainWidget::changeSeason(int a)
{

}

void MainWidget::mousePressEvent(QMouseEvent *e)
{

}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{

}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;


        frame_count++;
        final_time = time(NULL);
        if (final_time - initial_time > 0)
        {
            //display
            last_fps = frame_count/ (final_time - initial_time);
            frame_count = 0;
            initial_time = final_time;
            std::cout << "Fps : " << last_fps << std::endl;
        }

        // Render text
        //timer.start(1000/max_fps,this);
        // Request an update
        et.restart();


    }
            update();
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0,0,0, 1);

   // glOrtho(-17.0,17.0,-17.0,17.0,3.0,7.0);
    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]
//!
//!
    std::cout << "Before Terrain" << std::endl;
    scene = new Terrain;
    std::cout << "Before Player" << std::endl;
    Player* p = new Player;
    std::cout << "Before Add" << std::endl;
    scene->AddChild(p);
    //t->Translate(QVector3D(10,0,0));
    //scene->AddChild(new Terrain());
    //scene.CreateGeometry();//start with the basic level of details
    rotation = QQuaternion::fromAxisAndAngle(1,0,0,135);
    // Use QBasicTimer because its faster than QTimer
    std::cout << "Before Generation" << std::endl;
    std::vector<Room> r = generateLevel();
    std::cout << "Before Affichage" << std::endl;
    for (unsigned i = 0; i < r.size(); i++){
        std::cout << "Salle " << r[i].path << " at x : " << r[i].x/25 << " and y : " << r[i].y/25 << std::endl;
    }
    timer.start(1000/max_fps, this);
}
//! [3]
void MainWidget::initShaders()
{


    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    {
        std::cout << "Erreur lors de la compilation du vertex shader" << std::endl;
        close();
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
    {
        std::cout << "Erreur lors de la compilation du fragment shader" << std::endl;
        close();
    }

    // Link shader pipeline
    if (!program.link())
    {
        std::cout << "Erreur lors du link" << std::endl;
        close();
    }

    // Bind shader pipeline for use
    if (!program.bind())
    {
        std::cout << "Erreur lors du bind" << std::endl;
        close();
    }
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage("sprites.png")); //chargement de la sprite sheet ici

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]


void MainWidget::paintGL()
{

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    texture->bind();
//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(x, y, z);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
//! [6]
    // Use texture unit 0 which contains sprite sheet
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    //geometries->drawMeshGeometry(&program);
    scene->Render(&program,projection);//old version of this is drawTerrainGeometry();
}
