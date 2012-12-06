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
using namespace std;



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
    maxvelocity = .2;
    maxforce = .03f;
    //qDebug("%d", i);

    this->redColor = red;
    this->greenColor = green;
    this->blueColor = blue;




}

void Bird::animate(QVector<Bird> birds) //run
{
    flock(birds);
    update();
    boundary();
    draw();
}

void Bird::applyForce(QVector3D force)
{
    acceleration += force;
}

void Bird::flock(QVector<Bird> birds)
{
    QVector3D separation = separate(birds);
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
    velocity+=acceleration;
    velocity = limit(velocity,maxvelocity);
    position+=velocity;
    acceleration = QVector3D(0,0,0);
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
    //might be backwards
    QVector3D desired = target - position;
    desired.normalize();
    desired*= maxvelocity;
    //might be backwards
    QVector3D steer = desired - velocity;
    steer = limit(steer,maxforce);

    return steer;
}

void Bird::draw() //render
{
    /*//float angle = getAngle(velocity) + 1.57079633;
    //QQuaternion thisQuat = QQuaternion(position.x(),position.y(),position.z(),0);
    //QQuaternion zeroQuat = (0,0,0,0);
    //QQuaternion = QQuaternion::slerp(thisQuat,zeroQuat);
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    glColor3f(1.0,0,1.0);
    //glRotatef();
    //qDebug("drawing1");
    GLUquadricObj *quadric;
    quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL );
    gluSphere(quadric,1,100,100);
    gluDeleteQuadric(quadric);
    glPopMatrix();
    //qDebug("drawing2");*/

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

    //glColor3f(redColor,greenColor,blueColor);

    //cout << redColor << "..." << greenColor << endl;

    QVector3D tail = QVector3D::crossProduct(velocity,QVector3D(0,1,0));
    tail.normalize();
    QVector3D tailMidPoint = position - velocity.normalized() * 1.0f;
    QVector3D nose = position + velocity.normalized() * 6.0f;

    QVector3D left = tailMidPoint;
    left -= tail * 3;
    QVector3D right = tailMidPoint;
    right += tail * 3;
    QVector3D top = tailMidPoint;
    top += QVector3D(0,1,0) *3;
    QVector3D bottom = tailMidPoint;
    bottom -= QVector3D(0,1,0)*3;

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

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,-64,-3);
    glVertex3f(-124,-64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,64,-3);
    glVertex3f(-124,64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,-64,104);
    glVertex3f(-124,-64,104);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,64,104);
    glVertex3f(-124,64,104);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,64,104);
    glVertex3f(124,64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,-64,104);
    glVertex3f(124,-64,-3);
    glEnd();


    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(-124,-64,104);
    glVertex3f(-124,-64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(-124,64,104);
    glVertex3f(-124,64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(-124,-64,-3);
    glVertex3f(-124,64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,-64,-3);
    glVertex3f(124,64,-3);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(-124,-64,104);
    glVertex3f(-124,64,104);
    glEnd();

    glBegin(GL_LINES);
    glLineWidth(5);
    glVertex3f(124,-64,104);
    glVertex3f(124,64,104);
    glEnd();

    glPopMatrix();


}

QVector3D Bird::separate(QVector<Bird> birds)
{
    float desiredSeparation = 10.0f;
    QVector3D steer = QVector3D(0,0,0);
    int count = 0;

    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position-(*bird).position).length();//QVector3D::distanceToLine(this->position,(*bird).position);
        //QVector3D::
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

QVector3D Bird::align(QVector<Bird> birds)
{
    float neighbordist = 30;
    QVector3D sum = QVector3D(0,0,0);
    int count = 0;
    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position - (*bird).position).length();
        if ((d > 0) && (d < neighbordist)) {
            sum+= (*bird).velocity;
            count++;
        }
    }
    if (count > 0)
    {
        sum /= (float)count;
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
    float neighbordist = 30;
    QVector3D sum = QVector3D(0,0,0);
    int count = 0;
    QVector<Bird>::iterator bird;
    for(bird = birds.begin(); bird != birds.end(); bird++)
    {
        float d = (position - (*bird).position).length();
        if ((d > 0) && (d < neighbordist)) {
            sum+= (*bird).position;
            count++;
        }
    }

    if (count > 0)
    {
        sum /=count;
        return seek(sum);  // Steer towards the location
    }
    else
    {
        return QVector3D(0,0,0);
    }

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

    if(position.z() > 100 || position.z() < 1)
    {
        if(position.z() > 50)
        {
            position.setZ(10);
        }
        else
        {
            position.setZ(100);

        }

        //position.setZ(-position.z());
    }
}
