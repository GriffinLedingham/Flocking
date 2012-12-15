#include "bird.h"
#include <math.h>
#include <time.h>
#include <QVector3D>
#include <QVector>
#include "flock.h"
#include <math.h>
#include <float.h>
#include <OpenGL/glu.h>
#include <QDebug>
#include <iostream>
#include "cube.h"
using namespace std;

extern bool boundaryDis;
QVector3D topGlobal,noseGlobal,leftGlobal,rightGlobal;

Bird::Bird()
{


}

Bird::Bird(float x, float y, float z, int i,float red,float green,float blue)
{
    this->acceleration = QVector3D(0,0,0);
    float velocityX,velocityY,velocityZ;
    velocityX = -1+2*((float)rand())/(float)RAND_MAX;
    velocityY = -1+2*((float)rand())/(float)RAND_MAX;
    velocityZ = -1+2*((float)rand())/(float)RAND_MAX;

    this->velocity = QVector3D(velocityX,velocityY,velocityZ);
    this->position = QVector3D(x,y,z);

    r = 2.0f;
    maxvelocity = .5;
    maxforce = .03f;

    this->redColor = red;
    this->greenColor = green;
    this->blueColor = blue;
    this->wingStep = .2;
    this->wingHeight = 0;
}

void Bird::animate(QVector<Bird> birds, QVector<cube> cubes) //run
{
    flock(birds,cubes);
    update();
    collisionDetect(cubes);
    if(!boundaryDis)
    {
        boundary();
    }
    draw();
}

void Bird::applyForce(QVector3D force)
{
    acceleration += force;
}

void Bird::flock(QVector<Bird> birds, QVector<cube> cubes)
{
    QVector3D separation = separate(birds,cubes);
    QVector3D alignment = align(birds);
    QVector3D cohesion = cohes(birds);

    separation *= .2f;
    alignment *= .2;
    cohesion *= .1;

    applyForce(separation);
    applyForce(alignment);
    applyForce(cohesion);
}

void Bird::update()
{
    QVector3D oldPos = position;
    velocity+=acceleration;
    velocity = limit(velocity,maxvelocity);
    position+=velocity;
    acceleration = QVector3D(0,0,0);
    direction = position - oldPos;
}

QVector3D Bird::limit(QVector3D vec, float limit)
{
    QVector3D temp;
    if(vec.length() < limit)
    {
        return vec;
    }
    else
    {
        temp = vec.normalized();
        temp*= limit;
        return temp;
    }
}

QVector3D Bird::seek(QVector3D target)
{
    QVector3D desired = target - position;
    desired.normalize();
    desired*= maxvelocity;
    QVector3D steer = desired - velocity;
    steer = limit(steer,maxforce);

    return steer;
}

