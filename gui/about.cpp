#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowTitle("About Plater");
    setWindowIcon(QIcon("img/plater.png"));
}

About::~About()
{
    delete ui;
}
