#include "qmlplot.h"
#include "qcustomplot.h"
#include "rangeaxisticker.h"
#include <QDebug>
#include <QtGlobal>>

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

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    m_CustomPlot->setBackground(plotGradient);
    m_CustomPlot->setInteractions(QCP::iRangeZoom | QCP::iSelectItems);

    m_CustomPlot->plotLayout()->clear();
    m_CustomPlot->plotLayout()->setWrap(0);
    m_CustomPlot->plotLayout()->setFillOrder(QCPLayoutGrid::foRowsFirst);

    QSharedPointer<QCPAxisTicker> xTicker (new QCPAxisTicker);

    m_CustomPlot->plotLayout()->setRowSpacing(0);
    m_CustomPlot->plotLayout()->setMargins(QMargins(0,0,0,0));

    m_group = new QCPMarginGroup(m_CustomPlot);

    //    auto newRange = new QCPAxisRect(m_CustomPlot);
    //    setStyleSheet(newRange);

    //    newRange->setRangeZoom(Qt::Horizontal);

    //    newRange->axis(QCPAxis::atLeft)->setBasePen(Qt::NoPen);
    //    newRange->axis(QCPAxis::atLeft)->setTickLabels(false);


    //    newRange->axis(QCPAxis::atBottom)->setRange(0, 5);
    //    auto graph = new QCPGraph(newRange->axis(QCPAxis::atBottom), newRange->axis(QCPAxis::atLeft));
    QLinearGradient gradient(0, 0, 0, m_CustomPlot->height());
    gradient.setColorAt(0, QColor(8, 90, 77));
    gradient.setColorAt(1, QColor(0,0,0,0));

    //    graph->setBrush(QBrush(gradient));
    //    graph->setPen(QPen(QColor(53, 164, 139)));

    //    m_CustomPlot->plotLayout()->addElement(newRange);
    //    topRect->setMarginGroup(QCP::msLeft, group);
    //    bottomRect->setMarginGroup(QCP::msLeft, group);
    //    rightTopRect->setMarginGroup(QCP::msLeft, group);
    //    //    spaceRect->setMarginGroup(QCP::msLeft, group);

    //    auto topRect = new QCPAxisRect(m_CustomPlot, true);
    //    m_CustomPlot->plotLayout()->addElement(0,0, topRect);

    //    topRect->setAutoMargins(QCP::msAll);
    //    topRect->setMargins(QMargins(0, 0, 0, 0));

    //    topRect->axis(QCPAxis::atLeft)->setRange(0,  100);

    //    topRect->axis(QCPAxis::atLeft)->setLabel("ABS");
    //    setStyleSheet(topRect);

    //    auto bottomRect = new QCPAxisRect(m_CustomPlot, true);
    //    m_CustomPlot->plotLayout()->addElement(1,0, bottomRect);

    //    setStyleSheet(bottomRect);

    //    bottomRect->axis(QCPAxis::atLeft)->setRange(0,  100);

    //    bottomRect->axis(QCPAxis::atLeft)->setLabel("PHS");
    //    bottomRect->axis(QCPAxis::atLeft)->setLabelColor(Qt::white);

    //    bottomRect->setAutoMargins(QCP::msLeft | QCP::msBottom | QCP::msRight);
    //    bottomRect->setMargins(QMargins(0, 0, 0, 0));

    //    topRect->axis(QCPAxis::atBottom)->setTicker(xTicker);
    //    bottomRect->axis(QCPAxis::atBottom)->setTicker(xTicker);

    //    auto rightTopRect = new QCPAxisRect(m_CustomPlot);
    //    m_CustomPlot->plotLayout()->addElement(0, 1, rightTopRect);

    //    setStyleSheet(rightTopRect);

    //    rightTopRect->axis(QCPAxis::atBottom)->setTicker(xTicker);

    //    rightTopRect->setAutoMargins(QCP::msBottom | QCP::msRight | QCP::msTop);
    //    rightTopRect->setMargins(QMargins(0, 0, 0, 0));

    //    m_absGraph = m_CustomPlot->addGraph(topRect->axis(QCPAxis::atBottom), topRect->axis(QCPAxis::atLeft));
    //    m_phsGraph = m_CustomPlot->addGraph(bottomRect->axis(QCPAxis::atBottom), bottomRect->axis(QCPAxis::atLeft));

    //    auto pen = QPen(QColor(10, 140, 70, 255));
    //    pen.setWidth(2);
    //    m_absGraph->setPen(pen);
    //    //    m_absGraph->setBrush(QColor(10, 140, 70, 160));

    //    m_phsGraph->setPen(pen);
    //    //    m_phsGraph->setBrush(QColor(10, 140, 70, 160));



    ////    connect(topRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), bottomRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
    ////    topRect->axis(QCPAxis::atBottom)->setRange(0,  25);

    updateCustomPlotSize();

    // replot the plot widget
    m_CustomPlot->replot();

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

    connect(m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot );

    m_CustomPlot->replot();
}

