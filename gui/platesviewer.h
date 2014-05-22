#ifndef PLATESVIEWER_H
#define PLATESVIEWER_H

#include <QDialog>
#include "viewer.h"
#include "modelFile/modelFile.h"

namespace Ui {
class PlatesViewer;
}

class PlatesViewer : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlatesViewer(QWidget *parent = 0);
    ~PlatesViewer();

    void keyPressEvent(QKeyEvent *);
    void setPlateDimension(float width, float height);
    void setPlates(std::vector<std::string> plates);
    void viewPlate();
    
private slots:
    void on_nextButton_clicked();

    void on_prevButton_clicked();

private:
    int index;
    SimpleModel model;
    Ui::PlatesViewer *ui;
    Viewer *viewer;
    std::vector<std::string> plates;
};

#endif // PLATESVIEWER_H
