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
#include <QGuiApplication>


bool start = true;
int initial_time = time (NULL);
int final_time,frame_count;
int last_fps = 0;

GameManager::GameManager(QWidget *parent,int maxfps) :
    QOpenGLWidget(parent),
    texture(0),
    angularSpeed(0)
{
    if (maxfps == 0)
        max_fps = 1;
    else
        max_fps = maxfps;
    et.start();
    scene = std::vector<Room*>();
    std::cout << "fin constructeur gamemanager" << std::endl;
}

GameManager::~GameManager()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    for (int i = 0; i < scene.size(); i++){
        delete scene[i];
    }
    delete player;
    doneCurrent();
}

Player* GameManager::getPlayer(){return player;}

//
// USER INPUTS
//

void GameManager::keyReleaseEvent (QKeyEvent * event){
    if(event->key() == Qt::Key_Q || event->key() == Qt::Key_D || event->key() == Qt::Key_S || event->key() == Qt::Key_T){
        player->movAnim->StopWalk();
    }
}

void GameManager::keyPressEvent (QKeyEvent * event){
    float transX=0, transY=0;
    if (!player->movAnim->isStarted()){
        player->movAnim->StartAnimator();
    }
    player->movAnim->StopWalk();
    if(event->key() == Qt::Key_Q){
        transX--;
        player->movAnim->Walk();
    }

    if(event->key() == Qt::Key_D)
    {
        transX++;
        player->movAnim->Walk();
    }


    if(event->key() == Qt::Key_Z)
    {
        transY++;
        player->movAnim->Walk();
    }

    if(event->key() == Qt::Key_S){
        transY--;
        player->movAnim->Walk();
    }

    if (event->key() == Qt::Key_T){
        shader++;
        shader = shader % 2;
    }

    QVector3D vector(transX,transY,0);
    vector.normalize();
    vector *= player->GetSpeed();
    player->Move(vector);
    player->SetLastMove(vector);
    int i=0;
    scene[camera->getCurrentRoom()]->TriggerCheck(player);
    i=0;

    if(scene[camera->getCurrentRoom()]->CollisionCheck(player->getCollider()))
    {
        player->movAnim->StopWalk();
        player->Move(-vector);
    }

    if(event->key() == Qt::Key_W)
        camera->moveCamera(QVector3D(0,0,1));

    if(event->key() == Qt::Key_X)
        camera->moveCamera(QVector3D(0,0,-1));

    if(event->key() == Qt::Key_U)
        angularSpeed = 0;

}

void GameManager::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton){
        player->setUtilisationPrincipale(true);
    }
    else if (e->button() == Qt::RightButton && player->getPileSecondaire() != nullptr){
        player->setUtilisationSecondaire(true);
    }
}

void GameManager::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton){
        player->setUtilisationPrincipale(false);
    }
    else if (e->button() == Qt::RightButton){
        player->setUtilisationSecondaire(false);
    }
}

void GameManager::mouseMoveEvent(QMouseEvent *e){

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
        et.restart();
    }

    update();
}

//
// LEVEL GENERATION
//

