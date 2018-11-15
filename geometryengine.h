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

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    int hmapsize = 0;
    GeometryEngine();
    virtual ~GeometryEngine();

    int meshSize;//LOD 0
    int meshSize1;//LOD 1
    int meshSize2;//LOD 2

    int lod;

    void drawCubeGeometry(QOpenGLShaderProgram *program);
    void drawPlaneGeometry(QOpenGLShaderProgram *program);
    void drawQuadTreeGeometry(QOpenGLShaderProgram *program);
    void drawMeshGeometry(QOpenGLShaderProgram *program);//need only 1 function here, the LOD choice is done inside the function
private:
    void initCubeGeometry();
    void initPlaneGeometry();
    void initQuadTreeGeometry();
    void initMeshGeometry(std::string meshFile);//LOD 0
    void initMeshGeometry1(std::string meshFile);//LOD 1
    void initMeshGeometry2(std::string meshFile);//LOD 2

    QOpenGLBuffer arrayBuf;//LOD 0
    QOpenGLBuffer indexBuf;//LOD 0
    QOpenGLBuffer arrayBuf1;//LOD 1
    QOpenGLBuffer indexBuf1;//LOD 1
    QOpenGLBuffer arrayBuf2;//LOD 2
    QOpenGLBuffer indexBuf2;//LOD 2
};

#endif // GEOMETRYENGINE_H
