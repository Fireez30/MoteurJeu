#include "gamemanager.h"
#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
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

GameManager::GameManager(QWidget *parent,int maxfps) :
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
    scene = std::vector<Room*>();

}

GameManager::~GameManager()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete player;
    doneCurrent();
}

//
// USER INPUTS
//

void GameManager::keyPressEvent (QKeyEvent * event)
{
    if(event->key() == Qt::Key_Q){
           //x++;
           player->Translate(QVector3D(-1,0,0));
    }

    if(event->key() == Qt::Key_D)
    {
           //x--;
           player->Translate(QVector3D(1,0,0));
    }


    if(event->key() == Qt::Key_Z)
    {
          //y--;
          player->Translate(QVector3D(0,1,0));
    }

    if(event->key() == Qt::Key_S){
          //y++;
          player->Translate(QVector3D(0,-1,0));
    }

    if(event->key() == Qt::Key_W)
          z++;

    if(event->key() == Qt::Key_X)
          z--;

    if(event->key() == Qt::Key_U)
          angularSpeed = 0;

}

void GameManager::mousePressEvent(QMouseEvent *e)
{

}

void GameManager::mouseReleaseEvent(QMouseEvent *e)
{

}

//
// TICKRATE WITH TIMERS
//

void GameManager::timerEvent(QTimerEvent *)
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

//
// LEVEL GENERATION
//

bool isDirectionNextToBoss(int x,int y, int dir){
    int offsetx = 0;
    int offsety = 0;
    switch (dir){//compute new coordinates using direction
        case 0:
            offsety = 25;
            break;

        case 1:
            offsetx = -25;
            break;

        case 2:
            offsety = -25;
            break;

        default:
            offsetx = 25;
            break;
    }
    return (x+offsetx+25 == 0 && y+offsety == -25) || (x+offsetx-25 == 0 && y+offsety == -25) || (x+offsetx == 0 && y+offsety+25 == -25) || (x+offsetx == 0 && y+offsety-25 == -25);
    //return (x+offsetx == 0 && y+offsety == -25);}
}
std::vector<Rooms> generateLevel(){//0 -> haut, 1 -> gauche , 2 -> bas , 3 -> droite
    std::vector<Rooms> result = std::vector<Rooms>();
    std::cout<<"On génère le terrain !\n";
    QString path = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\Rooms";
    QStringList rooms = QDir(path).entryList(QStringList() << "*.oel",QDir::Files);//suppose to give only oel files
    srand (42);//init rand
    int maxdist = 3;
    int dist = 0;
    int x = 0;//x in tile-coordinates (used for generation)
    int y = 0;//y in tile-coordinates (used for generation)
    result.push_back({"start.oel",x,y});//stockage initial
    result.push_back({"boss.oel",x,y+15});//A CHANGER TO BOSS.OEL
    int direction = rand()%3 + 1;//start direction
    switch (direction){//compute new coordinates using direction
        case 1:
            x-=25;//right
            break;

        case 2:
            y-=15;//bottom
            break;

        default:
            x+=25;//left
            break;
    }
    while (dist < maxdist){
        dist++;
        if (dist == maxdist){//if we reached the end generate key room
            result.push_back({"key.oel",x,y});// A CHNAGER POUR KEY.OEL
            break;
        }
        int dir = direction;
        if (rand()%100 < 25){
            direction = rand()%4;//choose a new direction which is not the same
            while (direction-dir == 2 || direction-dir == -2 || direction==dir || isDirectionNextToBoss(x,y,direction)){//if we face down, dont go down or up, same for left right....
                direction = rand()%4;
            }
        }//after decinding of the new direction
        //create new secondary way
        if ((rand()%100) < (100*((maxdist-dist)/(float)maxdist)))//cant be 100/0
        {
            int newx = x;//dont use principal coordinates
            int newy = y;//dont use principal coordinates
            int secondarydir = rand()%4;//choose a direction
            while (secondarydir == direction || secondarydir-dir == 2 || secondarydir-dir == -2 || isDirectionNextToBoss(newx,newy,secondarydir)){//if we face down, dont go down or up, same for left right....
                secondarydir = rand()%4;
            }
            int offsetx = 0;
            int offsety = 0;
            switch (secondarydir){//compute new coordinates using direction
                case 0:
                    offsety = 15;
                    break;

                case 1:
                    offsetx = -25;
                    break;

                case 2:
                    offsety = -15;
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
                newx+= offsetx;
                newy+= offsety;
                int room = rand() % (rooms.size()-3);
                bool flag = true;
                for (int i=0; i < result.size(); i++)//test if room doesnt exist
                {
                    if (result[i].x == newx && result[i].y == newy)
                        flag = false;
                }

                if (flag){
                    result.push_back({rooms[room].toStdString(),newx,newy});
                }
                else
                    break;//cancel while
            }//end secondary way generation
        }//end of secondary test
        //generate the actual room
        for (int i=0; i < result.size(); i++)//test if room doesnt exist
        {
            if (result[i].x == x && result[i].y == y && result[i].path != "start.oel" && result[i].path != "boss.oel")//if there is a room which is not boss/start
            {
                result.erase(result.begin()+i);//erase the secondary room
            }
        }
        int room = rand() % (rooms.size() -3);// bug ici
        result.push_back({rooms[room].toStdString(),x,y});
        //update x y coordinates
        switch (direction){//compute new coordinates using direction
            case 0:
                y+=15;
                break;

            case 1:
                x-=25;
                break;

            case 2:
                y-=15;
                break;

            default:
                x+=25;
                break;
        }
    }
    std::cout<<"Terrain généré ! ^^ \n";
    return result;
}