void generateLevel(int** minMap, int dist, int maxDist,int distSecondaire, int x, int y, bool principal, std::vector<Rooms>* rooms){
    bool up=minMap[x][y+1]==0,down=minMap[x][y-1]==0,right=minMap[x+1][y]==0,left=minMap[x-1][y]==0;
    if(up || down || right || left){
        int memX = x, memY = y;
        if(dist < maxDist){
            minMap[x][y]=1;
            rooms->push_back({"temp",x,y});
            if(principal){
                int xTmp = x, yTmp = y;
                while(minMap[xTmp][yTmp]!=0){
                    int dir = rand()%4;
                    if(up && dir==0)
                        yTmp++;
                    else if(down && dir==1)
                        yTmp--;
                    else if(right && dir==2)
                        xTmp++;
                    else if(left && dir==3)
                        xTmp--;
                }
                generateLevel(minMap,dist+1,maxDist,distSecondaire,xTmp,yTmp,true,rooms);
                up=minMap[memX][memY+1]==0,down=minMap[memX][memY-1]==0,right=minMap[memX+1][memY]==0,left=minMap[memX-1][memY]==0; //Besoin de recalculer à cause de la récursivité
            }
            int chanceSecondaire = rand()%100;
            double maxChance = 100 - (double)dist/(double)maxDist*100;
            if(up && chanceSecondaire < maxChance)
                generateLevel(minMap,dist+1,distSecondaire,distSecondaire,memX,memY+1,false, rooms);
            chanceSecondaire = rand()%100;
            if(down && chanceSecondaire < maxChance)
                generateLevel(minMap,dist+1,distSecondaire,distSecondaire,memX,memY-1,false, rooms);
            chanceSecondaire = rand()%100;
            if(right && chanceSecondaire < maxChance)
                generateLevel(minMap,dist+1,distSecondaire,distSecondaire,memX+1,memY,false, rooms);
            chanceSecondaire = rand()%100;
            if(left && chanceSecondaire < maxChance)
                generateLevel(minMap,dist+1,distSecondaire,distSecondaire,memX-1,memY,false, rooms);
        }
        else if(dist == maxDist && principal){
            minMap[x][y] = 4;
            if(minMap[x][y-1]==0)
                minMap[x][y-1]=-1;
            if(minMap[x][y+1]==0)
                minMap[x][y+1]=-1;
            if(minMap[x-1][y]==0)
                minMap[x-1][y]=-1;
            if(minMap[x+1][y-1]==0)
                minMap[x+1][y]=-1;
            rooms->push_back({"key",x,y});
        }
    }
}
void attributeRoom(int** minMap, std::vector<Rooms>* rooms, std::string path){
    for(int i=0;i<rooms->size();i++){
        if(rooms->at(i).path.compare("temp")==0){
            int x = rooms->at(i).x, y = rooms->at(i).y;
            std::string tmp = "";
            if(minMap[x][y+1]>0)
                tmp+="1";
            else
                tmp+="0";
            if(minMap[x+1][y]>0)
                tmp+="1";
            else
                tmp+="0";
            if(minMap[x][y-1]>0)
                tmp+="1";
            else
                tmp+="0";
            if(minMap[x-1][y]>0)
                tmp+="1";
            else
                tmp+="0";
            std::string dossier = path +"\\"+ tmp;
            QStringList r = QDir(dossier.c_str()).entryList(QStringList() << "*.oel",QDir::Files);
            int id = rand()%r.size();
            rooms->at(i).path = tmp+"\\"+r[id].toStdString();
        }
        else if(rooms->at(i).path.compare("start")==0 || rooms->at(i).path.compare("key")==0){
            int x = rooms->at(i).x, y = rooms->at(i).y;
            std::string tmp = "";
            if(minMap[x][y+1]>0)
                tmp+="1";
            else
                tmp+="0";
            if(minMap[x+1][y]>0)
                tmp+="1";
            else
                tmp+="0";
            if(minMap[x][y-1]>0)
                tmp+="1";
            else
                tmp+="0";
            if(minMap[x-1][y]>0)
                tmp+="1";
            else
                tmp+="0";
            rooms->at(i).path =rooms->at(i).path+"\\"+tmp+".oel";
        }
    }
}

//
//GL SETUP
//

