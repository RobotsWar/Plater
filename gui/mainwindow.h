#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QTimer>
#include <QMainWindow>
#include "wizard.h"
#include "worker.h"
#include "platesviewer.h"
#include "about.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void enableAll(bool enable);
    void showError(std::string msg);
    void showSuccess(std::string msg);
    void showMessage(std::string msg);
    void wizardNext();
    float getPlateWidth();
    float getPlateHeight();

public slots:
    void on_worker_end();

    void on_about();

    void on_wizard_accept();
    
private slots:
    void on_outputDirectoryButton_clicked();

    void on_runButton_clicked();

    void on_partBrowse_clicked();

    void on_saveButton_clicked();

    void on_openButton_clicked();

    void on_clearButton_clicked();

    void timeOutSlot();

private:
    Ui::MainWindow *ui;
    QThread thread;
    bool enabled;
    Worker worker;
    Wizard *wizard;
    About *about;
    PlatesViewer *platesViewer;
    QStringList stls;
    QString workingDirectory;
    QTimer *timer;
};

#endif // MAINWINDOW_H
