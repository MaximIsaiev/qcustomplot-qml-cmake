#include <QDebug>
#include <QtGlobal>
#include <QtConcurrent/QtConcurrent>
#include "include/qmlplot.h"
#include "include/qcustomplot.h"
#include "memory"

CustomPlotItem::CustomPlotItem( QQuickItem* parent ) : QQuickPaintedItem( parent )
  , m_CustomPlot( nullptr )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );
    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    connect( this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize );
    connect(&m_futureWatcher, &QFutureWatcher<void>::finished, this, [this](){
        emit needCustomReplot();
    });
}

CustomPlotItem::~CustomPlotItem()
{
    delete m_CustomPlot;
    m_CustomPlot = nullptr;
}

void CustomPlotItem::initCustomPlot()
{
    m_CustomPlot = new QCustomPlot();
    setOpenGl(false);

    auto graph1 = m_CustomPlot->addGraph(); // blue line
    graph1->setPen(QPen(QColor(40, 110, 255)));
    graph1->setBrush(QColor(40, 110, 255, 90));
    graph1->setAdaptiveSampling(true);
    graph1->setLineStyle(QCPGraph::lsLine);
    graph1->setAntialiasedFill(false);
    graph1->setScatterSkip(100);
    auto graph2 = m_CustomPlot->addGraph(); // red line
    graph2->setPen(QPen(QColor(255, 110, 40)));
    graph2->setBrush(QColor(255, 110, 40, 90));
    graph2->setAdaptiveSampling(true);
    graph2->setChannelFillGraph(graph1);

    m_CustomPlot->axisRect()->setupFullAxesBox();
    m_CustomPlot->yAxis->setRange(-3, 7);
    m_CustomPlot->xAxis->setRange(0, pointCount());
    m_CustomPlot->setNotAntialiasedElements(QCP::aeAll);

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(this, &CustomPlotItem::needData, this, &CustomPlotItem::realtimeDataSlot);
    connect(this, &CustomPlotItem::needCustomReplot, this, [this](){
        emit m_CustomPlot->afterReplot();
        dataTimer.start(0);
    });
    connect(&dataTimer, &QTimer::timeout, this, [this](){
        dataTimer.stop();
        emit needData();
    });
    // добавляем возможность делать зум и выбирать элементы
    m_CustomPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    m_CustomPlot->setSelectionRectMode(QCP::srmZoom);

    // ресайзим график
    updateCustomPlotSize();

    // после replot нужно обязательно вызывать update
    connect(m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot );
    m_CustomPlot->replot();
}

void CustomPlotItem::paint(bool paint)
{
    m_needPaint = paint;
    emit needData();

}


void CustomPlotItem::paint( QPainter* painter )
{
    if (m_CustomPlot)
    {
        QElapsedTimer timer;
        timer.start();

        if (m_pixmapPaint) {
            QPixmap pixmap (boundingRect().size().toSize());
            QCPPainter qcpPainter(&pixmap);
            m_CustomPlot->toPainter(&qcpPainter);
            painter->drawPixmap(0, 0, pixmap);
        } else {
            QPicture picture;
            QCPPainter qcpPainter;
            qcpPainter.begin(&picture);
            m_CustomPlot->toPainter(&qcpPainter);
            qcpPainter.end();
            painter->drawPicture(0, 0, picture);
        }

        setReplotTime(QString::number(m_CustomPlot->replotTime()));
        setRepaintTime(QString::number(timer.elapsed()));
    }
}

void CustomPlotItem::mousePressEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CustomPlotItem::mouseReleaseEvent( QMouseEvent* event )
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CustomPlotItem::mouseMoveEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CustomPlotItem::mouseDoubleClickEvent( QMouseEvent* event )
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CustomPlotItem::wheelEvent( QWheelEvent *event )
{
    routeWheelEvents( event );
}

void CustomPlotItem::routeMouseEvents( QMouseEvent* event )
{
    if (m_CustomPlot)
    {
        QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CustomPlotItem::routeWheelEvents( QWheelEvent* event )
{
    if (m_CustomPlot)
    {
        QWheelEvent* newEvent = new QWheelEvent( event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CustomPlotItem::updateCustomPlotSize()
{
    if (m_CustomPlot)
    {
        m_CustomPlot->setGeometry(0, 0, (int)width(), (int)height());
        m_CustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    }
}

void CustomPlotItem::onCustomReplot()
{
    update();
}

void CustomPlotItem::realtimeDataSlot()
{
    QFuture<void> future = QtConcurrent::run([this](){
    if (!m_needPaint)
        return;

    auto graph1 = m_CustomPlot->graph(0);
    auto graph2 = m_CustomPlot->graph(1);

    if (graph1->data()->size() != m_pointCount || graph2->data()->size() != m_pointCount) {
        m_keys.resize(m_pointCount);
        m_values.resize(m_pointCount);
        std::generate(m_keys.begin(), m_keys.end(), [n = 0.0]() mutable { return n += 1; });
        graph1->setData(m_keys, m_values, true);
        graph2->setData(m_keys, m_values, true);
        m_CustomPlot->xAxis->setRange(0, m_pointCount);
    }

    std::generate(m_values.begin(), m_values.end(), [key = 0.0] () mutable {
        key++;
        return qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843);
    });

    int i = 0;
    for (auto &it : *graph1->data()) {
        it.value = m_values.at(i);
        i++;
    }

    i = 0;
    for (auto &it : *graph2->data()) {
        it.value = m_values.at(i) + 2.0;
        i++;
    }

    });

    m_futureWatcher.setFuture(future);
}

const QString &CustomPlotItem::replotTime() const {
    return m_replotTime;
}

void CustomPlotItem::setReplotTime(const QString &replotTime) {
    CustomPlotItem::m_replotTime = replotTime;
    emit replotTimeChanged();
}

const QString &CustomPlotItem::repaintTime() const {
    return m_repaintTime;
}

void CustomPlotItem::setRepaintTime(const QString &repaintTime) {
    CustomPlotItem::m_repaintTime = repaintTime;
    emit repaintTimeChanged();  
}

bool CustomPlotItem::openGl() const
{
    if (m_CustomPlot)
        return m_CustomPlot->openGl();
    else
        return false;
}

void CustomPlotItem::setOpenGl(bool mOpenGl)
{
    m_CustomPlot->setOpenGl(mOpenGl);
    emit openGlChanged();
}

bool CustomPlotItem::pixmapPaint() const
{
    return m_pixmapPaint;
}

void CustomPlotItem::setPixmapPaint(bool mPixmapPaint)
{
    m_pixmapPaint = mPixmapPaint;
    emit pixmapPaintChanged();
}

int CustomPlotItem::pointCount() const
{
    return m_pointCount;
}

void CustomPlotItem::setPointCount(int mPointCount)
{
    m_pointCount = mPointCount;
    emit pointCountChanged();
}

void CustomPlotItem::replot()
{
    m_CustomPlot->replot();
}

