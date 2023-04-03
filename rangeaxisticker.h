#ifndef RANGEAXISTICKER_H
#define RANGEAXISTICKER_H

#include <qcustomplot.h>


class RangeAxisTicker : public QCPAxisTicker
{
public:
    RangeAxisTicker(QVector<double> const &ticks, QVector<QString> const &labels)
        : QCPAxisTicker(), mTicks(ticks), mLabels(labels) {}

    RangeAxisTicker();

//    virtual void generate(const QCPRange &range, const QLocale &locale, QChar formatChar, int precision, QVector<double> &ticks, QVector<double> *subTicks, QVector<QString> *tickLabels) override;

    virtual double getTickStep(const QCPRange &range) override;

    virtual int getSubTickCount(double tickStep) override;

    virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) override;

    virtual QVector<double> createTickVector(double tickStep, const QCPRange &range) override;

    virtual QVector<QString> createLabelVector(const QVector<double> &ticks, const QLocale &locale, QChar formatChar, int precision) override;

    void addTicks(const QCPRange &range);

private:
    QVector<double> mTicks;
    QVector<QString> mLabels;
};

#endif // RANGEAXISTICKER_H
