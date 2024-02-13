#define QCUSTOMPLOT_USE_OPENGL

#include <QDebug>
#include <QtGlobal>
#include "include/qmlplot.h"
#include "include/qcustomplot.h"
#include "memory"

class FreqAxisTicker : public QCPAxisTicker
{

public:
    FreqAxisTicker() = default;
    ~FreqAxisTicker() override = default;

    void setRanges(QVector<QCPRange> &ranges);
    double getTickStep(const QCPRange &range) override;
    QVector<QString> createLabelVector(const QVector<double> &ticks, const QLocale &locale, QChar formatChar, int precision) override;
    QString labelByTick(double tick);

    QVector<QCPRange> ranges;
    QVector<QCPRange> rangeToKeys;
    double sum;
};



QVector<QString>
FreqAxisTicker::createLabelVector(const QVector<double> &ticks, const QLocale &locale, QChar formatChar,
                                  int precision) {

    QVector<QString> newLabels;
    for (auto tick : ticks) {
        newLabels.push_back(labelByTick(tick));
    }
    return newLabels;
}

void FreqAxisTicker::setRanges(QVector<QCPRange> &ranges)
{
    this->ranges = ranges;

    auto sum = 0.0;
    for (auto &&range : ranges) {
        sum += range.size();
    }

    this->sum = sum;

    double left = 0.0;
    for (auto &range : ranges) {
        auto right = (range.size() / sum * 100) + left;
        rangeToKeys.push_back(QCPRange(left, right));
        left = right;
    }

    qDebug() << rangeToKeys;
}

QString FreqAxisTicker::labelByTick(double tick)
{
    for (auto i = 0; i < this->ranges.size(); ++i) {
        auto freqRange = this->ranges[i];
        auto keyRange = this->rangeToKeys[i];

        if (!keyRange.contains(tick))
            continue;

        auto left = keyRange.lower;
        auto right = keyRange.upper;

        qDebug() << "Для тика: " << tick << "\n"
        << "Дипазон тиков: " << keyRange << "\n"
        << "Дипазон частот: " << freqRange << "\n";

        auto offsetKey = tick - left;
        auto freqOffset = offsetKey * freqRange.size() / keyRange.size();
        auto freq = freqOffset + freqRange.lower;
        qDebug() << "Частота смещенная: " << freqOffset << "\n"
        << "Частота: " << freq;

        return QString::number(freq);
    }

    return {};
}

double FreqAxisTicker::getTickStep(const QCPRange &range) {
    return QCPAxisTicker::getTickStep(range);
}

CustomPlotItem::CustomPlotItem( QQuickItem* parent ) : QQuickPaintedItem( parent )
  , m_CustomPlot( nullptr ), m_timerId( 0 )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );
    setRenderTarget(QQuickPaintedItem::FramebufferObject);

    connect( this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize );

//    worker1 = std::unique_ptr<ThreadWorker>(new ThreadWorker());
//    worker2 = std::unique_ptr<ThreadWorker>(new ThreadWorker());
//    worker1->moveToThread(&thread1);
//    worker2->moveToThread(&thread2);
//    thread1.start();
//    thread2.start();
//
//    connect(this, &CustomPlotItem::needWork, worker1.get(), &ThreadWorker::doWork);
//    connect(this, &CustomPlotItem::needWork, worker2.get(), &ThreadWorker::doWork);
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
    m_CustomPlot->setOpenGl(true);
    m_CustomPlot->setPlottingHint(QCP::phFastPolylines);

    m_absGraph = m_CustomPlot->addGraph(); // blue line
    m_absGraph->setPen(QPen(QColor(40, 110, 255)));
    m_absGraph->setBrush(QColor(40, 110, 255, 90));
    m_absGraph->setAdaptiveSampling(true);
    m_absGraph->setLineStyle(QCPGraph::lsLine);
    m_absGraph->setAntialiasedFill(false);
    m_absGraph->setScatterSkip(100);
    m_CustomPlot->addGraph(); // red line
    m_CustomPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
    m_CustomPlot->graph(1)->setBrush(QColor(255, 110, 40, 90));
    m_CustomPlot->graph(1)->setAdaptiveSampling(true);
    m_CustomPlot->graph(1)->setChannelFillGraph(m_absGraph);

