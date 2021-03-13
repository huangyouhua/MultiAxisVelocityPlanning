#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "calculationthread.h"
#include "calvelthread.h"

extern "C"
{
    #include "velocity_plan.h"
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend void SetDefaultValue();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    char* GetCuurentFilePath(void);

    Interface_data data;
    Matrix_t Inter_Joint_angle;


signals:
    void startCopyRsquested();

    void cancelCopuRequested();

private slots:
    void on_Inter_ponits_valueChanged(int arg1);

    void on_Lookahead_valueChanged(int arg1);

    void on_Frequency_valueChanged(int arg1);

    void on_Num_axis_valueChanged(int arg1);

    void on_delta_vmax_valueChanged(double arg1);

    void on_check_use_diff_vel_clicked(bool checked);

    void on_check_use_diff_acc_clicked(bool checked);

    void on_clear_dis_pressed();

    void on_check_save_pt_clicked(bool checked);

    void on_Load_angle_btn_pressed();

    void on_Max_limited_velocity_valueChanged(double arg1);

    void on_Max_limited_acc_valueChanged(double arg1);

    void on_Calculate_pt_pressed();

    void updateCopyProgress(double percent); //显示进度条的槽函数

    void errorHandleSlot();

    void copyFinishSlot();

    void on_Start_pushButton_clicked();

    void on_Cancel_pushButton_clicked();


    void on_grpOperation1_clicked(bool checked);

    void on_grpOperation2_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    CalculationThread *CalVel; // 用于计算速度的子线程
    CalVelThread *CalVel2;
    QThread * CalVelMoveToThread; // CalVel2将被移动到此线程执行

    void connectCopier(CalVelThread *calvel);//方法二信号和槽绑定
};

#endif // MAINWINDOW_H
