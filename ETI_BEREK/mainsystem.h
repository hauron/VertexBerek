#ifndef MAINSYSTEM_H
#define MAINSYSTEM_H

#include <QObject>

#include "gui/mainwindow.h"
#include "world/world.h"

#include <QTimer>

#include <QDebug>

class MainSystem : public QObject
{
    Q_OBJECT
public:
    explicit MainSystem(QObject *parent = 0);
    ~MainSystem();

private slots:
    void keyPressed(int eventKey);
    void keyReleased(int eventKey);
    void mouseMoved(int x, int y);

    void frameTick();

private:
    void exitRequested();

    MainWindow m_window;
    QTimer fpsTimer;
};

#endif // MAINSYSTEM_H

