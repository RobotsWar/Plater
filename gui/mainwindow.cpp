#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <log.h>
#include <viewer.h>
#include <wizard.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    enabled(true),
    worker(),
    wizard(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Plater");

    connect(&worker, SIGNAL(workerEnd()), this, SLOT(on_worker_end()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableAll(bool enable)
{
    enabled = enable;
    ui->parts->setEnabled(enable);
    ui->outputDirectory->setEnabled(enable);
    ui->outputDirectoryButton->setEnabled(enable);
    ui->plateHeight->setEnabled(enable);
    ui->plateWidth->setEnabled(enable);
    ui->stlRadio->setEnabled(enable);
    ui->ppmRadio->setEnabled(enable);
    ui->precision->setEnabled(enable);
    ui->spacing->setEnabled(enable);

    if (enable) {
        ui->runButton->setText("Run");
    } else {
        ui->runButton->setText("Cancel");
    }
}

void MainWindow::showError(string error)
{
    error = "<font color=\"red\">" + error + "</font";
    ui->errorMessage->setText(QString::fromStdString(error));
}

void MainWindow::showSuccess(string success)
{
    success = "<font color=\"green\"> " + success + "</font";
    ui->errorMessage->setText(QString::fromStdString(success));
}

void MainWindow::on_outputDirectoryButton_clicked()
{
    ui->outputDirectory->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::on_runButton_clicked()
{
    if (enabled) {
        enableAll(false);
        increaseVerboseLevel();
        worker.request.setPlateSize(ui->plateWidth->text().toFloat(),
                                    ui->plateHeight->text().toFloat());
        worker.request.pattern = ui->outputDirectory->text().toStdString() + "/plate_%03d";
        worker.request.spacing = ui->spacing->text().toFloat()*1000;
        worker.request.precision = ui->precision->text().toFloat()*1000;
        worker.parts = ui->parts->toPlainText().toStdString();
        if (ui->ppmRadio->isChecked()) {
            worker.request.mode = REQUEST_PPM;
        } else {
            worker.request.mode = REQUEST_STL;
        }

        showSuccess("Working...");
        worker.start();
    } else {
        worker.request.cancel = true;
    }
}

void MainWindow::on_partBrowse_clicked()
{
    QString stl = QFileDialog::getOpenFileName(this, "Select a part", "", "*.stl");

    if (stl != "") {
        if (wizard != NULL) {
            wizard->close();
            delete wizard;
        }

        wizard = new Wizard(stl);
        connect(wizard, SIGNAL(accepted()), this, SLOT(on_wizard_accept()));
        wizard->show();
    }
}

void MainWindow::on_worker_end()
{
    if (worker.request.hasError) {
        showError(worker.request.error);
    } else {
        if (worker.request.cancel) {
            showSuccess("Generation cancelled");
        } else {
            ostringstream oss;
            oss << "Generation over, generated " << worker.request.plates << " plates.";
            showSuccess(oss.str());
        }
    }
    enableAll(true);
}

void MainWindow::on_wizard_accept()
{
    QString parts = ui->parts->toPlainText();
    wizard->close();
    parts += wizard->getPart() + "\n";
    ui->parts->setText(parts);
}
