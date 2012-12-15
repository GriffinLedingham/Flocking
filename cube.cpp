#include "cube.h"
#include <QVector3D>
#include <OpenGL/glu.h>
#include <iostream>

using namespace std;



cube::cube()
{

}

cube::cube(QVector3D position,float height,float width, float depth)
{
    topLeftBack = QVector3D(position.x()+(width/2),position.y()+(height/2),position.z()+(depth/2));
    topRightBack = QVector3D(position.x()-(width/2),position.y()+(height/2),position.z()+(depth/2));
    topLeftFront = QVector3D(position.x()+(width/2),position.y()+(height/2),position.z()-(depth/2));
    topRightFront = QVector3D(position.x()-(width/2),position.y()+(height/2),position.z()-(depth/2));

    botLeftBack = QVector3D(position.x()+(width/2),position.y()-(height/2),position.z()+(depth/2));
    botRightBack = QVector3D(position.x()-(width/2),position.y()-(height/2),position.z()+(depth/2));
    botLeftFront = QVector3D(position.x()+(width/2),position.y()-(height/2),position.z()-(depth/2));
    botRightFront = QVector3D(position.x()-(width/2),position.y()-(height/2),position.z()-(depth/2));

    radius = height/2;

    cubePos = QVector3D(position.x(),position.y(),position.z());



}

void cube::draw()
{


    glBegin(GL_QUADS);
    glVertex3f(topLeftBack.x(),topLeftBack.y(),topLeftBack.z());
    glVertex3f(topRightBack.x(),topRightBack.y(),topRightBack.z());
    glVertex3f(topRightFront.x(),topRightFront.y(),topRightFront.z());
    glVertex3f(topLeftFront.x(),topLeftFront.y(),topLeftFront.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(botLeftBack.x(),botLeftBack.y(),botLeftBack.z());
    glVertex3f(botRightBack.x(),botRightBack.y(),botRightBack.z());
    glVertex3f(botRightFront.x(),botRightFront.y(),botRightFront.z());
    glVertex3f(botLeftFront.x(),botLeftFront.y(),botLeftFront.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(topLeftFront.x(),topLeftFront.y(),topLeftFront.z());
    glVertex3f(topRightFront.x(),topRightFront.y(),topRightFront.z());
    glVertex3f(botRightFront.x(),botRightFront.y(),botRightFront.z());
    glVertex3f(botLeftFront.x(),botLeftFront.y(),botLeftFront.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(topLeftBack.x(),topLeftBack.y(),topLeftBack.z());
    glVertex3f(topRightBack.x(),topRightBack.y(),topRightBack.z());
    glVertex3f(botRightBack.x(),botRightBack.y(),botRightBack.z());
    glVertex3f(botLeftBack.x(),botLeftBack.y(),botLeftBack.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(topLeftFront.x(),topLeftFront.y(),topLeftFront.z());
    glVertex3f(topLeftBack.x(),topLeftBack.y(),topLeftBack.z());
    glVertex3f(botLeftBack.x(),botRightBack.y(),botRightBack.z());
    glVertex3f(botLeftFront.x(),botRightFront.y(),botRightFront.z());
    glEnd();


    glBegin(GL_QUADS);
    glVertex3f(topRightFront.x(),topRightFront.y(),topRightFront.z());
    glVertex3f(topRightBack.x(),topRightBack.y(),topRightBack.z());
    glVertex3f(botRightBack.x(),botRightBack.y(),botRightBack.z());
    glVertex3f(botRightFront.x(),botRightFront.y(),botRightFront.z());
    glEnd();
}
