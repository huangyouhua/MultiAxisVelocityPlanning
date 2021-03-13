#ifndef CALVELTHREAD_H
#define CALVELTHREAD_H

#include <QObject>

class CalVelThread : public QObject
{
    Q_OBJECT

public:
    explicit CalVelThread(QObject *parent = 0);

signals:
    void percentCopied(double percent);
    void finished();
    void errorOccurred();

public slots:
    void startCalVel();
    void cancelCalVel();

private:
    bool canceled;

};

#endif // CALVELTHREAD_H
