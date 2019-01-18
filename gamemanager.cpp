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
#include "uiobject.h"

bool start = true;
int initial_time = time (NULL);
int final_time,frame_count;
int last_fps = 0;

GameManager::GameManager(QWidget *parent,int maxfps) :
    QOpenGLWidget(parent),
    texture(0),
    angularSpeed(0)
{
    transX = 0;
    transY = 0;
    if (maxfps == 0)
        max_fps = 1;
    else
        max_fps = maxfps;
    et.start();
    scene = std::vector<Room*>();
    lights = std::vector<LightSource*>();
    UI = std::vector<UiObject*>();
    walkDown = false;
    walkUp = false;
    walkLeft = false;
    walkRight = false;
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
   /* for (int i = 0; i < lights.size(); i++){
         delete lights[i];
    }*/
    delete player;
    doneCurrent();
}

Player* GameManager::getPlayer(){return player;}

//
// USER INPUTS
//

//lorsqu'une touche est relachée, ne sert plus pour le calcul du déplacement
void GameManager::keyReleaseEvent (QKeyEvent * event){

    if (event->key() == Qt::Key_Q){
        walkLeft = false;
    }
    if (event->key() == Qt::Key_D){
        walkRight = false;
    }
    if (event->key() == Qt::Key_Z){
        walkUp = false;
    }
    if (event->key() == Qt::Key_S){
        walkDown = false;
    }
}

void GameManager::keyPressEvent (QKeyEvent * event){
    transX=0;
    transY=0;
    //permet de stocker les inputs pour du multi input
    if(event->key() == Qt::Key_Q){
        walkLeft = true;
    }

    if(event->key() == Qt::Key_D)
    {
        walkRight = true;
    }


    if(event->key() == Qt::Key_Z)
    {
        walkUp = true;
    }

    if(event->key() == Qt::Key_S){
        walkDown = true;
    }

    //shaders ON / OFF
    if (event->key() == Qt::Key_T){
        shader++;
        shader = shader % 2;
    }

    //Debug only , mode triche pour pas mourrir
    if (event->key() == Qt::Key_K){
        player->setHealth(999);
        player->PickKey();
    }
/*
    if(event->key() == Qt::Key_W)
        camera->moveCamera(QVector3D(0,0,1));

    if(event->key() == Qt::Key_X)
        camera->moveCamera(QVector3D(0,0,-1));

    if(event->key() == Qt::Key_U)
        angularSpeed = 0;
*/

}

void GameManager::mousePressEvent(QMouseEvent *e)
{
    //utilise la pile secondaire si on en a une
    if (e->button() == Qt::RightButton){
        player->setUtilisationSecondaire(true);
    }
}

