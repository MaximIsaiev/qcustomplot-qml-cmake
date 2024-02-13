#ifndef QMLPLOT_H
#define QMLPLOT_H

#include <QtQuick>
#include "include/qcustomplot.h"

static const int size1 = 2000;

class QCustomPlot;
class QCPAbstractPlottable;

struct FrequencyRangeImpl {
    QVector<double> ticks;
    QVector<QString> labels;
    double leftFrequency;
    double rightFrequency;
    double filter;
};

class ThreadWorker : public QObject
{
public:
    explicit ThreadWorker(QObject *parent = nullptr) : QObject(parent) {}
    ~ThreadWorker() override = default;

    static void doWork(int i) {
        while (i < 10000) {
            i++;
            qDebug() << i;
        }
        qDebug() << "end";
    }

};

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString replotTime READ replotTime WRITE setReplotTime NOTIFY replotTimeChanged)
    Q_PROPERTY(QString repaintTime READ repaintTime WRITE setRepaintTime NOTIFY repaintTimeChanged)

public:
    CustomPlotItem( QQuickItem* parent = 0 );
    virtual ~CustomPlotItem();

    void paint( QPainter* painter );

    Q_INVOKABLE void initCustomPlot();

    Q_INVOKABLE void addRange(int left, int right);

    Q_INVOKABLE void paint();

    Q_INVOKABLE void addPost();

    Q_INVOKABLE void reset();

signals:
    void needWork(int i);
    void needData();
    void needCustomReplot();
    void replotTimeChanged();
    void repaintTimeChanged();

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
    QThread thread1;
    QThread thread2;
    std::unique_ptr<ThreadWorker> worker1;
    std::unique_ptr<ThreadWorker> worker2;
    QTimer dataTimer;
    QVector<double> m_keys;
    QVector<double> m_values;
    QPixmap grabPixmap;
    QString m_replotTime;
    QString m_repaintTime;
public:
    const QString &replotTime() const;

    void setReplotTime(const QString &replotTime);

    const QString &repaintTime() const;

    void setRepaintTime(const QString &repaintTime);

private:


    void setStyleSheet(QCPAxisRect *rect);

private slots:
    void graphClicked( QCPAbstractPlottable* plottable );
    void onCustomReplot();
    void updateCustomPlotSize();
    void zoomSynchronization(const QCPRange & range);
    void realtimeDataSlot();

};

#endif // QMLPLOT_H