void CustomPlotItem::addRange(int left, int right, int index)
{

    if (m_CustomPlot->plotLayout()->elementCount() == 0) {
        auto newRange = new QCPAxisRect(m_CustomPlot);
        setStyleSheet(newRange);

        newRange->setRangeZoom(Qt::Horizontal);

        newRange->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
        newRange->setMargins(QMargins(0, 5, 0, 0));

        newRange->axis(QCPAxis::atLeft)->setBasePen(Qt::NoPen);
        newRange->axis(QCPAxis::atLeft)->setTickLabels(false);

        auto ticker = QSharedPointer<QCPAxisTickerText>(new QCPAxisTickerText());
        newRange->axis(QCPAxis::atBottom)->setTicker(ticker);

        QVector<double> newTicks;
        QVector<QString> newLabels;

        for (auto i = left; i <= right; i += 5) {
            newTicks.push_back(i - left);
            newLabels.push_back(QString::number(i));
        }

        ticker->setTicks(newTicks, newLabels);

        newRange->axis(QCPAxis::atBottom)->setRange(newTicks.first(), newTicks.back());

        connect(newRange->axis(QCPAxis::atBottom), static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this, [this](const QCPRange & range) {
            if (range.lower < m_rangeData.front().ticks.first())
                reset();
        });

        m_CustomPlot->plotLayout()->addElement(newRange);
        rows.push_back(newRange);
        m_CustomPlot->xAxis->setRange(newTicks.first(), newTicks.back());


        QCPItemRect *rectLegend = new QCPItemRect(m_CustomPlot);
        rectLegend->bottomRight->setCoords(newTicks.back(), newRange->axis(QCPAxis::atLeft)->range().upper - 0.5);
        rectLegend->topLeft->setCoords(newTicks.first(), newRange->axis(QCPAxis::atLeft)->range().upper);
        rectLegend->setBrush(QBrush(Qt::white));
        rectLegend->setPen(QPen(Qt::black));

        QCPItemText *textLabel = new QCPItemText(m_CustomPlot);
        textLabel->position->setCoords(newTicks.first() + (newTicks.back() - newTicks.first()) / 2, newRange->axis(QCPAxis::atLeft)->range().upper - 0.25);
        textLabel->setText(QString::number(left) + "-" + QString::number(right));
        textLabel->setBrush(QBrush(Qt::NoBrush));
        textLabel->setPen(QPen(Qt::NoPen)); // show black border around text

        FrequencyRangeImpl newRangeImpl;
        newRangeImpl.ticks = newTicks;
        newRangeImpl.labels = newLabels;
        newRangeImpl.leftFrequency = left;
        newRangeImpl.rightFrequency = right;

        m_rangeData.push_back(newRangeImpl);

        m_CustomPlot->replot();

        return;
    }



    QSharedPointer<QCPAxisTickerText> ticker (new QCPAxisTickerText);

    auto oldTicks = rows.front()->axis(QCPAxis::atBottom)->tickVector();
    auto oldLabels = rows.front()->axis(QCPAxis::atBottom)->tickVectorLabels();

    QVector<double> newTicks;
    QVector<QString> newLabels;

    for (auto i = left; i <= right; i += 5) {
        newTicks.push_back(i - left + oldTicks.back());
        newLabels.push_back(QString::number(i));
    }

    ticker->addTicks(oldTicks, oldLabels);
    ticker->addTicks(newTicks, newLabels);

    FrequencyRangeImpl newRangeImpl;
    newRangeImpl.ticks = newTicks;
    newRangeImpl.labels = newLabels;
    newRangeImpl.leftFrequency = left;
    newRangeImpl.rightFrequency = right;

    m_rangeData.push_back(newRangeImpl);

    for (auto && rect : rows) {

        QCPItemLine *line = new QCPItemLine(m_CustomPlot);

        line->start->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
        line->start->setCoords(oldTicks.back(), rect->axis(QCPAxis::atLeft)->range().lower);

        line->end->setCoords(oldTicks.back(), rect->axis(QCPAxis::atLeft)->range().upper);
        line->end->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));

        line->setClipAxisRect(rect);
        line->setPen(QPen(Qt::green));

        QCPItemRect *rectLegend = new QCPItemRect(m_CustomPlot);
        rectLegend->bottomRight->setCoords(newTicks.back(), rect->axis(QCPAxis::atLeft)->range().upper - 0.5);
        rectLegend->bottomRight->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));

        rectLegend->topLeft->setCoords(newTicks.first(), rect->axis(QCPAxis::atLeft)->range().upper);
        rectLegend->topLeft->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));

        rectLegend->setClipAxisRect(rect);
        rectLegend->setBrush(QBrush(Qt::white));
        rectLegend->setPen(QPen(Qt::black));

        QCPItemText *textLabel = new QCPItemText(m_CustomPlot);
        textLabel->position->setCoords(newTicks.first() + (right - left) / 2, rect->axis(QCPAxis::atLeft)->range().upper - 0.25);
        textLabel->position->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));

        textLabel->setClipAxisRect(rect);
        textLabel->setText(QString::number(left) + "-" + QString::number(right));
        textLabel->setBrush(QBrush(Qt::NoBrush));
        textLabel->setPen(QPen(Qt::NoPen));

        rect->axis(QCPAxis::atBottom)->setTicker(ticker);
        rect->axis(QCPAxis::atBottom)->setRange(oldTicks.first(), newTicks.back());
    }


    m_CustomPlot->replot();

}

