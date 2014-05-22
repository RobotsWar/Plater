#include <sstream>
#include "viewer.h"
#include <GL/glu.h>
#include <GL/glut.h>

Viewer::Viewer(int framesPerSecond, QWidget *parent, char *name)
    : QGLWidget(parent)
{
    setMinimumSize(500, 200);
    t = 0.0;
    if(framesPerSecond == 0)
        t_Timer = NULL;
    else
    {
        int seconde = 1000; // 1 seconde = 1000 ms
        int timerInterval = seconde / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );
    }
}

void Viewer::setModel(SimpleModel *model_)
{
    model = model_;
}

void Viewer::initializeGL()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat ambient[] = {0.8f, 1.0f, 0.4f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Viewer::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 1000.0f);

    // gluLookAt(2.0, 2.0, 2.0, 0, 0, 0, 0, 0, 1);

    if (model) {
        t += 0.05;
        Point3 maxP = model->max();
        float radius = maxP.x;
        if (maxP.y > radius) radius = maxP.y;
        if (maxP.z > radius) radius = maxP.z;
        if (radius < 0) radius *= -1;

        radius *= 3/1000.0;

        float X = radius*cos(t);
        float Y = radius*sin(t);
        float Z = radius;

        gluLookAt(X, Y, Z, 0, 0, 0, 0, 0, 1);

        // GLfloat light0_pos[] = {X, Y, Z, 1.0f};
        GLfloat light0_pos[] = {0, 0, maxP.z*5/1000.0, 1.0f};
        GLfloat light0_dir[] = {1.f, 0.f, 0.f};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, radius/3000.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Viewer::paintGL()
{
    GLfloat mat_amb_diff[] = { 1.0, 1.0, 1.0, 1.0 };

    if (model) {
        resizeGL(size().width(), size().height());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                mat_amb_diff);

    glBegin(GL_TRIANGLES);
    if (model != NULL) {
        for (auto volume : model->volumes) {
            for (auto face : volume.faces) {
            for (int i=0; i<3; i++) {
                glVertex3f(face.v[i].x/1000.0, face.v[i].y/1000.0, face.v[i].z/1000.0);
            }
            }
        }
    }

    glEnd();
}

void Viewer::keyPressEvent(QKeyEvent *keyEvent)
{
}

void Viewer::timeOutSlot()
{
    updateGL();
}
