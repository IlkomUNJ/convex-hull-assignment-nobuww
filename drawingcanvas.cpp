#include "drawingcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <algorithm>

DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400, 300);
    m_drawConvexHull = false;
    m_iterations = 0;
}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set a pen and brush for the points
    QPen pointPen(Qt::red, 2, Qt::SolidLine);
    QBrush pointBrush(Qt::red);
    painter.setPen(pointPen);
    painter.setBrush(pointBrush);

    // Always draw the points
    for (const QPoint& p : m_points) {
        painter.drawEllipse(p, 5, 5); // Draw a circle with a radius of 5 pixels
    }

    // Only draw the convex hull if the flag is true
    if (m_drawConvexHull && !m_convexHullPoints.isEmpty()) {
        QPen hullPen(Qt::green, 3, Qt::SolidLine);
        painter.setPen(hullPen);
        painter.setBrush(Qt::NoBrush);

        // Draw the closed polygon using the points in m_convexHullPoints
        painter.drawPolygon(QPolygon(m_convexHullPoints));
    }

    // Draw the iteration count
    painter.setPen(QPen(Qt::black));
    painter.setFont(QFont("Arial", 12));
    painter.drawText(10, 20, QString("Iterations: %1").arg(m_iterations));
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event)
{
    m_points.append(event->pos());

    // Reset the convex hull flag so the previous hull disappears
    m_drawConvexHull = false;
    m_convexHullPoints.clear();
    m_iterations = 0; // Reset iteration count
    update();
}

void DrawingCanvas::calculateAndDrawConvexHull()
{
    m_iterations = 0; 

    if (m_points.size() < 3) {
        qDebug() << "Need at least 3 points to form a convex hull.";
        update(); 
        return;
    }

    std::sort(m_points.begin(), m_points.end(), [](const QPoint& a, const QPoint& b) {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    });

    QVector<QPoint> upperHull;
    for (const auto& p : m_points) {
        m_iterations++;
        while (upperHull.size() >= 2 && orientation(upperHull[upperHull.size() - 2], upperHull.back(), p) != 2) {
            upperHull.pop_back();
            m_iterations++;
        }
        upperHull.append(p);
    }

    QVector<QPoint> lowerHull;
    for (int i = m_points.size() - 1; i >= 0; --i) {
        const auto& p = m_points[i];
        m_iterations++;
        while (lowerHull.size() >= 2 && orientation(lowerHull[lowerHull.size() - 2], lowerHull.back(), p) != 2) {
            lowerHull.pop_back();
            m_iterations++;
        }
        lowerHull.append(p);
    }

    if (!lowerHull.isEmpty()) {
        lowerHull.pop_back();
    }
    if (!lowerHull.isEmpty()) {
        lowerHull.pop_front();
    }

    m_convexHullPoints = upperHull;
    m_convexHullPoints.append(lowerHull);

    m_drawConvexHull = true;
    update();
}

void DrawingCanvas::calculateAndDrawSlowConvexHull()
{
    m_iterations = 0;
    if (m_points.size() < 3) {
        qDebug() << "Need at least 3 points to form a convex hull.";
        update();
        return;
    }

    m_convexHullPoints.clear();

    QVector<QVector<QPoint>> E;

    for (const auto& p : m_points) {
        for (const auto& q : m_points) {
            m_iterations++; 
            if (p == q) continue;

            bool valid = true;
            for (const auto& r : m_points) {
                m_iterations++; 
                if (r == p || r == q) continue;

                if (orientation(p, q, r) == 2) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                E.append({p, q});
            }
        }
    }

    if (E.isEmpty()) {
        qDebug() << "No convex hull edges found.";
        m_drawConvexHull = false;
        update();
        return;
    }

    QPoint currentPoint = E[0][0];
    for (const auto& edge : E) {
        if (edge[0].x() < currentPoint.x()) {
            currentPoint = edge[0];
        }
    }

    m_convexHullPoints.append(currentPoint);
    QPoint nextPoint;

    while(true) {
        m_iterations++;
        bool foundNext = false;
        for (const auto& edge : E) {
            if (edge[0] == currentPoint) {
                nextPoint = edge[1];
                foundNext = true;
                break;
            }
        }

        if (!foundNext) {
            break;
        }

        if (nextPoint == m_convexHullPoints.first()) {
            break;
        }

        m_convexHullPoints.append(nextPoint);
        currentPoint = nextPoint;
    }

    m_drawConvexHull = true;
    update();
}

void DrawingCanvas::clearCanvas()
{
    m_points.clear();
    m_convexHullPoints.clear();
    m_drawConvexHull = false;
    m_iterations = 0;
    update();
}

int DrawingCanvas::orientation(QPoint p, QPoint q, QPoint r)
{
    qint64 val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
    if (val == 0) return 0;
    return (val < 0) ? 1 : 2;
}