//
//GL SETUP
//

void GameManager::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0,0,0, 1);

   // glOrtho(-17.0,17.0,-17.0,17.0,3.0,7.0);
    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);


    std::cout << "Before Terrain" << std::endl;
    std::cout << "Before Player" << std::endl;
    //t->Translate(QVector3D(10,0,0));
    //scene->AddChild(new Terrain());
    //scene.CreateGeometry();//start with the basic level of details
    //rotation = QQuaternion::fromAxisAndAngle(1,0,0,-35);
    // Use QBasicTimer because its faster than QTimer
    player = new Player();
    std::vector<Rooms> r = generateLevel();
    std::cout << "Before Affichage" << std::endl;
    for (int i = 0; i < r.size();i++){
        scene.push_back(new Room);
    }
    for (int i = 0;i < scene.size(); i++){
        scene[i]->ReadFile(r,i);
    }
    std::cout << "Apres push des salles" << std::endl;
    player->renderer.CreateGeometry();
    std::cout << "Apres creation du joueur" << std::endl;
    for (int i = 0;i < scene.size(); i++){
        scene[i]->CreateGeometry();
    }
    std::cout << "Apres ReadFile" << std::endl;
    /*for (int i = 0;i < scene->GetTiles().size();i++){
        std::cout << "Tile at x :" << scene->GetTiles()[i].GetPosition().x() <<"at y :" << scene->GetTiles()[i].GetPosition().y() << std::endl;
    }*/
    timer.start(1000/max_fps, this);
}

void GameManager::initShaders()
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

void GameManager::initTextures()
{
    QImage img;
    std::string s = "C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\sprites.png";
    img.load(s.data());
    texture = new QOpenGLTexture(img); //chargement de la sprite sheet ici

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}


//
// GL UPDATE
//

void GameManager::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 100.0, fov = 90.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}


void GameManager::paintGL()
{

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

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    texture->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(x, y, z);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains sprite sheet
    program.setUniformValue("texture", 0);
    player->Render(&program,texture);
    // Draw cube geometry
    //geometries->drawMeshGeometry(&program);
    for (int i = 0;i < scene.size(); i++){
        scene[i]->Render(&program,texture);//old version of this is drawTerrainGeometry();
    }
   // player->DisplayCoords();

}
