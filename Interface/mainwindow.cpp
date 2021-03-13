#include "mainwindow.h"
#include "ui_mainwindow.h"
#include    <QDir>
#include    <QFileDialog>
#include    <QTextStream>
#include    <QTextDocument>
#include    <QTextBlock>
#include    <string>
#include    <QMessageBox>
#include <string>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //第一种多线程的方法————重写run()函数
    CalVel = new CalculationThread(this); //指向计算速度的子线程
    connect(CalVel, SIGNAL(percentCopied(double)), this, SLOT(updateCopyProgress(double)));//绑定计算速度的子线程的信号和主线程的槽函数
    connect(CalVel, SIGNAL(errorOccurred()), this, SLOT(errorHandleSlot()));
    connect(CalVel, SIGNAL(finished()), this, SLOT(copyFinishSlot()));

    //第二种多线程的方法————moveToThread()
    CalVel2 = new CalVelThread;// 这个实例要负责复制任务，不要设置parent
    CalVelMoveToThread = new QThread;
    connectCopier(CalVel2); // 连接信号-槽，复制的开始和取消指令是通过信号发送的
    CalVel2->moveToThread(CalVelMoveToThread); // 将实例移动到新的线程，实现多线程运行
    CalVelMoveToThread->start(); // 启动子线程

    data.angle_file_name = NULL;
    data.MaxVelocity_file_name = NULL;
    data.MaxAcceleration_file_name = NULL;
    data.save_inter_joint_angle_file_name = NULL;
    data.Interpolation_point = Default_Interpolation_points;
    data.LgLookahead = Default_lgLookahead;
    data.Sample_Frequency = Default_Frequency;
    data.Dof = Default_dof;
    data.delta_Vmax = Default_delta_vmax;
    data.Max_velocity = Default_max_velocity;
    data.Max_acceleration = Default_max_acceleration;

    ui->Inter_ponits->setValue(Default_Interpolation_points);
    ui->Lookahead->setValue(Default_lgLookahead);
    ui->Frequency->setValue(Default_Frequency);
    ui->Num_axis->setValue(Default_dof);
    ui->delta_vmax->setValue(Default_delta_vmax);
    ui->Max_limited_velocity->setValue(Default_max_velocity);
    ui->Max_limited_acc->setValue(Default_max_acceleration);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Inter_ponits_valueChanged(int arg1)
{
    data.Interpolation_point = arg1;
}

void MainWindow::on_Lookahead_valueChanged(int arg1)
{
    data.LgLookahead = arg1;
}

void MainWindow::on_Frequency_valueChanged(int arg1)
{
    data.Sample_Frequency = arg1;
}

void MainWindow::on_Num_axis_valueChanged(int arg1)
{
    data.Dof = arg1;
}

void MainWindow::on_delta_vmax_valueChanged(double arg1)
{
    data.delta_Vmax = arg1;
}

void MainWindow::on_Max_limited_velocity_valueChanged(double arg1)
{
    data.Max_velocity = arg1;
}

void MainWindow::on_Max_limited_acc_valueChanged(double arg1)
{
    data.Max_acceleration = arg1;
}

char* MainWindow::GetCuurentFilePath(void)
{
    QString aFileName;
    QString filter;
    QString dlgTitle;
    QString curPath;
    size_t FilePathLen;
    while(1)
    {
        curPath=QDir::currentPath();// get current system path
        dlgTitle="打开一个文件";
//        filter="程序文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
        filter="所有文件(*.*)"; //文件过滤器
        aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
        if(!aFileName.isEmpty())
        {
            FilePathLen = aFileName.size();
            char* FilePath = (char*)malloc(sizeof(char) * (FilePathLen+1));//allocate memory
            QByteArray ba = aFileName.toLatin1();
            strcpy(FilePath, ba.data());
            ui->plainTextEdit->appendPlainText("Load file successfully ..." + aFileName);
            return FilePath;
        }
        else
        {
            QString dlgTitle="Warning";
            QString strInfo="You haven't chose a file, do you want to exit？";

            QMessageBox::StandardButton  defaultBtn=QMessageBox::NoButton; //缺省按钮

            QMessageBox::StandardButton result;//返回选择的按钮
            result=QMessageBox::question(this, dlgTitle, strInfo,
                              QMessageBox::Yes|QMessageBox::No,
                              defaultBtn);

            if(result==QMessageBox::No)
            {
                continue;
            }
            else
            {
                ui->plainTextEdit->appendPlainText("You haven't chose a file...");
                return NULL;
            }
        }
    }
}

//get files path
void MainWindow::on_check_use_diff_vel_clicked(bool checked)
{
    if(checked)
    {
         data.MaxVelocity_file_name = GetCuurentFilePath();

         if(!data.MaxVelocity_file_name)
            ui->check_use_diff_vel->setChecked(false);
    }
    else
    {
        free(data.MaxVelocity_file_name);
        data.MaxVelocity_file_name = NULL;
        ui->plainTextEdit->appendPlainText("Use same Max limited joint velocity...");
    }
}

