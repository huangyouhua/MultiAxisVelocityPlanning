#include "calvelthread.h"
#include "common.h"
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QDebug>

CalVelThread::CalVelThread(QObject *parent)
    : QObject(parent)
{
    canceled = false;
}

void CalVelThread::startCalVel()
{
    qDebug() << "START";
    canceled = false;
    QFile src(SrcFile);
    QFile cpy(CpyFile);
    qint64 srcSize = QFileInfo(src).size();
    int bytesWritten = 0;
    if (srcSize==0 || !src.open(QFile::ReadOnly))
    {
        emit errorOccurred();
        return;
    }
    if (cpy.exists())
    {
        cpy.resize(0);
    }
    if (!cpy.open(QFile::WriteOnly))
    {
        src.close();
        emit errorOccurred();
        return;
    }
    while (!src.atEnd())
    {
        if (canceled)
        {
            src.close();
            cpy.close();
            cpy.remove();
            return;
        }
        QThread::msleep(8);
        qint64 wrt = cpy.write(src.readLine());
        if (wrt < 0)
        {
            emit errorOccurred();
            src.close();
            cpy.close();
            cpy.remove();
            return;
        }
        bytesWritten += wrt;
        emit percentCopied(bytesWritten*1.0/srcSize);
    }
    src.close();
    cpy.close();
    emit finished();
}

void CalVelThread::cancelCalVel()
{
    canceled = true;
}

