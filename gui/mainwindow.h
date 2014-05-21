#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QMainWindow>
#include "worker.h"

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
    void showError(std::string error);
    void showSuccess(std::string error);

public slots:
    void on_worker_end();
    
private slots:
    void on_outputDirectoryButton_clicked();

    void on_runButton_clicked();

    void on_partBrowse_clicked();

private:
    Ui::MainWindow *ui;
    QThread thread;
    bool enabled;
    Worker worker;
};

#endif // MAINWINDOW_H
