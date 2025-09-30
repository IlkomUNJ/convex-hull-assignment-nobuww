#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QPoint>
#include <QVector>

class DrawingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void calculateAndDrawConvexHull();
    void calculateAndDrawSlowConvexHull();
    void clearCanvas();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int orientation(QPoint p, QPoint q, QPoint r);

    QVector<QPoint> m_points;
    QVector<QPoint> m_convexHullPoints;
    bool m_drawConvexHull;
    int m_iterations;
};

#endif // DRAWINGCANVAS_H
