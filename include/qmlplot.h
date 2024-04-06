#pragma once

#include <QtQuick>
#include "include/qcustomplot.h"

class QCustomPlot;
class QCPAbstractPlottable;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString replotTime READ replotTime WRITE setReplotTime NOTIFY replotTimeChanged)
    Q_PROPERTY(QString repaintTime READ repaintTime WRITE setRepaintTime NOTIFY repaintTimeChanged)
    Q_PROPERTY(bool openGl READ openGl WRITE setOpenGl NOTIFY openGlChanged)
    Q_PROPERTY(bool pixmapPaint READ pixmapPaint WRITE setPixmapPaint NOTIFY pixmapPaintChanged)
    Q_PROPERTY(int pointCount READ pointCount WRITE setPointCount NOTIFY pointCountChanged)

public:
    explicit CustomPlotItem(QQuickItem* parent = nullptr);
    ~CustomPlotItem() override;

    void paint( QPainter* painter ) override;

    Q_INVOKABLE void replot();
    Q_INVOKABLE void initCustomPlot();

    Q_INVOKABLE void paint(bool paint);

    const QString &replotTime() const;
    void setReplotTime(const QString &replotTime);

    const QString &repaintTime() const;
    void setRepaintTime(const QString &repaintTime);

    bool openGl() const;
    void setOpenGl(bool mOpenGl);

    bool pixmapPaint() const;
    void setPixmapPaint(bool mPixmapPaint);

    int pointCount() const;
    void setPointCount(int mPointCount);

signals:
    void needData();
    void needCustomReplot();
    void replotTimeChanged();
    void repaintTimeChanged();
    void openGlChanged();
    void pixmapPaintChanged();
    void pointCountChanged();

protected:
    void routeMouseEvents( QMouseEvent* event );
    void routeWheelEvents( QWheelEvent* event );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent *event );

private:
    QCustomPlot* m_CustomPlot;
    QTimer dataTimer;
    QVector<double> m_keys;
    QVector<double> m_values;
    QString m_replotTime;
    QString m_repaintTime;
    QFutureWatcher<void> m_futureWatcher;
    int m_pointCount {1024};
    bool m_pixmapPaint {false};
    bool m_needPaint {true};

private slots:
    void onCustomReplot();
    void updateCustomPlotSize();
    void realtimeDataSlot();

};