//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    m_CustomPlot->xAxis->setTicker(timeTicker);
    m_CustomPlot->axisRect()->setupFullAxesBox();
    m_CustomPlot->yAxis->setRange(-1.2, 3);
    m_CustomPlot->xAxis->setRange(0, size1);
    m_CustomPlot->setNotAntialiasedElements(QCP::aeAll);

    // make left and bottom axes transfer their ranges to right and top axes:
//    connect(m_CustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_CustomPlot->xAxis2, SLOT(setRange(QCPRange)));
//    connect(m_CustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_CustomPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(this, &CustomPlotItem::needData, this, &CustomPlotItem::realtimeDataSlot);
    connect(this, &CustomPlotItem::needCustomReplot, this, [this](){
        m_CustomPlot->replot(QCustomPlot::rpQueuedReplot);
        dataTimer.start(0);
    });
    connect(&dataTimer, &QTimer::timeout, this, [this](){
        dataTimer.stop();
        emit needData();
    });
//    dataTimer.start(0); // Interval 0 means to refresh as fast as possible

//    // фон в виде градиента
//    QLinearGradient plotGradient;
//    plotGradient.setStart(0, 0);
//    plotGradient.setFinalStop(0, 350);
//    plotGradient.setColorAt(0, QColor(80, 80, 80));
//    plotGradient.setColorAt(1, QColor(50, 50, 50));
//    m_CustomPlot->setBackground(plotGradient);
//
//    // добавляем возможность делать зум и выбирать элементы
    m_CustomPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
//
//    // избавляемся от графика который строиться по дефолту
//    m_CustomPlot->plotLayout()->clear();
//
//    // говорим что новые layout элементы будут добавляться как новые строки (друг под дургом)
//    m_CustomPlot->plotLayout()->setFillOrder(QCPLayoutGrid::foRowsFirst);
//    // если вместо нуля поставить другое число, например 5, то после 5 добавленного элемента,
//    // новые элементы будут добавляться в следующий столбец
//    m_CustomPlot->plotLayout()->setWrap(0);
//
//    // убираем отступы между элементами
//    m_CustomPlot->plotLayout()->setRowSpacing(0);
//    m_CustomPlot->plotLayout()->setMargins(QMargins(0,0,0,0));
//
//    // создаем группы оступов, чтобы все графики одинаково выравнивались
//    m_group = new QCPMarginGroup(m_CustomPlot);

    // ресайзим график
    updateCustomPlotSize();

    // после replot нужно обязательно вызывать update
    connect(m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot );
    m_CustomPlot->replot();
    qDebug() << "OPENGL - " << m_CustomPlot->openGl();

}

