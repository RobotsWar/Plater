#ifndef VIEWER_H
#define VIEWER_H

#include <QTimer>
#include <QGLWidget>
#include <modelFile/modelFile.h>

class Viewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Viewer(int framesPerSecond = 10, QWidget *parent = 0, char *name = 0);
    void setModel(SimpleModel *model);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent( QKeyEvent *keyEvent );

    SimpleModel *model;
    float t;

public slots:
    virtual void timeOutSlot();

private:
    QTimer *t_Timer;
};

#endif // VIEWER_H
