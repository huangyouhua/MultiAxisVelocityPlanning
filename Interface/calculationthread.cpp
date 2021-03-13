#include "calculationthread.h"
#include "common.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>

CalculationThread::CalculationThread(QObject *parent)
    : QThread(parent) //父类部分的构造函数
{
    //新线程的初始化不能放在这里
}

//重写run函数
void CalculationThread::run()
{
    canceled = false;
     QFile src(SrcFile);
     QFile cpy(CpyFile);
     qint64 srcSize = QFileInfo(src).size();
     int bytesWritten = 0;
     if (srcSize==0 || !src.open(QFile::ReadOnly))
     {
         emit errorOccurred();//文件读取出错，发送槽函数
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
         msleep(8);
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
     emit finished();//发送信号
}

void CalculationThread::cancelCal()//通过该函数给子线程发送取消指令
{
    canceled = true;//
}