void MainWindow::on_check_use_diff_acc_clicked(bool checked)
{
    if(checked)
    {
         data.MaxAcceleration_file_name = GetCuurentFilePath();

         if(!data.MaxAcceleration_file_name)
            ui->check_use_diff_acc->setChecked(false);
    }
    else
    {
        free(data.MaxAcceleration_file_name);
        data.MaxAcceleration_file_name = NULL;
        ui->plainTextEdit->appendPlainText("Use same Max limited joint velocity...");
    }
}

void MainWindow::on_check_save_pt_clicked(bool checked)
{
    if(checked)
    {
         data.save_inter_joint_angle_file_name = GetCuurentFilePath();

         if(!data.save_inter_joint_angle_file_name)
            ui->check_save_pt->setChecked(false);
    }
    else
    {
        free(data.save_inter_joint_angle_file_name);
        data.save_inter_joint_angle_file_name = NULL;
        ui->plainTextEdit->appendPlainText("don't save PT...");
    }
}


void MainWindow::on_Load_angle_btn_pressed()
{
    char* p = GetCuurentFilePath();
    if(p)
    {
        free(data.angle_file_name);
        data.angle_file_name = p;
    }
    if(data.angle_file_name)
        ui->Load_angle_btn->setStyleSheet("color:gray");
}

void MainWindow::on_clear_dis_pressed()
{
    ui->plainTextEdit->setPlainText("");
}

void MainWindow::on_Calculate_pt_pressed()
{
    if(data.angle_file_name == NULL)
    {
        QString dlgTitle="Warning";
        QString strInfo="You should chose an joint angle file!";

        QMessageBox::StandardButton  defaultBtn=QMessageBox::NoButton; //缺省按钮

        QMessageBox::StandardButton result;//返回选择的按钮
        result=QMessageBox::question(this, dlgTitle, strInfo,
                          QMessageBox::Yes|QMessageBox::Cancel,
                          defaultBtn);
        if(result==QMessageBox::Yes)
        {
            data.angle_file_name = GetCuurentFilePath();
            if(data.angle_file_name)
                ui->Load_angle_btn->setStyleSheet("color:gray");
        }
        else
        {
        }
    }
    ui->plainTextEdit->appendPlainText("Calculating PT...");
    Inter_Joint_angle = velocity_plan(data);//主要计算代码入口
    ui->plainTextEdit->appendPlainText("Finish Calculating PT ...");
}

void MainWindow::errorHandleSlot()
{
    ui->Start_pushButton->setEnabled(true);
    ui->Cancel_pushButton->setEnabled(false);
    ui->plainTextEdit->appendPlainText("Calculate PT error...");

}

void MainWindow::updateCopyProgress(double percent)
{
    ui->progressBar->setValue(percent*100);//进度条显示
}

void MainWindow::copyFinishSlot()
{
    ui->progressBar->setValue(100);
    ui->Start_pushButton->setEnabled(true);
    ui->Cancel_pushButton->setEnabled(false);
    ui->plainTextEdit->appendPlainText("Finish Calculate PT...");
}

void MainWindow::on_Start_pushButton_clicked()
{
    ui->Start_pushButton->setEnabled(false);//不可点击
    ui->Cancel_pushButton->setEnabled(true);//可点击
    // 注意：这里用start()不用run()
    if (ui->grpOperation1->isChecked())
    {
        CalVel->start(); // 复制文件的新线程开始（自动调用run()）
    }
    else if(ui->grpOperation2->isChecked())
        emit startCopyRsquested();

    ui->plainTextEdit->appendPlainText("Start Calculate PT...");
}

void MainWindow::on_Cancel_pushButton_clicked()
{
    ui->Start_pushButton->setEnabled(true);//不可点击
    ui->Cancel_pushButton->setEnabled(false);//可点击
    if (ui->grpOperation1->isChecked())
    {
        CalVel->cancelCal(); // 复制文件的新线程结束
    }
    else if(ui->grpOperation2->isChecked())
        emit cancelCopuRequested();

    ui->plainTextEdit->appendPlainText("Stop Calculate PT...");
}

void MainWindow::connectCopier(CalVelThread *calvel)//方法二信号和槽绑定
{
    connect(this, SIGNAL(startCopyRsquested()), // 使用信号-槽机制，发出开始指令
            calvel, SLOT(startCalVel()));
    connect(this, SIGNAL(cancelCopuRequested()), // 使用信号-槽机制，发出取消指令
            calvel, SLOT(cancelCalVel()));
    connect(calvel, SIGNAL(errorOccurred()),
            this, SLOT(errorHandleSlot()));
    connect(calvel, SIGNAL(percentCopied(double)),
            this, SLOT(updateCopyProgress(double)));
    connect(calvel, SIGNAL(finished()),
            this, SLOT(copyFinishSlot()));
}

void MainWindow::on_grpOperation1_clicked(bool checked)
{
    ui->grpOperation2->setChecked(!checked);
    CalVel2->disconnect();
}

void MainWindow::on_grpOperation2_clicked(bool checked)
{
    ui->grpOperation1->setChecked(!checked);
    connectCopier(CalVel2); // 连接信号-槽，复制的开始和取消指令是通过信号发送的
    CalVel->disconnect();
}