void CustomPlotItem::paint()
{
    for (auto && rect : rows) {
        auto graphs = rect->axis(QCPAxis::atBottom)->graphs();

        for (auto && graph : graphs) {
            graph->data()->clear();
        }

        for (auto && range : m_rangeData) {
            QVector<double> keys;
            QVector<double> values;
            //    auto rect = new QCPAxisRect(m_CustomPlot);
            auto graph = new QCPGraph(rect->axis(QCPAxis::atBottom), rect->axis(QCPAxis::atLeft));

            graph->setPen(QPen(Qt::green));
            graph->setBrush(QBrush(QColor(0, 255, 0, 20)));

            for (double i = range.ticks.first(); i <= range.ticks.back(); i+=0.1) {
                keys.push_back(i);
                values.push_back((sin((i - range.ticks.first() + range.leftFrequency) / 6.28) * 2) + 2);

                if (values.back() >= 3.9999) {
                    QCPItemLine *marker = new QCPItemLine(m_CustomPlot);

                    marker->start->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
                    marker->start->setCoords(keys.back(), 0);

                    marker->end->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
                    marker->end->setCoords(keys.back(), values.back());

                    marker->setClipAxisRect(rect);

                    marker->setHead(QCPLineEnding(QCPLineEnding::EndingStyle::esSpikeArrow, 8, 10, true));
                    marker->setPen(QPen(Qt::yellow, 1, Qt::DashDotLine));
                    marker->setSelectable(true);
                }
            }

            graph->setData(keys, values);


        }
    }
    m_CustomPlot->replot();
}