void Bird::draw()
{
    //color
    float mat_ambient_color[] = {redColor, greenColor, blueColor, 1.0f};
    float high_shininess = 100.0f;
    float mat_specular[] = {1.0f, 1.0f,1.0f, 1.0f};
    float no_mat[] = {redColor, greenColor, blueColor, 1.0f};
    float mat_diffuse[] = {redColor, greenColor, blueColor, 1.0f};

    //set material
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, high_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    glPushMatrix();

    QVector3D tail = QVector3D::crossProduct(velocity,QVector3D(0,1,0));
    tailGlobal = tail;
    tail.normalize();
    QVector3D tailMidPoint = position - velocity.normalized() * 1.0f;
    QVector3D nose = position + velocity.normalized() * 6.0f;
    noseGlobal = nose;

    QVector3D left = tailMidPoint;
    left -= tail * 3;
    QVector3D right = tailMidPoint;
    right += tail * 3;
    QVector3D top = tailMidPoint;
    top += QVector3D(0,1,0) *3;
    topGlobal = top;
    QVector3D bottom = tailMidPoint;
    bottom -= QVector3D(0,1,0)*3;
    leftGlobal = left;
    rightGlobal = right;

    wingHeight += wingStep;
    if(wingHeight>4)
    {
        wingStep = -.2f;
    }
    if(wingHeight<-4)
    {
        wingStep = .2f;
    }

   /* QVector3D wingFront = tail + 2*((nose-tail)/3);
    QVector3D wingBack = tail + ((nose-tail)/3);
    QVector3D rightWingPoint = wingBack + ((wingFront-wingBack)/2) + QVector3D(1,0,0)*6;
    QVector3D leftWingPoint = wingBack + ((wingFront-wingBack)/2) - QVector3D(1,0,0)*6;

    //QVector3D rightWingPoint = tail + right*3 + QVector3D(0,1,0)*wingHeight;
    //QVector3D leftWingPoint = tail + left*3 + QVector3D(0,1,0)*wingHeight;
*/


    glBegin(GL_QUADS);
    glVertex3f(nose.x(),nose.y(),nose.z());
    glVertex3f(left.x(),left.y(),left.z());
    glVertex3f(bottom.x(),bottom.y(),bottom.z());
    glVertex3f(nose.x(),nose.y(),nose.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(nose.x(),nose.y(),nose.z());
    glVertex3f(right.x(),right.y(),right.z());
    glVertex3f(bottom.x(),bottom.y(),bottom.z());
    glVertex3f(nose.x(),nose.y(),nose.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(nose.x(),nose.y(),nose.z());
    glVertex3f(right.x(),right.y(),right.z());
    glVertex3f(top.x(),top.y(),top.z());
    glVertex3f(nose.x(),nose.y(),nose.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(nose.x(),nose.y(),nose.z());
    glVertex3f(left.x(),left.y(),left.z());
    glVertex3f(top.x(),top.y(),top.z());
    glVertex3f(nose.x(),nose.y(),nose.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(bottom.x(),bottom.y(),bottom.z());
    glVertex3f(left.x(),left.y(),left.z());
    glVertex3f(top.x(),top.y(),top.z());
    glVertex3f(right.x(),right.y(),right.z());
    glEnd();



    /*glBegin(GL_QUADS);
    glVertex3f(wingFront.x(),wingFront.y(),wingFront.z());
    glVertex3f(wingBack.x(),wingBack.y(),wingBack.z());
    glVertex3f(rightWingPoint.x(),rightWingPoint.y(),rightWingPoint.z());
    glVertex3f(rightWingPoint.x(),rightWingPoint.y(),rightWingPoint.z());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(wingFront.x(),wingFront.y(),wingFront.z());
    glVertex3f(wingBack.x(),wingBack.y(),wingBack.z());
    glVertex3f(leftWingPoint.x(),leftWingPoint.y(),leftWingPoint.z());
    glVertex3f(leftWingPoint.x(),leftWingPoint.y(),leftWingPoint.z());
    glEnd();*/
    glPopMatrix();

    //Wing code below.
    //Wings do not stay attached to body.
/*
    glBegin(GL_QUADS);
    glVertex3f(nose.x(),nose.y(),nose.z() + 3);
    glVertex3f(nose.x(),nose.y(),nose.z() + 7);
    glVertex3f(nose.x()-5,nose.y() + wingHeight,nose.z() + 5);
    glVertex3f(nose.x()-5,nose.y() + wingHeight,nose.z() + 5);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(nose.x(),nose.y(),nose.z() + 3);
    glVertex3f(nose.x(),nose.y(),nose.z() + 7);
    glVertex3f(nose.x()+5,nose.y() + wingHeight,nose.z() + 5);
    glVertex3f(nose.x()+5,nose.y() + wingHeight,nose.z() + 5);
    glEnd();

*/

}

void Bird::collisionDetect(QVector<cube> cubes)
{

    QVector<cube>::iterator cube;
    for(cube = cubes.begin(); cube != cubes.end(); cube++)
    {
        float d = (position-(*cube).cubePos).length();
        if(d>0 && d < 2*(*cube).radius)
        {
            QVector3D diff = position - (*cube).cubePos;
            applyForce(diff);
        }
    }
}



QVector3D Bird::align(QVector<Bird> birds)
{
    float dist = 30;
    QVector3D sum = QVector3D(0,0,0);
    int count = 0;
    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position - (*bird).position).length();
        if ((d > 0) && (d < dist))
        {
            sum+= (*bird).velocity;
            count++;
        }
    }
    if (count > 0)
    {
        sum.normalize();
        sum *= maxvelocity;
        QVector3D steer = sum-velocity;
        steer = limit(steer,maxforce);
        return steer;
    }
    else
    {
        return QVector3D(0,0,0);
    }
}

QVector3D Bird::cohes(QVector<Bird> birds)
{
    int count = 0;
    float dist = 30;
    QVector3D sum = QVector3D(0,0,0);
    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position - (*bird).position).length();
        if ((d > 0) && (d < dist))
        {
            sum+= (*bird).position;
            count++;
        }
    }

    if (count > 0)
    {
        sum /=count;
        return seek(sum);
    }
    else
    {
        return QVector3D(0,0,0);
    }

}

QVector3D Bird::separate(QVector<Bird> birds, QVector<cube> cubes)
{
    float desiredSeparation = 10.0f;
    QVector3D steer = QVector3D(0,0,0);
    int count = 0;

    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position-(*bird).position).length();
        if(d>0 && d < desiredSeparation)
        {
            QVector3D diff = position - (*bird).position;
            diff.normalize();
            diff /= d;
            steer += diff;
            count++;
        }
    }

    if(count > 0)
    {
        steer /= (float)count;
    }
    if(steer.length() > 0)
    {
        steer.normalize();
        steer *= maxvelocity;
        steer -= velocity;
    }

    steer = limit(steer, maxforce);

    return steer;
}

void Bird::boundary()
{
    if(position.x() > 120 || position.x() < -120)
    {
        if(position.x() > 0)
        {
            position.setX(position.x()-10);
        }
        else
        {
            position.setX(position.x()+10);

        }
        position.setX(-position.x());
    }

    if(position.y() > 60 || position.y() < -60)
    {
        if(position.y() > 0)
        {
            position.setY(position.y()-10);
        }
        else
        {
            position.setY(position.y()+10);

        }

        position.setY(-position.y());
        position.setX(-position.x());
    }

    if(position.z() > 200 || position.z() < 1)
    {
        if(position.z() > 100)
        {
            position.setZ(10);
        }
        else
        {
            position.setZ(200);

        }
    }
}


