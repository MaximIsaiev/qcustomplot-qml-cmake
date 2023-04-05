#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QtQuick>
#include <qcustomplot.h>
#include <rangeaxisticker.h>
class QCustomPlot;
class QCPAbstractPlottable;

struct FrequencyRangeImpl {
    QVector<double> ticks;
    QVector<QString> labels;
    double leftFrequency;
    double rightFrequency;
    double filter;
};

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    CustomPlotItem( QQuickItem* parent = 0 );
    virtual ~CustomPlotItem();

    void paint( QPainter* painter );

    Q_INVOKABLE void initCustomPlot();

    Q_INVOKABLE void addRange(int left, int right, int index);

    Q_INVOKABLE void paint();

    Q_INVOKABLE void addPost();

    Q_INVOKABLE void reset();

protected:
    void routeMouseEvents( QMouseEvent* event );
    void routeWheelEvents( QWheelEvent* event );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent *event );

    virtual void timerEvent(QTimerEvent *event);

private:
    QCustomPlot*         m_CustomPlot;
    QCPCurve*            verticalLine;
    int                  m_timerId;
    QCPGraph*            m_absGraph;
    QCPGraph*            m_phsGraph;
    std::vector<QCPAxisRect*> rows;
    QCPMarginGroup *m_group;
    std::vector<FrequencyRangeImpl> m_rangeData;

    void setStyleSheet(QCPAxisRect *rect);

private slots:
    void graphClicked( QCPAbstractPlottable* plottable );
    void onCustomReplot();
    void updateCustomPlotSize();
    void zoomSynchronization(const QCPRange & range);

};

#endif // QMLPLOT_H