void CustomPlotItem::addRange(int left, int right)
{
    // создаем rect который в дальнейшем будем дробить на диапазоны
        auto newRange = new QCPAxisRect(m_CustomPlot);
        setStyleSheet(newRange);

        newRange->setRangeZoom(Qt::Horizontal);

        newRange->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
        newRange->setMargins(QMargins(0, 5, 0, 0));

        newRange->axis(QCPAxis::atLeft)->setBasePen(Qt::NoPen);
        newRange->axis(QCPAxis::atLeft)->setTickLabels(false);

        // создаем тикер для подписей к оси Х
        auto ticker = QSharedPointer<FreqAxisTicker>(new FreqAxisTicker());
        QVector<QCPRange> ranges = {{100, 150}, {190, 200}, {210, 230}};
        ticker->setRanges(ranges);
        ticker->setTickCount(10);
        newRange->axis(QCPAxis::atBottom)->setTicker(ticker);

        newRange->axis(QCPAxis::atBottom)->setRange(0, 100);

        //     добавили rect в layout
        m_CustomPlot->plotLayout()->addElement(newRange);

        // храним этот rect для дальнейшего взаимодействия
        rows.push_back(newRange);

        // rect с подписью диапазонов
        for (auto i = 0; i < ranges.size(); ++i) {
            auto rectLegend = new QCPItemRect(m_CustomPlot);
            rectLegend->bottomRight->setCoords(ticker->rangeToKeys[i].upper, newRange->axis(QCPAxis::atLeft)->range().upper - 0.5);
            rectLegend->topLeft->setCoords(ticker->rangeToKeys[i].lower, newRange->axis(QCPAxis::atLeft)->range().upper);
            rectLegend->setBrush(QBrush(Qt::white));
            rectLegend->setPen(QPen(Qt::black));

            QCPItemLine *marker = new QCPItemLine(m_CustomPlot);
            marker->start->setAxes(newRange->axis(QCPAxis::atBottom),  newRange->axis(QCPAxis::atLeft));
            marker->start->setCoords(ticker->rangeToKeys[i].upper, 0);
            marker->end->setAxes(newRange->axis(QCPAxis::atBottom),  newRange->axis(QCPAxis::atLeft));
            marker->end->setCoords(ticker->rangeToKeys[i].upper, newRange->axis(QCPAxis::atLeft)->range().upper);
            marker->setClipAxisRect(newRange);
            marker->setHead(QCPLineEnding(QCPLineEnding::EndingStyle::esSpikeArrow, 8, 10, true));
            marker->setPen(QPen(Qt::yellow, 1, Qt::DashDotLine));
            marker->setSelectable(true);

            // структура для хранения данных о диапазоне
            FrequencyRangeImpl newRangeImpl;
            newRangeImpl.ticks = {ticker->rangeToKeys[i].lower, ticker->rangeToKeys[i].upper};
            newRangeImpl.labels = {"", ""};
            newRangeImpl.leftFrequency = ranges[i].lower;
            newRangeImpl.rightFrequency = ranges[i].upper;

            m_rangeData.push_back(newRangeImpl);
        }

        emit needWork(100);

//    // для первого элемента другая логика
//    if (m_CustomPlot->plotLayout()->elementCount() == 0) {
//
//        // создаем rect который в дальнейшем будем дробить на диапазоны
//        auto newRange = new QCPAxisRect(m_CustomPlot);
//        setStyleSheet(newRange);
//
//        newRange->setRangeZoom(Qt::Horizontal);
//
//        newRange->setAutoMargins(QCP::msLeft | QCP::msRight | QCP::msBottom);
//        newRange->setMargins(QMargins(0, 5, 0, 0));
//
//        newRange->axis(QCPAxis::atLeft)->setBasePen(Qt::NoPen);
//        newRange->axis(QCPAxis::atLeft)->setTickLabels(false);
//
//        // создаем тикер для подписей к оси Х
//        auto ticker = QSharedPointer<QCPAxisTickerText>(new QCPAxisTickerText());
//        newRange->axis(QCPAxis::atBottom)->setTicker(ticker);
//
//        QVector<double> newTicks;
//        QVector<QString> newLabels;
//
//        // заполняем подписи так как нам нужно
//        for (auto i = left; i <= right; i += 5) {
//            newTicks.push_back(i - left);
//            newLabels.push_back(QString::number(i));
//        }
//
//        ticker->setTicks(newTicks, newLabels);
//
//        newRange->axis(QCPAxis::atBottom)->setRange(newTicks.first(), newTicks.back());
//
//        // сбрасываем зум если вышли за пределы диапазона
//        connect(newRange->axis(QCPAxis::atBottom), static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this, [this](const QCPRange & range) {
//            if (range.lower < m_rangeData.front().ticks.first())
//                reset();
//            zoomSynchronization(range);
//        });
//
//        // добавили rect в layout
//        m_CustomPlot->plotLayout()->addElement(newRange);
//
//        // храним этот rect для дальнейшего взаимодействия
//        rows.push_back(newRange);
//
//        m_CustomPlot->xAxis->setRange(newTicks.first(), newTicks.back());
//
//        // rect с подписью диапазонов
//        QCPItemRect *rectLegend = new QCPItemRect(m_CustomPlot);
//        rectLegend->bottomRight->setCoords(newTicks.back(), newRange->axis(QCPAxis::atLeft)->range().upper - 0.5);
//        rectLegend->topLeft->setCoords(newTicks.first(), newRange->axis(QCPAxis::atLeft)->range().upper);
//        rectLegend->setBrush(QBrush(Qt::white));
//        rectLegend->setPen(QPen(Qt::black));
//
//        QCPItemText *textLabel = new QCPItemText(m_CustomPlot);
//        textLabel->position->setCoords(newTicks.first() + (newTicks.back() - newTicks.first()) / 2, newRange->axis(QCPAxis::atLeft)->range().upper - 0.25);
//        textLabel->setText(QString::number(left) + "-" + QString::number(right));
//        textLabel->setBrush(QBrush(Qt::NoBrush));
//        textLabel->setPen(QPen(Qt::NoPen)); // show black border around text
//

//
//        m_CustomPlot->replot();
//
//        return;
//    }
//
//    QSharedPointer<QCPAxisTickerText> ticker (new QCPAxisTickerText);
//
//    auto oldTicks = rows.front()->axis(QCPAxis::atBottom)->tickVector();
//    auto oldLabels = rows.front()->axis(QCPAxis::atBottom)->tickVectorLabels();
//
//    QVector<double> newTicks;
//    QVector<QString> newLabels;
//
//    for (auto i = left; i <= right; i += 5) {
//        newTicks.push_back(i - left + oldTicks.back());
//        newLabels.push_back(QString::number(i));
//    }
//
//    ticker->addTicks(oldTicks, oldLabels);
//    ticker->addTicks(newTicks, newLabels);
//
//    FrequencyRangeImpl newRangeImpl;
//    newRangeImpl.ticks = newTicks;
//    newRangeImpl.labels = newLabels;
//    newRangeImpl.leftFrequency = left;
//    newRangeImpl.rightFrequency = right;
//
//    m_rangeData.push_back(newRangeImpl);
//
//    // если несколько "постов" добавляем диапазон для каждого
//    for (auto && rect : rows) {
//
//        QCPItemLine *line = new QCPItemLine(m_CustomPlot);
//
//        line->start->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
//        line->start->setCoords(oldTicks.back(), rect->axis(QCPAxis::atLeft)->range().lower);
//
//        line->end->setCoords(oldTicks.back(), rect->axis(QCPAxis::atLeft)->range().upper);
//        line->end->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
//
//        line->setClipAxisRect(rect);
//        line->setPen(QPen(Qt::green));
//
//        QCPItemRect *rectLegend = new QCPItemRect(m_CustomPlot);
//        rectLegend->bottomRight->setCoords(newTicks.back(), rect->axis(QCPAxis::atLeft)->range().upper - 0.5);
//        rectLegend->bottomRight->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
//
//        rectLegend->topLeft->setCoords(newTicks.first(), rect->axis(QCPAxis::atLeft)->range().upper);
//        rectLegend->topLeft->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
//
//        rectLegend->setClipAxisRect(rect);
//        rectLegend->setBrush(QBrush(Qt::white));
//        rectLegend->setPen(QPen(Qt::black));
//
//        QCPItemText *textLabel = new QCPItemText(m_CustomPlot);
//        textLabel->position->setCoords(newTicks.first() + (right - left) / 2, rect->axis(QCPAxis::atLeft)->range().upper - 0.25);
//        textLabel->position->setAxes(rect->axis(QCPAxis::atBottom),  rect->axis(QCPAxis::atLeft));
//
//        textLabel->setClipAxisRect(rect);
//        textLabel->setText(QString::number(left) + "-" + QString::number(right));
//        textLabel->setBrush(QBrush(Qt::NoBrush));
//        textLabel->setPen(QPen(Qt::NoPen));
//
//        rect->axis(QCPAxis::atBottom)->setTicker(ticker);
//        rect->axis(QCPAxis::atBottom)->setRange(oldTicks.first(), newTicks.back());
//    }

    m_CustomPlot->replot();
}

