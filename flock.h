#ifndef FLOCK_H
#define FLOCK_H

#include <Qvector>
#include "bird.h"
#include "cube.h"

class Flock
{
public:
    Flock();
    Flock(QVector<cube>);
    void animate();
    void newBird(Bird);
    QVector<Bird> birds;
    QVector<cube> cubes;

private:

};

#endif // FLOCK_H
