#include "drawingwindow.h"
#include "drawingcanvas.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

DrawingWindow::DrawingWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    DrawingCanvas *drawingCanvas = new DrawingCanvas(this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *drawButton = new QPushButton("Draw Fast Convex Hull", this);
    QPushButton *slowDrawButton = new QPushButton("Draw Slow Convex Hull", this);
    QPushButton *clearButton = new QPushButton("Clear", this);

    buttonLayout->addWidget(drawButton);
    buttonLayout->addWidget(slowDrawButton);
    buttonLayout->addWidget(clearButton);

    mainLayout->addWidget(drawingCanvas);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);
    setWindowTitle("Convex Hull Drawing");

    connect(drawButton, &QPushButton::clicked, drawingCanvas, &DrawingCanvas::calculateAndDrawConvexHull);
    connect(slowDrawButton, &QPushButton::clicked, drawingCanvas, &DrawingCanvas::calculateAndDrawSlowConvexHull);
    connect(clearButton, &QPushButton::clicked, drawingCanvas, &DrawingCanvas::clearCanvas);
}

DrawingWindow::~DrawingWindow()
{
}
