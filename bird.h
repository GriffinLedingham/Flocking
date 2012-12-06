#ifndef BIRD_H
#define BIRD_H

#include <QVector3D>
#include <QVector>

class Bird
{
public:
    Bird();
    Bird(float,float,float,int,float,float,float);
    void animate(QVector<Bird>);
    void applyForce(QVector3D);
    void flock(QVector<Bird>);
    void update();
    QVector3D seek(QVector3D);
    void draw();
    QVector3D separate(QVector<Bird>);
    QVector3D align(QVector<Bird>);
    QVector3D cohes(QVector<Bird>);
    QVector3D limit(QVector3D,float);
    float getAngle(QVector3D);
    void boundary();

private:
    QVector3D position;
    QVector3D velocity;
    QVector3D acceleration;
    float r;
    float maxvelocity;
    float maxforce;

    float redColor;
    float greenColor;
    float blueColor;
};

#endif // BIRD_H