void CustomPlotItem::addPost()
{
    auto newPost = new QCPAxisRect(m_CustomPlot);
    rows.push_back(newPost);

    setStyleSheet(newPost);

    newPost->setRangeZoom(Qt::Horizontal);

    newPost->axis(QCPAxis::atLeft)->setBasePen(Qt::NoPen);
    newPost->axis(QCPAxis::atLeft)->setTickLabels(false);

    newPost->axis(QCPAxis::atBottom)->setTickLabels(false);

    newPost->setAutoMargins(QCP::msLeft | QCP::msRight);
    newPost->setMargins(QMargins(0, 5, 0, 0));

    newPost->axis(QCPAxis::atBottom)->setTicker(rows.front()->axis(QCPAxis::atBottom)->ticker());
    newPost->axis(QCPAxis::atBottom)->setRange(rows.front()->axis(QCPAxis::atBottom)->range());

    for (auto && range : m_rangeData) {

        //        QCPItemLine *line = new QCPItemLine(m_CustomPlot);
        //        //        line->setParent(newPost);
        //        //        line->start->setAxisRect(newPost);
        //        //        line->end->setAxisRect(newPost);
        //        //        line->start->setCoords(range.ticks.back(), newPost->axis(QCPAxis::atLeft)->coordToPixel(0));
        //        //        line->end->setCoords(range.ticks.back(), newPost->axis(QCPAxis::atLeft)->coordToPixel(4));
        //        line->end->setParentAnchor(mDummyTracer->position);
        //        line->start->setParentAnchor(line->end);
        //        line->start->setCoords(15, 0);
        //        line->setPen(QPen(Qt::red));

        //        QCPItemRect *rectLegend = new QCPItemRect(m_CustomPlot);
        //        rectLegend->bottomRight->setCoords(range.ticks.back(), newPost->axis(QCPAxis::atLeft)->range().upper - 0.5);
        //        rectLegend->topLeft->setCoords(range.ticks.first(), newPost->axis(QCPAxis::atLeft)->range().upper);
        //        rectLegend->setBrush(QBrush(Qt::white));
        //        rectLegend->setPen(QPen(Qt::black));

        QCPItemRect *   xRectItem = new QCPItemRect( m_CustomPlot );

        xRectItem->setVisible          (true);
        xRectItem->setBrush(QBrush(Qt::white));
        xRectItem->setPen(QPen(Qt::black));

        xRectItem->topLeft     ->setAxes(  newPost->axis(QCPAxis::atBottom),  newPost->axis(QCPAxis::atLeft) );
        xRectItem->topLeft     ->setCoords( range.ticks.first(), newPost->axis(QCPAxis::atLeft)->range().upper);

        xRectItem->bottomRight ->setAxes( newPost->axis(QCPAxis::atBottom), newPost->axis(QCPAxis::atLeft) );
        xRectItem->bottomRight ->setCoords( range.ticks.back(), newPost->axis(QCPAxis::atLeft)->range().upper - 0.5 );

        xRectItem->setClipAxisRect     ( newPost );

        QCPItemLine *line = new QCPItemLine(m_CustomPlot);
        line->setVisible(true);
        line->start->setAxes(newPost->axis(QCPAxis::atBottom),  newPost->axis(QCPAxis::atLeft));
        line->start->setCoords(range.ticks.back(), newPost->axis(QCPAxis::atLeft)->range().lower);

        line->end->setAxes(newPost->axis(QCPAxis::atBottom),  newPost->axis(QCPAxis::atLeft));
        line->end->setCoords(range.ticks.back(), newPost->axis(QCPAxis::atLeft)->range().upper);

        line->setClipAxisRect(newPost);

        line->setPen(QPen(Qt::green));

        QCPItemText *textLabel = new QCPItemText(m_CustomPlot);
        textLabel->setVisible(true);

        textLabel->position->setAxes(newPost->axis(QCPAxis::atBottom),  newPost->axis(QCPAxis::atLeft));
        textLabel->position->setCoords(range.ticks.first() + (range.rightFrequency - range.leftFrequency) / 2, newPost->axis(QCPAxis::atLeft)->range().upper - 0.25);
        textLabel->setText(QString::number(range.leftFrequency) + "-" + QString::number(range.rightFrequency));

        textLabel->setClipAxisRect(newPost);
        textLabel->setBrush(QBrush(Qt::NoBrush));
        textLabel->setPen(QPen(Qt::NoPen));
    }

    //    newPost->axis(QCPAxis::atBottom)->setRange(left, right);
    //    auto graph = new QCPGraph(newRange->axis(QCPAxis::atBottom), newRange->axis(QCPAxis::atLeft));
    //    QLinearGradient gradient(0, 0, 0, m_CustomPlot->height());
    //    gradient.setColorAt(0, QColor(8, 90, 77));
    //    gradient.setColorAt(1, QColor(0,0,0,0));
    //    graph->setBrush(QBrush(gradient));
    //    graph->setPen(QPen(QColor(53, 164, 139)));

    for (size_t i = 0; i < rows.size(); i++) {
        m_CustomPlot->plotLayout()->addElement(rows.at(i));
    }

    connect(newPost->axis(QCPAxis::atBottom), static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this, &CustomPlotItem::zoomSynchronization);

    m_CustomPlot->replot();
}

void CustomPlotItem::reset()
{
    zoomSynchronization(QCPRange(m_rangeData.front().ticks.first(), m_rangeData.back().ticks.back()));
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
    if (event->button() == Qt::RightButton)
        reset();

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

void CustomPlotItem::setStyleSheet(QCPAxisRect *rect)
{
    rect->axis(QCPAxis::atLeft)->setLabelColor(Qt::white);
    rect->axis(QCPAxis::atBottom)->setBasePen(QPen(Qt::white, 1));
    rect->axis(QCPAxis::atLeft)->setBasePen(Qt::NoPen);
    rect->axis(QCPAxis::atLeft)->setTickLabels(false);

    rect->axis(QCPAxis::atBottom)->setSubTickPen(QPen(Qt::white, 1));
    rect->axis(QCPAxis::atLeft)->setSubTickPen(Qt::NoPen);
    rect->axis(QCPAxis::atBottom)->setTickLabelColor(Qt::white);
    rect->axis(QCPAxis::atLeft)->setTickLabelColor(Qt::white);
    rect->axis(QCPAxis::atBottom)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    rect->axis(QCPAxis::atLeft)->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));

    rect->axis(QCPAxis::atBottom)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    rect->axis(QCPAxis::atLeft)->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    rect->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(false);
    rect->axis(QCPAxis::atLeft)->grid()->setSubGridVisible(false);
    rect->axis(QCPAxis::atBottom)->grid()->setZeroLinePen(Qt::NoPen);
    rect->axis(QCPAxis::atLeft)->grid()->setZeroLinePen(Qt::NoPen);
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

void CustomPlotItem::zoomSynchronization(const QCPRange &range)
{
    for (auto && row: rows)
        row->axis(QCPAxis::atBottom)->setRange(range);
}

void CustomPlotItem::onCustomReplot()
{
    qDebug() << Q_FUNC_INFO;
    update();
}