void CustomPlotItem::paint()
{
    emit needData();
    return;
    // для каждого поста
    for (auto && rect : rows) {
        auto graphs = rect->axis(QCPAxis::atBottom)->graphs();

        // удаляем данные с графиков
        for (auto && graph : graphs) {
            graph->data()->clear();
        }

        // для каждого диапазона
        for (auto && range : m_rangeData) {
            QVector<double> keys;
            QVector<double> values;
            //    auto rect = new QCPAxisRect(m_CustomPlot);
            auto graph = new QCPGraph(rect->axis(QCPAxis::atBottom), rect->axis(QCPAxis::atLeft));

            graph->setPen(QPen(Qt::green));
            graph->setBrush(QBrush(QColor(0, 255, 0, 20)));

            for (double i = range.ticks.first(); i <= range.ticks.back(); i+=0.1) {
                // добавляем значения по x и по y
                keys.push_back(i);
                values.push_back((sin((i - range.ticks.first() + range.leftFrequency) / 6.28) * 2) + 2);

                // реализация маркеров (ну почти)
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

            // добавляем точки на график
            graph->setData(keys, values);
        }
    }
    m_CustomPlot->replot();
}

void CustomPlotItem::addPost()
{
    // новый пост создаем
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

    // добавляем все диапазоны в пост
    for (auto && range : m_rangeData) {

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
        QElapsedTimer timer;
        timer.start();
        static QPicture picture;
        QCPPainter qcpPainter;
        qcpPainter.begin(&picture);
        m_CustomPlot->toPainter(&qcpPainter);
        qcpPainter.end();
        painter->drawPicture(0,0, picture);
        setReplotTime(QString::number(m_CustomPlot->replotTime()));
        setRepaintTime(QString::number(timer.elapsed()));
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
    // наводим красоту
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
//    QElapsedTimer timer;
//    timer.start();
//    grabPixmap = QPixmap(m_CustomPlot->size());
//    m_CustomPlot->render(&grabPixmap, QPoint(), m_CustomPlot->visibleRegion(), QWidget::IgnoreMask);
//    qDebug() << "Elapsed grab: " << timer.elapsed();

    update();
}

void CustomPlotItem::realtimeDataSlot()
{
//    static QTime time(QTime::currentTime());
//// calculate two new data points:
//    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
//    static double lastPointKey = 0;
    if (!m_absGraph)
        return;

    if (!m_absGraph->data())
        return;

    if ( m_absGraph->data()->size() == 0) {
        m_keys.resize(size1);
        m_values.resize(size1);
        std::generate(m_keys.begin(), m_keys.end(), [n = 0.0]() mutable { return n += 1; });
        m_absGraph->setData(m_keys, m_values, true);
        m_CustomPlot->graph(1)->setData(m_keys, m_values, true);
    }

    std::generate(m_values.begin(), m_values.end(), [key = 0.0] () mutable {
        key++;
        return qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843);
    });

    int i = 0;
    for (auto it = m_absGraph->data()->begin(); it != m_absGraph->data()->end(); ++it) {
        it->value = m_values.at(i);
        i++;
    }

    i = 0;
    for (auto it = m_CustomPlot->graph(1)->data()->begin(); it != m_CustomPlot->graph(1)->data()->end(); ++it) {
        it->value = m_values.at(i) + 2.0;
        i++;
    }


    emit needCustomReplot();

//    if (key-lastPointKey > 0.002) // at most add point every 2 ms
//    {
//        // add data to lines:
//
////        m_CustomPlot->graph(1)->setData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
//        // rescale value (vertical) axis to fit the current data:
//        //ui->customPlot->graph(0)->rescaleValueAxis();
//        //ui->customPlot->graph(1)->rescaleValueAxis(true);
//        lastPointKey = key;
//    }
//// make key axis range scroll with the data (at a constant range size of 8):
//    m_CustomPlot->xAxis->setRange(0, 1080);
//    m_CustomPlot->replot();
//
//// calculate frames per second:
//    static double lastFpsKey;
//    static int frameCount;
//    ++frameCount;
//    if (key-lastFpsKey > 2) // average fps over 2 seconds
//    {
//        qDebug() <<
//                QString("%1 FPS, Total Data points: %2")
//                        .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
//                        .arg(m_CustomPlot->graph(0)->data()->size()+m_CustomPlot->graph(1)->data()->size());
//        lastFpsKey = key;
//        frameCount = 0;
//    }
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

