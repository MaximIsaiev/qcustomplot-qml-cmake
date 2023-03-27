#include "qmlplot.h"
#include "qcustomplot.h"
#include <QDebug>

CustomPlotItem::CustomPlotItem( QQuickItem* parent ) : QQuickPaintedItem( parent )
  , m_CustomPlot( nullptr ), m_timerId( 0 )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );

    connect( this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize );
}

CustomPlotItem::~CustomPlotItem()
{
    delete m_CustomPlot;
    m_CustomPlot = nullptr;

    if(m_timerId != 0) {
        killTimer(m_timerId);
    }
}

void CustomPlotItem::initCustomPlot()
{

    m_CustomPlot = new QCustomPlot();
    m_CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    m_CustomPlot->setBackground(plotGradient);

    m_CustomPlot->plotLayout()->clear();
    m_CustomPlot->setOpenGl(true);


    QSharedPointer<QCPAxisTicker> xTicker (new QCPAxisTicker);
    m_CustomPlot->plotLayout()->setRowSpacing(0);

    m_CustomPlot->plotLayout()->setMargins(QMargins(0,0,0,0));

    auto topRect = new QCPAxisRect(m_CustomPlot, true);
    m_CustomPlot->plotLayout()->addElement(0,0, topRect);

    topRect->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msTop);
    topRect->setMargins(QMargins(0, 0, 0, 0));

    topRect->axis(QCPAxis::atLeft)->setRange(-2,  50);

    topRect->axis(QCPAxis::atLeft)->setLabel("ABS");
    topRect->axis(QCPAxis::atLeft)->setLabelColor(Qt::white);
    topRect->axis(QCPAxis::atBottom)->setBasePen(QPen(Qt::white, 1));
    topRect->axis(QCPAxis::atLeft)->setBasePen(QPen(Qt::white, 1));
    topRect->axis(QCPAxis::atBottom)->setTickPen(QPen(Qt::white, 1));
    topRect->axis(QCPAxis::atLeft)->setTickPen(QPen(Qt::white, 1));

    topRect->axis(QCPAxis::atBottom)->setSubTickPen(QPen(Qt::white, 1));
    topRect->axis(QCPAxis::atLeft)->setSubTickPen(QPen(Qt::white, 1));
    topRect->axis(QCPAxis::atBottom)->setTickLabelColor(Qt::white);
    topRect->axis(QCPAxis::atLeft)->setTickLabelColor(Qt::white);
    topRect->axis(QCPAxis::atBottom)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    topRect->axis(QCPAxis::atLeft)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));

     topRect->axis(QCPAxis::atBottom)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    topRect->axis(QCPAxis::atLeft)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
     topRect->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(false);
    topRect->axis(QCPAxis::atLeft)->grid()->setSubGridVisible(false);
     topRect->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(Qt::NoPen);
    topRect->axis(QCPAxis::atLeft)->grid()->setZeroLinePen(Qt::NoPen);
//     topRect->axis(QCPAxis::atBottom)->setUpperEnding(QCPLineEnding::esSpikeArrow);
//    topRect->axis(QCPAxis::atLeft)->setUpperEnding(QCPLineEnding::esSpikeArrow);

//    auto spaceRect = new QCPAxisRect(m_CustomPlot, true);
//    m_CustomPlot->plotLayout()->addElement(1, 0, spaceRect);


//    spaceRect->setAutoMargins(QCP::msLeft);
//    spaceRect->setMargins(QMargins(0, 0, 0, 0));


//    spaceRect->axis(QCPAxis::atBottom)->setTicker(xTicker);
//    spaceRect->axis(QCPAxis::atBottom)->setBasePen(Qt::NoPen);
//    spaceRect->axis(QCPAxis::atBottom)->setTickLabels(false);
//    spaceRect->axis(QCPAxis::atBottom)->setTicks(false);

//    spaceRect->removeAxis(spaceRect->axis(QCPAxis::atLeft));
////    spaceRect->axis(QCPAxis::atLeft)->setTickLabels(false);
////    spaceRect->axis(QCPAxis::atLeft)->setTicks(false);

    auto bottomRect = new QCPAxisRect(m_CustomPlot, true);
    m_CustomPlot->plotLayout()->addElement(1,0, bottomRect);

    bottomRect->axis(QCPAxis::atLeft)->setRange(0,  104);

    bottomRect->axis(QCPAxis::atLeft)->setLabel("PHS");
    bottomRect->axis(QCPAxis::atLeft)->setLabelColor(Qt::white);

    bottomRect->setAutoMargins(QCP::msLeft | QCP::msBottom | QCP::msRight);
    bottomRect->setMargins(QMargins(0, 0, 0, 0));

    bottomRect->axis(QCPAxis::atBottom)->setBasePen(QPen(Qt::white, 1));
    bottomRect->axis(QCPAxis::atLeft)->setBasePen(QPen(Qt::white, 1));
    bottomRect->axis(QCPAxis::atBottom)->setTickPen(QPen(Qt::white, 1));
    bottomRect->axis(QCPAxis::atLeft)->setTickPen(QPen(Qt::white, 1));

    bottomRect->axis(QCPAxis::atBottom)->setSubTickPen(QPen(Qt::white, 1));
    bottomRect->axis(QCPAxis::atLeft)->setSubTickPen(QPen(Qt::white, 1));
    bottomRect->axis(QCPAxis::atBottom)->setTickLabelColor(Qt::white);
    bottomRect->axis(QCPAxis::atLeft)->setTickLabelColor(Qt::white);
    bottomRect->axis(QCPAxis::atBottom)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    bottomRect->axis(QCPAxis::atLeft)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));

     bottomRect->axis(QCPAxis::atBottom)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    bottomRect->axis(QCPAxis::atLeft)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
     bottomRect->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(false);
    bottomRect->axis(QCPAxis::atLeft)->grid()->setSubGridVisible(false);
     bottomRect->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(Qt::NoPen);
    bottomRect->axis(QCPAxis::atLeft)->grid()->setZeroLinePen(Qt::NoPen);
