#include "canvas.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <cmath>

Canvas::Canvas(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent),
    m_scale(0.5),
    m_minScale(0.05),
    m_maxScale(50.0),
    m_panning(false),
    m_dragging(false)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::StrongFocus); // needed to receive key events
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlignment(Qt::AlignLeft | Qt::AlignBottom);
}
void Canvas::wheelEvent(QWheelEvent *event)
{
    qreal delta = event->angleDelta().y();
    if (delta == 0) {
        QGraphicsView::wheelEvent(event);
        return;
    }

    const double factorPerDegree = std::pow(1.0015, delta);
    double factor = factorPerDegree;

    double newScale = m_scale * factor;
    if (newScale < m_minScale) factor = m_minScale / m_scale;
    if (newScale > m_maxScale) factor = m_maxScale / m_scale;

    scale(factor, factor);
    m_scale *= factor;

    event->accept();
}
void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lastPanPoint = event->pos();
        m_clickPosScene = mapToScene(event->pos());
        setCursor(Qt::ClosedHandCursor);
        m_panning = true;
        m_dragging = false;
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}
void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning) {
        QPointF delta = event->pos() - m_lastPanPoint;
        if (!delta.isNull()) m_dragging = true;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - int(delta.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - int(delta.y()));
        m_lastPanPoint = event->pos();

        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}
void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_panning) {
        m_panning = false;
        setCursor(Qt::ArrowCursor);

        if (!m_dragging) {
            // It was a click, not a drag
            emit pointClicked(m_clickPosScene.x(), -m_clickPosScene.y());
        }

        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}
void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R) {
        fitAll();
        event->accept();
        return;
    }
    QGraphicsView::keyPressEvent(event);
}
void Canvas::drawBackground(QPainter *painter, const QRectF &rect)
{
    const int gridSize = 5;
    const qreal left = std::floor(rect.left()  / gridSize) * gridSize;
    const qreal top  = std::floor(rect.top()   / gridSize) * gridSize;

    QPen pen(Qt::lightGray);
    pen.setWidthF(0.0);
    painter->setPen(pen);

    for (qreal x = left; x < rect.right(); x += gridSize)
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));

    for (qreal y = top; y < rect.bottom(); y += gridSize)
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));

    // axes
    QPen axisPen(Qt::gray);
    axisPen.setWidth(0);
    painter->setPen(axisPen);
    painter->drawLine(QPointF(0, rect.top()), QPointF(0, rect.bottom()));
    painter->drawLine(QPointF(rect.left(), 0), QPointF(rect.right(), 0));
}
void Canvas::fitAll()
{
    resetTransform();
    QRectF bounds = scene()->itemsBoundingRect();
    if (bounds.isValid()) {
        fitInView(bounds, Qt::KeepAspectRatio);
        m_scale = transform().m11();
    }
    else{
        fitInView(QRectF(0, -50, 70, 50), Qt::KeepAspectRatio);
        m_scale = transform().m11();
    }
}

