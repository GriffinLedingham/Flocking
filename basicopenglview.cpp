#include "basicopenglview.h"
#include <QTimer>
#include <OpenGL/glu.h>
#include <QMouseEvent>
#include <cmath>
#include "flock.h"
#include "bird.h"
#include "cloth.h"
#include "cube.h"

float cameraZ = -10.0f;
float cameraX = 0.0f;
float cameraY = -3.0f;
float cameraRotHor = 0.0f;
float cameraRotVer = 0.0f;
float windowWidth = 550;
float windowHeight = 281;

extern bool boundaryDis;

extern int verticalSpringCount,horizontalSpringCount;

QVector3D gravityVector = QVector3D(0.0f,-0.1f,0.0f);

extern bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall, spaceDown, delDown;

int ballX,ballY,ballZ;
Flock flock;

extern bool birdCam;

QVector<cube> cubes;

//First line of code to be uncommented for a cloth simulation
//cloth cloth1(14,10);

BasicOpenGLView::BasicOpenGLView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    projectionMatrix(),
    viewMatrix()
{

    cubes.append(cube(QVector3D(-50,0,50),50,50,50));
    cubes.append(cube(QVector3D(70,-10,130),50,50,50));

    srand(time(NULL));

    flock = Flock(cubes);

    for (int i = 0; i < 51; i++)
    {
        if(i<26)
        {
            Bird *tempBird = new Bird(-40,0,140,i,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX);
            flock.newBird(*tempBird);
        }
        else
        {
            Bird *tempBird = new Bird(0,0,160,i,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX,(float)rand()/(float)RAND_MAX);
            flock.newBird(*tempBird);
        }
    }


}

void BasicOpenGLView::initializeGL()
{
    glClearColor(.0f, .0f, .0f, 1.0f);

    //Timer which updates the GL render.
    QTimer *timer = new QTimer();
    timer->start(17); //time in ms, here 17 ms
    connect(timer ,SIGNAL(timeout()),this,SLOT(paintGL()));

    //Timer which updates the scene objects.
    QTimer *animate = new QTimer();
    animate->start(1000); //time in ms, here 1000 ms
    connect(timer ,SIGNAL(timeout()),this,SLOT(animateGL()));
}

void BasicOpenGLView::animateGL()
{
    //Controls used to move camera, based on key input
    if(!birdCam)
    {
        if(mFor == true)
        {
            cameraX = cameraX - sin((cameraRotHor*3.141592)/180);
            cameraZ = cameraZ + cos((cameraRotHor*3.141592)/180);
        }
        if(mBack == true)
        {
            cameraX = cameraX + sin((cameraRotHor*3.141592)/180)*.5;
            cameraZ = cameraZ - cos((cameraRotHor*3.141592)/180)*.5;
        }
        if(mLeft == true)
        {
            cameraX = cameraX - sin(((cameraRotHor-90)*3.141592)/180);
            cameraZ = cameraZ + cos(((cameraRotHor-90)*3.141592)/180);
        }
        if(mRight == true)
        {
            cameraX = cameraX - sin(((cameraRotHor+90)*3.141592)/180);
            cameraZ = cameraZ + cos(((cameraRotHor+90)*3.141592)/180);
        }
    }
    if(cLeft == true)
    {
        cameraRotHor -= 3;
    }
    if(cRight == true)
    {
        cameraRotHor += 3;
    }
    if(cUp == true)
    {
        cameraRotVer -= 3;
    }
    if(cDown == true)
    {
        cameraRotVer += 3;
    }
    if(cJump == true)
    {
        cameraY -= 1;
    }
    if(cFall == true)
    {
        cameraY += 1;
    }
}

void BasicOpenGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if(birdCam == false)
    {
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        viewMatrix.rotate(cameraRotVer,1,0,0);
        viewMatrix.rotate(cameraRotHor,0,1,0);

        viewMatrix.translate(cameraX,cameraY,cameraZ);
        viewMatrix.lookAt(QVector3D(0.0,3.0,-50.0),
                          QVector3D(0.0,4.0,0.0),
                          QVector3D(0.0,1.0,0.0));
        glLoadMatrixd(viewMatrix.data());
    }
    else
    {
        glMatrixMode(GL_MODELVIEW);
        viewMatrix.setToIdentity();
        if(cameraRotHor>30)
        {
            cameraRotHor = 30;
        }
        else if(cameraRotHor < -30)
        {
            cameraRotHor = -30;
        }
        if(cameraRotVer>30)
        {
            cameraRotVer = 30;
        }
        else if(cameraRotVer < -30)
        {
            cameraRotVer = -30;
        }
        viewMatrix.rotate(cameraRotVer,1,0,0);
        viewMatrix.rotate(180 + cameraRotHor,0,1,0);

        viewMatrix.lookAt(flock.birds[15].noseGlobal + flock.birds[15].velocity.normalized(),
                          flock.birds[15].position - flock.birds[15].velocity * 3,
                          QVector3D(0,1,0));
        glLoadMatrixd(viewMatrix.data());
    }

    for(int i=0;i<cubes.size();i++)
    {

        cubes[i].draw();
    }
//=======================================================================================================================================================================================

    //Below was to have my Assignment 3 cloth fall onto the birds.
    //There is some functionality in my implementation, but the cloth only collision detecs noses and heads, therefor causing very undesired behavior.
    //The simulation was too slow with cloth enabled, but you may uncomment my lines in this file to test it.

    //The way I should do collision detection here would be creating arrays of points along each edge of each bird, and checking each point/cloth masspoint per timestep.
    //This would unfortunately be too compuationally intensive, and I did not have time to fully implement this functionality.

//=======================================================================================================================================================================================
    //glEnable(GL_DEPTH_TEST);

    /*float newWindX = (float)rand()/((float)RAND_MAX/.01f);
    float newWindY = (float)rand()/((float)RAND_MAX/.01f);
    float newWindZ = (float)rand()/((float)RAND_MAX/.01f);

    for(int i=0;i<verticalSpringCount;i++)//i: x coordinate in array
    {
        for(int j = 0;j<horizontalSpringCount;j++)//j: y coordinate in array
        {
            massPoint *temp = cloth1.getmassPoint(j,i);
            for(int k = 0;k<flock.birds.size();k++)
            {
                if(
                        (flock.birds[k].noseGlobal.x() < temp->pos.x() + 1 && flock.birds[k].noseGlobal.x() > temp->pos.x() - 1)
                        &&
                        (flock.birds[k].noseGlobal.y() < temp->pos.y() + 1 && flock.birds[k].noseGlobal.y() > temp->pos.y() - 1)
                        &&
                        (flock.birds[k].noseGlobal.z() < temp->pos.z() + 1 && flock.birds[k].noseGlobal.z() > temp->pos.z() - 1)
                        )
                {
                    //qDebug("A BOID HIT A FLAG");
                    temp->accel = QVector3D(0,0,0);
                    temp->fixPosition();
                    temp->accel = flock.birds[k].acceleration;
                    temp->pos = flock.birds[k].noseGlobal;
                    temp->old_pos = flock.birds[k].noseGlobal;
                    temp->birdContact=true;
                }
                else if(
                        (flock.birds[k].topGlobal.x() < temp->pos.x() + 1 && flock.birds[k].topGlobal.x() > temp->pos.x() - 1)
                        &&
                        (flock.birds[k].topGlobal.y() < temp->pos.y() + 1 && flock.birds[k].topGlobal.y() > temp->pos.y() - 1)
                        &&
                        (flock.birds[k].topGlobal.z() < temp->pos.z() + 1 && flock.birds[k].topGlobal.z() > temp->pos.z() - 1)
                        )
                {
                    temp->accel = QVector3D(0,0,0);
                    temp->fixPosition();
                    temp->accel = flock.birds[k].acceleration;
                    temp->pos = flock.birds[k].topGlobal;
                    temp->old_pos = flock.birds[k].topGlobal;

                    temp->birdContact=true;


                }
                else if(
                        (flock.birds[k].leftGlobal.x() < temp->pos.x() + 1 && flock.birds[k].leftGlobal.x() > temp->pos.x() - 1)
                        &&
                        (flock.birds[k].leftGlobal.y() < temp->pos.y() + 1 && flock.birds[k].leftGlobal.y() > temp->pos.y() - 1)
                        &&
                        (flock.birds[k].leftGlobal.z() < temp->pos.z() + 1 && flock.birds[k].leftGlobal.z() > temp->pos.z() - 1)
                        )
                {
                    temp->accel = QVector3D(0,0,0);
                    temp->fixPosition();
                    temp->accel = flock.birds[k].acceleration;
                    temp->pos = flock.birds[k].leftGlobal;
                    temp->old_pos = flock.birds[k].leftGlobal;

                    temp->birdContact=true;


                }
                else if(
                        (flock.birds[k].rightGlobal.x() < temp->pos.x() + 1 && flock.birds[k].rightGlobal.x() > temp->pos.x() - 1)
                        &&
                        (flock.birds[k].rightGlobal.y() < temp->pos.y() + 1 && flock.birds[k].rightGlobal.y() > temp->pos.y() - 1)
                        &&
                        (flock.birds[k].rightGlobal.z() < temp->pos.z() + 1 && flock.birds[k].rightGlobal.z() > temp->pos.z() - 1)
                        )
                {
                    temp->accel = QVector3D(0,0,0);
                    temp->fixPosition();
                    temp->accel = flock.birds[k].acceleration;
                    temp->pos = flock.birds[k].rightGlobal;
                    temp->old_pos = flock.birds[k].rightGlobal;

                    temp->birdContact=true;


                }
                else
                {
                    //temp->birdContact=false;

                }
            }
        }
    }

    //arbitrary gravity value
    cloth1.addPointGravity(gravityVector*.3f);
    cloth1.windForce(QVector3D(newWindX,newWindY,newWindZ)*.3f);
    cloth1.timeStep();*/