void GameManager::mouseReleaseEvent(QMouseEvent *e)
{
    //n'utilise plus la pile secondaire
    if (e->button() == Qt::RightButton){
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
    //détermine le vecteur du mouvement selon l'état des touches
    transX = 0;
    transY = 0;
    if (walkUp)
        transY++;
    if (walkDown)
        transY--;
    if (walkLeft)
        transX--;
    if (walkRight)
        transX++;


    if (transX == 0 && transY == 0)
    {
        player->movAnim->StopWalk();//arreter l'animation de marche si elle tourne
        scene[camera->getCurrentRoom()]->TriggerCheck(player); //détecter collisions du joueur / entités
    }
    else{
        player->movAnim->Walk();//sinon on lance l'animation si pas en cours
        QVector2D vector(transX,transY);
        vector.normalize();
        vector.setY(0);
        vector *= player->GetSpeed();
        player->Move(vector);//mouvement en Y dabord (mouvement décomposé)
        scene[camera->getCurrentRoom()]->TriggerCheck(player);//détecter collisions du joueur / entités
        if(scene[camera->getCurrentRoom()]->CollisionCheck(player->getCollider()))//si le joueur collide avec un mur, reset
        {
            player->ResetMove();
        }
        player->movAnim->Walk();
        vector = QVector2D(transX,transY);
        vector.normalize();
        vector.setX(0);
        vector *= player->GetSpeed();
        player->Move(vector);//mouvement en x ensuite
        scene[camera->getCurrentRoom()]->TriggerCheck(player);
        if(scene[camera->getCurrentRoom()]->CollisionCheck(player->getCollider()))
        {
            player->ResetMove();
        }
    }
    scene[camera->getCurrentRoom()]->affectEnemiesInRange();//vérifie quelles entités sont dans le cone de la lampe et les affecte
    scene[camera->getCurrentRoom()]->UpdateEntities();//met a jour les entités
    player->Update();//update du joueur
    if(player->getPileSecondaire()==nullptr){//cacher l'UI si on a pas de pile secondaire
        UI[3]->renderer.setWidth(0);
        UI[6]->renderer.setWidth(0);
    }
    else{//si on a une pile secondaire afficher l'UI
        UI[3]->renderer.setWidth(player->getPileSecondaire()->getRatioLife()*7);
        UI[6]->renderer.setWidth(1);
    }
    // détection de la mort du joueur : fermer le jeu
    //rajouter un écran de fin ou quelque chose
    if (player->isDead()){
        this->close();
    }

    //Update l'UI selon la vie du joueur
    if (player->getHealth() == 1){
        if (!UI[0]->MainTextBound()){
            UI[0]->BindMainTexture();
        }
        if (UI[1]->MainTextBound()){
            UI[1]->BindAltTexture();
        }
        if (UI[2]->MainTextBound()){
            UI[2]->BindAltTexture();
        }
    }

    else if (player->getHealth() == 2){
        if (!UI[0]->MainTextBound()){
            UI[0]->BindMainTexture();
        }
        if (!UI[1]->MainTextBound()){
            UI[1]->BindMainTexture();
        }
        if (UI[2]->MainTextBound()){
            UI[2]->BindAltTexture();
        }
    }

    if (player->getHealth() == 3){
        if (!UI[0]->MainTextBound()){
            UI[0]->BindMainTexture();
        }
        if (!UI[1]->MainTextBound()){
            UI[1]->BindMainTexture();
        }
        if (!UI[2]->MainTextBound()){
            UI[2]->BindMainTexture();
        }
    }
    update(); //update opengl et appel a paintgl
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
    initializeOpenGLFunctions();

    glClearColor(0,0,0, 1);

    initShaders(); //init les 2 shaders  : normal et UI
    initTextures();//bind la texture qui contient la spritesheet

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    // Use QBasicTimer because its faster than QTimer
    player = new Player();
    lights.push_back(player->getLight());//halo lumineux autour du joueur
    lights.push_back(player->getLampeLight());//cone de la lampe du joueur
    camera = new Camera(&UI);
    srand(time(NULL));
    int seed = rand() %1000000;
    srand(seed);

    //génération du niveau seedé
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

    //création et construction des salles
    for (int i = 0; i < rooms->size();i++){
        Room* r = new Room(&lights);
        r->setPosition(rooms->at(i).x,rooms->at(i).y);
        r->setPlayer(player);
        r->ReadFile(rooms,i, path, player, camera);
        scene.push_back(r);
    }
    //création de la "géométrie" du joueur
    player->renderer.CreateGeometry();
    //placement caméra
    camera->setRooms(scene);
    camera->setCurrentRoom(x,y);
    //début timer tickrate joueur
    timer.start(1000/max_fps, this);
    //UI basique
    UI.push_back(new UiObject(162-11,83+7,0,QVector2D(0.0/16.0,0), QVector2D(1.0/16.0,0)));//coeur 1
    UI.push_back(new UiObject(162-10,83+7,0,QVector2D(0.0/16.0,0), QVector2D(1.0/16.0,0)));//coeur 2
    UI.push_back(new UiObject(162-9,83+7,0,QVector2D(0.0/16.0,0), QVector2D(1.0/16.0,0)));//coeur 3
    UI.push_back(new UiObject(162+5,83+7,0,QVector2D(13.0/16.0,5.0/16.0), QVector2D(13.0/16.0,5.0/16.0)));//barre lampe
    UI.push_back(new UiObject(162-5,83-7,0,QVector2D(13.0/16.0,5.0/16.0), QVector2D(13.0/16.0,5.0/16.0)));//boss life bar
    UI.push_back(new UiObject(162-6,83-7,0,QVector2D(0.0/16.0,12.0/16.0), QVector2D(0.0/16.0,12.0/16.0)));//boss icon
    UI.push_back(new UiObject(162+4,83+7,0,QVector2D(12.0/16.0,7.0/16.0), QVector2D(12.0/16.0,7.0/16.0)));//icone lampe
    UI[3]->renderer.setWidth(7.0f);
    UI[4]->renderer.setWidth(10.0f);
}

void GameManager::initShaders()
{
    //Shaders de base
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

    //Shaders UI
    // Compile vertex shader
    if (!uiprogram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/uivshader.glsl"))
    {
        std::cout << "Erreur lors de la compilation du vertex shader" << std::endl;
        close();
    }

    // Compile fragment shader
    if (!uiprogram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/uifshader.glsl"))
    {
        std::cout << "Erreur lors de la compilation du fragment shader" << std::endl;
        close();
    }

    // Link shader pipeline
    if (!uiprogram.link())
    {
        std::cout << "Erreur lors du link" << std::endl;
        close();
    }

    // Bind shader pipeline for use
    if (!uiprogram.bind())
    {
        std::cout << "Erreur lors du bind" << std::endl;
        close();
    }
}

void GameManager::initTextures()
{
    //Initialisation de la sprite sheet
    QImage img;
    std::string s = qApp->applicationDirPath().toUtf8().constData();
    s += "/sprites.png";

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
    }

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    texture->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    QVector3D pos = camera->getPosition();
    matrix.translate(pos.x(), pos.y(), pos.z());
    matrix.rotate(rotation);

    //bind shaders UI et affichage UI
    uiprogram.bind();
    uiprogram.setUniformValue("mvp_matrix", projection * matrix);

    for (int i = 0; i < UI.size(); i++){
        UI[i]->Render(&uiprogram,texture);
    }

    //bind UI Classique
    program.bind();

    QVector2D size = QVector2D(this->width(),this->height());
    player->ChangeOrientation(this->mapFromGlobal(QCursor::pos()),matrix,projection,size);//adaptation sprite joueur a la souris

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    //Bind des lightsources
    QRect vp = QRect(0,0,size.x(),size.y());
    program.setUniformValue("test",shader);
    int nbLights = lights.size();
    program.setUniformValue("numLights", nbLights);
    lights[0]->position = QVector2D(player->position.x(),player->position.y());
    lights[1] = player->getLampeLight();
    lights[1]->position = QVector2D(player->position.x(),player->position.y());
    lights[1]->direction= -player->getDirection();
    for(int i=0;i<lights.size();i++){
        QVector3D conversionDeMerde = QVector3D(lights[i]->position.x(),lights[i]->position.y(),0);
        QVector3D screenpos = conversionDeMerde.project(matrix,projection,vp);
        std::string sBase = "allLights[" + std::to_string(i);
        std::string var = "].position";
        std::string s = sBase + var;
        program.setUniformValue(s.data(),QVector4D(screenpos.x()+24,720-(screenpos.y()+24),0,0));
        var = "].color";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->color);
        var = "].attenuation";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->attenuation);
        var = "].ambientCoefficient";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->ambientCoefficient);
        var = "].coneAngle";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->coneAngle);
        var = "].maxAngle";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->maxAngle);
        var = "].coneDirection";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->direction);
        var = "].dist";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->dist*48);
        var = "].maxDist";
        s = sBase + var;
        program.setUniformValue(s.data(),lights[i]->maxDist*48);
    }
    // Use texture unit 0 which contains sprite sheet
    program.setUniformValue("texture", 0);
    //rendu joueur
    player->Render(&program,texture);
    //rendu salle
    scene[camera->getCurrentRoom()]->Render(&program,texture);//render different components of the room
}
