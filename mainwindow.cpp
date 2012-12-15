#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>

bool mFor, mBack, mLeft, mRight, cUp, cDown, cLeft, cRight, cJump, cFall, spaceDown, delDown;
float windX=0.20f;
float windY=0.20f;
float windZ=0.01f;
float damping=.00f; //how much to damp the cloth simulation each frame
int springConstraintCalculations = 20; //how many springConstraint calculations occur each time step

int flagWidth = 14;
int flagHeight = 10;
float timestep=0.3f;

int verticalSpringCount = 16; //how many vertical springConstraints in the cloth
int horizontalSpringCount = 20; //how many horizontal springConstraints in the cloth
extern float cameraZ;
extern float cameraX;
extern float cameraY;
extern float cameraRotHor;
extern float cameraRotVer;
bool boundaryDis = false;

bool birdCam = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    animationTimer(NULL)
{
    ui->setupUi(this);

    // we create a simple timer, with the widget being its parent
    animationTimer = new QTimer(this);
    // the timer will send a signal timeout at regular intervals.
    // whenever this timeout signal occurs, we want it to call our drawOpenGL
    // function
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(drawOpenGL()));
    // we start the timer with a timeout interval of 20ms
    animationTimer->start(20);

    srand ( time(NULL) );


}

void MainWindow::drawOpenGL()
{
    // whenever drawOpenGL gets called, we only want to repaint our opengl widget
    ui->oglwidget->repaint();
    //ui->oglwidget->animateGL();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->text() =="w")
    {
        mFor = true;
    }

    if(event->text() == "s")
    {
        mBack = true;
    }

    if(event->text() =="a")
    {
        mLeft = true;
    }

    if(event->text() == "d")
    {
        mRight = true;
    }

    if(event->text() == "-") //down
    {
        cFall = true;
    }

    if(event->text() == "=") //up
    {
        cJump = true;
    }

    if(event->key() == Qt::Key_Left) //rot left
    {
        cLeft = true;
    }

    if(event->key() == Qt::Key_Right) //rot right
    {
        cRight = true;
    }

    if(event->key() == Qt::Key_Up) //rot up
    {
        cUp = true;
    }
    if(event->key() == Qt::Key_Space) //rot up
    {
        spaceDown = true;
    }
    if(event->key() == Qt::Key_Backspace) //rot up
    {
        delDown = true;
    }

    if(event->key() == Qt::Key_Down) //rot down
    {
        cDown = true;
    }

    if(event->text() == "r")
    {
        cameraRotHor = 0.0f;
        cameraRotVer = 10.0f;
        cameraX = 0.0f;
        cameraY = -3.0f;
        cameraZ = -10.0f;
    }


}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Space) //rot up
    {
        spaceDown = false;
    }
    if(event->key() == Qt::Key_Backspace) //rot up
    {
        delDown = false;
    }
    if(event->text() == "w")
    {
        mFor = false;
    }

    if(event->text() == "s")
    {
        mBack = false;
    }

    if(event->text() == "a")
    {
        mLeft = false;
    }

    if(event->text() == "d")
    {
        mRight = false;
    }

    if(event->text() == "-") //down
    {
        cFall = false;
    }

    if(event->text() == "=") //up
    {
        cJump = false;
    }

    if(event->key() == Qt::Key_Left) //rot left
    {
        cLeft = false;
    }

    if(event->key() == Qt::Key_Right) //rot right
    {
        cRight = false;
    }

    if(event->key() == Qt::Key_Up) //rot up
    {
        cUp = false;
    }

    if(event->key() == Qt::Key_Down) //rot down
    {
        cDown = false;
    }
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(value == 1)
    {
        birdCam = true;
        cameraRotHor = 0;
        cameraRotVer = 0;
    }
    else
    {
        birdCam = false;
        cameraRotHor = 0;
        cameraRotVer = 0;
        cameraZ = -10.0f;
        cameraX = 0.0f;
        cameraY = -3.0f;
    }
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
    {
        boundaryDis=true;
    }
    else
    {
        boundaryDis=false;
    }
}
