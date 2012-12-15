#ifndef CUBE_H
#define CUBE_H
#include <QVector3D>

class cube
{
public:
    cube();
    cube(QVector3D,float,float,float);
    void draw();
    QVector3D topLeftBack,topRightBack,topLeftFront,topRightFront;
    QVector3D botLeftBack,botRightBack,botLeftFront,botRightFront;
    float radius;
    QVector3D cubePos;
private:
};

#endif // CUBE_H
