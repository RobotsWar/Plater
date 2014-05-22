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
    GLfloat specular[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat ambient[] = {0.3f, 0.3f, 0.3f, 0.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glClearStencil(0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // glEnable(GL_MODULATE);
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glEnable(GL_CULL_FACE);

    // glDepthFunc(GL_LEQUAL);
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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
        t += 0.02;
        Point3 maxP = model->max();
        float radius = maxP.x;
        if (maxP.y > radius) radius = maxP.y;
        if (maxP.z > radius) radius = maxP.z;
        if (radius < 0) radius *= -1;

        radius *= 3/1000.0;

        float X = radius*cos(t);
        float Y = radius*sin(t);
        float Z = radius;

        float Xl = radius*cos(t+M_PI/2.0);
        float Yl = radius*sin(t+M_PI/2.0);

        GLfloat light0_pos[] = {Xl, Yl, Z, 0.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

        gluLookAt(X, Y, Z, 0, 0, 0, 0, 0, 1);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Viewer::paintGL()
{
    GLfloat mat_amb_diff[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat mat_dif_diff[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_spe_diff[] = { 0.2, 0.2, 0.2, 1.0 };
    glEnable(GL_LIGHTING);

    if (model) {
        resizeGL(size().width(), size().height());
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                mat_amb_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                mat_dif_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                mat_spe_diff);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor4ub(255, 255, 255, 255);

    glBegin(GL_TRIANGLES);
    if (model != NULL) {
        for (auto volume : model->volumes) {
            for (auto face : volume.faces) {
                float x1 = face.v[1].x-face.v[0].x;
                float y1 = face.v[1].y-face.v[0].y;
                float z1 = face.v[1].z-face.v[0].z;

                float x2 = face.v[2].x-face.v[0].x;
                float y2 = face.v[2].y-face.v[0].y;
                float z2 = face.v[2].z-face.v[0].z;

                float Nx = (y1*z2 - z1*y2);
                float Ny = (z1*x2 - x1*z2);
                float Nz = (x1*y2 - y1*x2);
                float Nd = sqrt(Nx*Nx+Ny*Ny+Nz*Nz);

                glNormal3f(Nx/Nd, Ny/Nd, Nz/Nd);
                for (int i=0; i<3; i++) {
                    glVertex3f(face.v[i].x/1000.0, face.v[i].y/1000.0, face.v[i].z/1000.0);
                }
            }
        }
    }
    glEnd();

    glDisable(GL_LIGHTING);
    glLineWidth(3.0);
    glColor3f(0.0, 0.4, 0.0);
    glBegin(GL_LINES);
    for (float x=0; x<=plateWidth; x+=10.0) {
        glVertex3f(x-plateWidth/2, -plateHeight/2, 0);
        glVertex3f(x-plateWidth/2, plateHeight/2, 0);
    }
    for (float y=0; y<=plateHeight; y+=10.0) {
        glVertex3f(-plateWidth/2, y-plateHeight/2, 0);
        glVertex3f(plateWidth/2, y-plateHeight/2, 0);
    }
    glEnd();
}

void Viewer::keyPressEvent(QKeyEvent *keyEvent)
{
}

void Viewer::setPlateDimension(float width, float height)
{
    plateWidth = width;
    plateHeight = height;
}

void Viewer::timeOutSlot()
{
    updateGL();
}
