#include "player.h"
#include <QMouseEvent>
#include <GL/gl.h>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <math.h>
#include <iostream>
#include <QVector3D>
#include <QTime>

Player::Player(){
    CreateGeometry();
}

void Player::CreateCollider(){
    //compute leftcorner pos
    // create the bounding box (give leftcorner, height, width and 1 if obstacle
}


void Player::CreateGeometry()
{
    VertexData vertices[4]= {
        {QVector3D(0.0f, 0.0f,  0.0f), QVector2D(326.0f, 462.0f)},
        {QVector3D(0.0f, 1.0f, 0.0f), QVector2D(341.0f, 462.0f)},
        {QVector3D(1.0f, 0.0f, 0.0f), QVector2D(326.0f, 477.0f)},
        {QVector3D(1.0f, 1.0f, 0.0f), QVector2D(341.0f, 477.0f)}
    };

    GLushort indices[6] = {0, 1, 2, 1, 2, 3};
//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 6 * sizeof(GLushort));
}

void Player::Render(QOpenGLShaderProgram *program, QMatrix4x4 projection){
    std::cout << " Player render" << std::endl;
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    QMatrix4x4 matrix;
    matrix.translate(realPosition.x(), realPosition.y(), realPosition.z());
    matrix.rotate(realRotation);
    // Set modelview-projection matrix
    program->setUniformValue("mvp_matrix", projection * matrix);
 //! [6]
 //!
    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    //Render stuff here (render the loaded mesh)
    for (unsigned i = 0; i < childs.size(); i++){
        //we can optimize here (view dependant, too far from camera , ...)
        childs[i]->Render(program,projection);
    }
}
