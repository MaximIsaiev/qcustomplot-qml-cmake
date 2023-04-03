#include "rangeaxisticker.h"

//void RangeAxisTicker::generate(const QCPRange &range, const QLocale &locale, QChar formatChar, int precision, QVector<double> &ticks, QVector<double> *subTicks, QVector<QString> *tickLabels)
//{

//}

RangeAxisTicker::RangeAxisTicker()
{

}

double RangeAxisTicker::getTickStep(const QCPRange &range)
{
    return 1;
}

int RangeAxisTicker::getSubTickCount(double tickStep)
{
    return 0;
}

QString RangeAxisTicker::getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision)
{
    return QString();
}

QVector<double> RangeAxisTicker::createTickVector(double tickStep, const QCPRange &range)
{
    return mTicks;
}

QVector<QString> RangeAxisTicker::createLabelVector(const QVector<double> &ticks, const QLocale &locale, QChar formatChar, int precision)
{
    return mLabels;
}

void RangeAxisTicker::addTicks(const QCPRange &range)
{
    for (auto i = range.lower; i <= range.upper; i++) {
        mTicks.push_back(i);
        mLabels.push_back(QString::number(i));
    }
    generate(range, QLocale(), QChar(), 0, mTicks, nullptr, &mLabels);
}
