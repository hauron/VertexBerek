#ifndef MAINGLWIDGET_H
#define MAINGLWIDGET_H

#include <QGLWidget>
#include <GL/glu.h>

#include <QKeyEvent>
#include <QMouseEvent>

#include "world/world.h"

class MainGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MainGLWidget(QWidget *parent = 0);

signals:
    void keyPressed(int eventKey);
    void keyReleased(int eventKey);
    void mouseMoved(int x, int y);

protected slots:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void setLook();
};

#endif // MAINGLWIDGET_H
