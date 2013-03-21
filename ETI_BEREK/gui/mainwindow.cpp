#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainGLWidget *MainWindow::getCentralWidget()
{
    return ui->widget;
}

void MainWindow::toggleFullScreen()
{
    if(this->isFullScreen())
        this->showNormal();
    else
        this->showFullScreen();
}
