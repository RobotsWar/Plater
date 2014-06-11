#ifndef WIZARD_H
#define WIZARD_H

#include <QDialog>
#include <stl/StlFactory.h>
#include "viewer.h"

namespace Ui {
class Wizard;
}

class Wizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit Wizard(QString stl, QWidget *parent = 0);
    ~Wizard();
    void setOrientation(std::string orientation);
    QString getPartFilename();
    QString getPartOptions();

    int getQuantity();
    void setPlateDimension(float width, float height);
    
private slots:
    void on_front_clicked();
    void on_bottom_clicked();
    void on_top_clicked();
    void on_back_clicked();
    void on_left_clicked();
    void on_right_clicked();

private:
    QString stl;
    std::string orientation;
    Ui::Wizard *ui;
    Model model;
    Viewer *viewer;
};

#endif // WIZARD_H