void GameManager::initializeGL()
{

    std::string path = qApp->applicationDirPath().toUtf8().constData();
    path += "/Rooms";
    //"D:\\Enseignement\\Moteur de jeux\\TP\\MoteurHere\\MoteurJeu\\Rooms"; - Benj portable
    //"C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\Rooms" - Benj fixe
    //"D:\\Git\\MoteurJeu\\Rooms" - Romain portable
    //"C:\\Users\\bornt\\Documents\\GitHub\\MoteurJeu\\Rooms" - Benoit portable
    initializeOpenGLFunctions();

    glClearColor(0,0,0, 1);

    // glOrtho(-17.0,17.0,-17.0,17.0,3.0,7.0);
    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    // Use QBasicTimer because its faster than QTimer
    player = new Player();
    std::cout << "PLayer créé" << std::endl;
    camera = new Camera();
    std::cout << "Camera créé" << std::endl;
    //srand(13);
    int seed = 14562;
    std::cout << "avant srand" << std::endl;
    srand(seed);
    std::cout << "apres srand Seed : " << seed << "\n";
    std::vector<Rooms>* rooms = new std::vector<Rooms>();
    int maxdist = 6,distSecondaire = 4;
    int x = maxdist;//x in tile-coordinates (used for generation)
    int y = maxdist;//y in tile-coordinates (used for generation)
    int** minMap = new int*[maxdist*2];
    for(int i = 0; i < maxdist*2; ++i){
        minMap[i] = new int[maxdist*2];
        for(int i2=0;i2<maxdist*2;i2++)
            minMap[i][i2]=0;
    }
    rooms->push_back({"start",x,y});//stockage initial
    minMap[x][y] = 2;
    rooms->push_back({"boss.oel",x,y+1});//A CHANGER TO BOSS.OEL
    minMap[x][y+1] = 3;
    minMap[x][y+2] = -1;
    minMap[x+1][y+1] = -1;
    minMap[x-1][y+1] = -1;

    int dir = rand()%3;
    if(dir==0)
        generateLevel(minMap, 1,maxdist,distSecondaire,x+1,y,true,rooms);
    else if(dir==1)
        generateLevel(minMap, 1,maxdist,distSecondaire,x-1,y,true,rooms);
    else
        generateLevel(minMap, 1,maxdist,distSecondaire,x,y-1,true,rooms);
    int chanceSecondaire = rand()%100;
    double maxChance = 100 - 2/(double)maxdist*100;
    if(minMap[x-1][y]==0 && chanceSecondaire < maxChance)
        generateLevel(minMap,1,distSecondaire,distSecondaire,x-1,y,false, rooms);
    chanceSecondaire = rand()%100;
    if(minMap[x+1][y]==0 && chanceSecondaire < maxChance)
        generateLevel(minMap,1,distSecondaire,distSecondaire,x+1,y,false, rooms);
    chanceSecondaire = rand()%100;
    if(minMap[x][y-1]==0 && chanceSecondaire < maxChance)
        generateLevel(minMap,1,distSecondaire,distSecondaire,x,y-1,false, rooms);

    attributeRoom(minMap, rooms,path);

    for (int i = 0; i < rooms->size();i++){
        Room* r = new Room;
        r->setPosition(rooms->at(i).x,rooms->at(i).y);
        r->setPlayer(player);
        r->ReadFile(rooms,i, path, player, camera);
        scene.push_back(r);
        //std::cout << "Salle " << rooms->at(i).path << " at x : " << rooms->at(i).x << " and y : " << rooms->at(i).y<< std::endl;
    }
    player->renderer.CreateGeometry();
    camera->setRooms(scene);
    camera->setCurrentRoom(x,y);
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
    //std::string s = "D:\\Enseignement\\Moteur de jeux\\TP\\MoteurHere\\MoteurJeu\\sprites.png";
    std::string s = qApp->applicationDirPath().toUtf8().constData();
    s += "/sprites.png";
    std::cout << s << std::endl;
    //"D:\\Enseignement\\Moteur de jeux\\TP\\MoteurHere\\MoteurJeu\\sprites.png"; - Benj portable
    //"C:\\Users\\Fireez\\Documents\\GitHub\\MoteurJeu\\sprites.png" - Benj fixe
    //"D:\\Git\\MoteurJeu\\sprites.png" - Romain portable
    //"C:\\Users\\bornt\\Documents\\GitHub\\MoteurJeu\\sprites.png" - Benoit portable

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
        last_fps = frame_count/ (final_time - initial_time);
        frame_count = 0;
        initial_time = final_time;
        //        std::cout << "Fps : " << last_fps << std::endl;
    }

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    texture->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    QVector3D pos = camera->getPosition();
    matrix.translate(pos.x(), pos.y(), pos.z());
    matrix.rotate(rotation);

    QVector2D size = QVector2D(this->width(),this->height());
    player->ChangeOrientation(this->mapFromGlobal(QCursor::pos()),matrix,projection,size);
    //std::cout << "Player life : " << player->getHealth() << std::endl;
    scene[camera->getCurrentRoom()]->UpdateEntities();
    scene[camera->getCurrentRoom()]->TriggerCheck(player);
    scene[camera->getCurrentRoom()]->affectEnemiesInRange();
    // !! if player HP is 1 , change shaders to color the scren in red ?
    if (player->isDead()){
        //this->close();
        //
    }
    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    QRect vp = QRect(0,0,size.x(),size.y());
    QVector3D screenpos = player->position.project(matrix,projection,vp);
    //program.setUniformValue("playerpos",QVector4D(screenpos.x()+24,720-(screenpos.y()+24),0,0));
    program.setUniformValue("test",shader);
    program.setUniformValue("numLights", 2);
    program.setUniformValue("allLights[0].position",QVector4D(screenpos.x()+24,720-(screenpos.y()+24),0,0));
    program.setUniformValue("allLights[0].color",QVector3D(1,1,1));
    program.setUniformValue("allLights[0].attenuation",0.0005f);
    program.setUniformValue("allLights[0].ambientCoefficient",0.5f);
    program.setUniformValue("allLights[0].coneAngle",180.0f);
    program.setUniformValue("allLights[0].maxAngle",180.0f);
    program.setUniformValue("allLights[0].coneDirection",QVector3D(1,0,0));
    program.setUniformValue("allLights[0].dist",150.0f);
    program.setUniformValue("allLights[0].maxDist",200.0f);

    if (player->utilisePilePrincipale() || player->utilisePileSecondaire()){
        program.setUniformValue("allLights[1].position",QVector4D(screenpos.x()+24,720-(screenpos.y()+24),0,0));
        program.setUniformValue("allLights[1].color",QVector3D(3,3,0));
        program.setUniformValue("allLights[1].attenuation",0.005f);
        program.setUniformValue("allLights[1].ambientCoefficient",0.5f);
        program.setUniformValue("allLights[1].coneAngle",20.0f);
        program.setUniformValue("allLights[1].maxAngle",25.0f);
        program.setUniformValue("allLights[1].coneDirection",QVector3D(-player->GetDirection().x(),-player->GetDirection().y(),0));
        program.setUniformValue("allLights[1].dist",250.0f);
        program.setUniformValue("allLights[1].maxDist",300.0f);
    }
    else {
        program.setUniformValue("allLights[1].position",QVector4D(screenpos.x()+24,720-(screenpos.y()+24),0,0));
        program.setUniformValue("allLights[1].color",QVector3D(3,3,0));
        program.setUniformValue("allLights[1].attenuation",0.005f);
        program.setUniformValue("allLights[1].ambientCoefficient",0.5f);
        program.setUniformValue("allLights[1].coneAngle",20.0f);
        program.setUniformValue("allLights[1].maxAngle",25.0f);
        program.setUniformValue("allLights[1].coneDirection",QVector3D(-player->GetDirection().x(),-player->GetDirection().y(),0));
        program.setUniformValue("allLights[1].dist",0.0f);
        program.setUniformValue("allLights[1].maxDist",0.0f);
    }

    // Use texture unit 0 which contains sprite sheet
    program.setUniformValue("texture", 0);
    player->Render(&program,texture);
    // Draw cube geometry
    //for (size_t i = 0; i < scene.size(); i++){
    //    scene[i]->Render(&program,texture);
    //}
    //std::cout << "AH" << std::endl;
    //std::cout << "Player tient la clé ? " << player->getHoldKey() << std::endl;
    scene[camera->getCurrentRoom()]->Render(&program,texture);//render different components of the room
    //std::cout << player->renderer.spriteCoords.x() << " " << player->renderer.spriteCoords.y() << std::endl;
}
