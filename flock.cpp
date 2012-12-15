#include "flock.h"
#include <QVector>

Flock::Flock()
{

}

Flock::Flock(QVector<cube> cubesIn)
{
    birds = QVector<Bird>();
    cubes = cubesIn;
}

void Flock::animate()
{
    QVector<Bird>::iterator bird;
        for(bird = birds.begin(); bird != birds.end(); bird++)
        {
            (*bird).animate(birds,cubes);
        }
}

void Flock::newBird(Bird bird)
{
    birds.push_back(bird);
}