//     bottomRect->axis(QCPAxis::atBottom)->setUpperEnding(QCPLineEnding::esSpikeArrow);
//    bottomRect->axis(QCPAxis::atLeft)->setUpperEnding(QCPLineEnding::esSpikeArrow);


    topRect->axis(QCPAxis::atBottom)->setTicker(xTicker);
    bottomRect->axis(QCPAxis::atBottom)->setTicker(xTicker);

    topRect->axis(QCPAxis::atBottom)->setBasePen(Qt::NoPen);
    topRect->axis(QCPAxis::atBottom)->setTickLabels(false);
    topRect->axis(QCPAxis::atBottom)->setTicks(false);

    m_absGraph = m_CustomPlot->addGraph(topRect->axis(QCPAxis::atBottom), topRect->axis(QCPAxis::atLeft));
    m_phsGraph = m_CustomPlot->addGraph(bottomRect->axis(QCPAxis::atBottom), bottomRect->axis(QCPAxis::atLeft));

    auto pen = QPen(QColor(10, 140, 70, 255));
    pen.setWidth(2);
    m_absGraph->setPen(pen);
//    m_absGraph->setBrush(QColor(10, 140, 70, 160));

    m_phsGraph->setPen(pen);
//    m_phsGraph->setBrush(QColor(10, 140, 70, 160));

    QCPMarginGroup *group = new QCPMarginGroup(m_CustomPlot);
    topRect->setMarginGroup(QCP::msLeft, group);
    bottomRect->setMarginGroup(QCP::msLeft, group);
//    spaceRect->setMarginGroup(QCP::msLeft, group);

    connect(topRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), bottomRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
    topRect->axis(QCPAxis::atBottom)->setRange(0,  25);

    updateCustomPlotSize();

    // replot the plot widget
    m_CustomPlot->replot();

    startTimer(100);


    //    QLinearGradient gradient(0, 0, 0, m_CustomPlot->height());
    //    gradient.setColorAt(0, QColor(44, 77, 232));
    //    gradient.setColorAt(1, QColor(0, 143, 157));

    //    updateCustomPlotSize();
    //    m_CustomPlot->addGraph();
    //    QPen * pen = new QPen(Qt::green);
    //    pen->setWidth(2);
    //    m_CustomPlot->graph( 0 )->setPen(*pen );
    //    m_CustomPlot->setBackground(QBrush(gradient));
    //    m_CustomPlot->xAxis->setLabel( "t" );
    //    m_CustomPlot->yAxis->setLabel( "S" );
    //    m_CustomPlot->xAxis->setRange(0, 15);
    //    m_CustomPlot->yAxis->setRange( 0, 5 );
    //    m_CustomPlot->yAxis->grid()->setVisible(true);
    //    m_CustomPlot->yAxis->grid()->setPen(QPen( Qt::black));

    //    m_CustomPlot ->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );

    //    verticalLine = new QCPCurve(m_CustomPlot->xAxis, m_CustomPlot->yAxis);

    //    startTimer(100);

        connect( m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot );

    m_CustomPlot->replot();
}


void CustomPlotItem::paint( QPainter* painter )
{
    if (m_CustomPlot)
    {
        QPixmap    picture( boundingRect().size().toSize() );
        QCPPainter qcpPainter( &picture );

        m_CustomPlot->toPainter( &qcpPainter );

        painter->drawPixmap( QPoint(), picture );
    }
}

void CustomPlotItem::mousePressEvent( QMouseEvent* event )
{
    qDebug() << Q_FUNC_INFO;
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

void CustomPlotItem::timerEvent(QTimerEvent *event)
{
    static double t, U;
    U = ((double)rand() / RAND_MAX) * 50;

    m_absGraph->addData(t, U);
    m_phsGraph->addData(t, ((double)rand() / RAND_MAX) * 100);

    qDebug() << Q_FUNC_INFO << QString("Adding dot t = %1, S = %2").arg(t).arg(U);
    t+=0.1;

    auto range = m_CustomPlot->axisRect(0)->axis(QCPAxis::atBottom)->range();
    if (t > range.upper)
        m_CustomPlot->axisRect(0)->axis(QCPAxis::atBottom)->setRange(range.lower + (t - range.upper), t);
    m_CustomPlot->replot();
}

void CustomPlotItem::graphClicked( QCPAbstractPlottable* plottable )
{
    qDebug() << Q_FUNC_INFO << QString( "Clicked on graph '%1 " ).arg( plottable->name() );
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
    qDebug() << Q_FUNC_INFO;
    update();
}
