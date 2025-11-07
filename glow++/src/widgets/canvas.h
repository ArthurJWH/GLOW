#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QPointF>

class Canvas : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Canvas(QGraphicsScene *scene, QWidget *parent = nullptr);
    void fitAll();

signals:
    void pointClicked(double x, double y);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    double m_scale;
    double m_minScale;
    double m_maxScale;

    bool m_panning;
    bool m_dragging;
    QPoint m_lastPanPoint;
    QPointF m_clickPosScene; // for click detection
};

#endif // CANVAS_H


