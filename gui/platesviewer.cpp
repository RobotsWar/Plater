#include <QKeyEvent>
#include "platesviewer.h"
#include "ui_platesviewer.h"

PlatesViewer::PlatesViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlatesViewer)
{
    ui->setupUi(this);
    index = 0;
    viewer = new Viewer;
    ui->zone->addWidget(viewer);
}

PlatesViewer::~PlatesViewer()
{
    delete ui;
}

void PlatesViewer::keyPressEvent(QKeyEvent *evt)
{
    if (evt->key() == Qt::Key_P) {
        index--;
        viewPlate();
    }
    if (evt->key() == Qt::Key_N) {
        index++;
        viewPlate();
    }

    QDialog::keyPressEvent(evt);
}

void PlatesViewer::setPlateDimension(float width, float height)
{
    viewer->setPlateDimension(width, height);
}

void PlatesViewer::setPlates(std::vector<std::string> plates_)
{
    plates = plates_;
    viewPlate();
}

void PlatesViewer::viewPlate()
{
    if (plates.size() == 0) {
        return;
    }
    while (index < 0) {
        index += plates.size();
    }
    index = (index)%plates.size();

    FMatrix3x3 id;
    model = loadModelFromFile(plates[index].c_str(), id);
    model = model.center();
    viewer->setModel(&model);

    ui->info->setText(QString("<center>Plate %1/%2<br/>%3</center>").arg(index+1).arg(plates.size()).arg(QString::fromStdString(plates[index])));
}

void PlatesViewer::on_nextButton_clicked()
{
    index++;
    viewPlate();
}

void PlatesViewer::on_prevButton_clicked()
{
    index--;
    viewPlate();
}
