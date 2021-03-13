#ifndef CALCULATION_H
#define CALCULATION_H

#include <QThread>

class CalculationThread : public QThread
{
    Q_OBJECT

public:
    CalculationThread(QObject * parent = 0);

protected:
    void run(); // 新线程入口

signals:
    void percentCopied(double);//信号可以传递变量给槽函数
    void finished();
    void errorOccurred();

public slots:
    void cancelCal();

private:
    bool canceled;

};

#endif
