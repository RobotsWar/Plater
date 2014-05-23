#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowTitle("About Plater");
    setWindowIcon(QIcon("img/plater.png"));
    setFixedSize(550, 250);
    QImage img("img/plater.png");
    ui->imgLabel->setPixmap(QPixmap::fromImage(img));
}

About::~About()
{
    delete ui;
}
