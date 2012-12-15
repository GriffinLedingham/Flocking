#ifndef BIRD_H
#define BIRD_H

#include <QVector3D>
#include <QVector>
#include "cube.h"

class Bird
{
public:
    Bird();
    Bird(float,float,float,int,float,float,float);
    void animate(QVector<Bird>, QVector<cube>);
    void applyForce(QVector3D);
    void flock(QVector<Bird>, QVector<cube>);
    void collisionDetect(QVector<cube>);
    void update();
    QVector3D seek(QVector3D);
    void draw();
    QVector3D separate(QVector<Bird>, QVector<cube>);
    QVector3D align(QVector<Bird>);
    QVector3D cohes(QVector<Bird>);
    QVector3D limit(QVector3D,float);
    float getAngle(QVector3D);
    void boundary();
    QVector3D topGlobal,noseGlobal,leftGlobal,rightGlobal,tailGlobal;
    QVector3D position;
    QVector3D acceleration;
    float wingHeight;
    float wingStep;
    QVector3D velocity;
    QVector3D direction;

private:
    float r;
    float maxvelocity;
    float maxforce;

    float redColor;
    float greenColor;
    float blueColor;
};

#endif // BIRD_H
