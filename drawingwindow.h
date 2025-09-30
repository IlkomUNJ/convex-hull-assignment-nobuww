#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "drawingcanvas.h"

class DrawingWindow : public QMainWindow
{
    Q_OBJECT

public:
    DrawingWindow(QWidget *parent = nullptr);
    ~DrawingWindow();
private:
    QPushButton *m_slowDrawButton;
    QPushButton *m_clearButton;
    QPushButton *m_drawButton;
    DrawingCanvas *m_drawingCanvas;
};

#endif // DRAWINGWINDOW_H
