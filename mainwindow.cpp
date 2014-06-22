#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audioiomanager.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AudioIOManager *audioIOManager = new AudioIOManager();
    delete audioIOManager;
}

MainWindow::~MainWindow()
{
    delete ui;
}