//=======================================================================================================================================================================================

    glPushMatrix();

    float redColor = 1;
    float greenColor = 1;
    float blueColor = 1;
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

    //Initialize lighting
    GLfloat light_pos[] = {1.0, 2.0, 3.0, 1.0};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity( );
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glPopMatrix();

    if(!boundaryDis)
    {
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
        glVertex3f(124,-64,204);
        glVertex3f(-124,-64,204);
        glEnd();

        glBegin(GL_LINES);
        glLineWidth(5);
        glVertex3f(124,64,204);
        glVertex3f(-124,64,204);
        glEnd();

        glBegin(GL_LINES);
        glLineWidth(5);
        glVertex3f(124,64,204);
        glVertex3f(124,64,-3);
        glEnd();

        glBegin(GL_LINES);
        glLineWidth(5);
        glVertex3f(124,-64,204);
        glVertex3f(124,-64,-3);
        glEnd();


        glBegin(GL_LINES);
        glLineWidth(5);
        glVertex3f(-124,-64,204);
        glVertex3f(-124,-64,-3);
        glEnd();

        glBegin(GL_LINES);
        glLineWidth(5);
        glVertex3f(-124,64,204);
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
        glVertex3f(-124,-64,204);
        glVertex3f(-124,64,204);
        glEnd();

        glBegin(GL_LINES);
        glLineWidth(5);
        glVertex3f(124,-64,204);
        glVertex3f(124,64,204);
        glEnd();
    }

    flock.animate();

    //Final commented out lines of code to enable cloth simulation.

    //cloth1.drawCloth();
    // this->swapBuffers();

}

void BasicOpenGLView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;

    glMatrixMode(GL_PROJECTION);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, (float)windowWidth/(float)windowHeight, 0.01f, 500.0f);

    glLoadMatrixd(projectionMatrix.data());

    glMatrixMode(GL_MODELVIEW);
}


