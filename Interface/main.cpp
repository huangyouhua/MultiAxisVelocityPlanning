#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>

extern "C"
{
    #include "myMatrix.h"
    #include "velocity_plan.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
    #include "PCS.h"
    #include "LookAhead.h"
    #include "myMatrix.h"
    #include "Interpolation.h"
}
char joint_filename[200] = "/media/hyh/Study/Desktop/TempFile/Interface/build-Interface-Desktop_Qt_5_9_0_GCC_64bit-Debug/refjoint.txt";
char Save_inter_joint_angle_file_name[200] = "/media/hyh/Study/Desktop/TempFile/Interface/build-Interface-Desktop_Qt_5_9_0_GCC_64bit-Debug/joint_angle_interpolation.txt";
char MaxVelocity_file[100] = "MaxVelocity.txt";
char MaxAcceleration_file[100] = "MaxAcceleration.txt";
void GetData(Interface_data* data)
{
    unsigned char MaxVelocity_flag = 0;
    unsigned char MaxAccelertion_flag = 0;
    unsigned char Save_inter_joint_angle_flag = 1;

    data->Dof = 6;
    data->Interpolation_point = 5;
    data->LgLookahead = 50;
    data->Sample_Frequency = 100;
    data->delta_Vmax = 0.001*PI;
    data->angle_file_name = joint_filename;
    data->save_inter_joint_angle_file_name = Save_inter_joint_angle_file_name;

    if(!MaxVelocity_flag)
    {
        data->MaxVelocity_file_name = NULL;
        data->Max_velocity = Default_max_velocity;
    }
    else
        data->MaxVelocity_file_name = MaxVelocity_file;
    if(!MaxAccelertion_flag)
    {
        data->MaxAcceleration_file_name = NULL;
        data->Max_acceleration = Default_max_acceleration;
    }
    else
        data->MaxAcceleration_file_name = MaxAcceleration_file;
    if(!Save_inter_joint_angle_flag)
        data->save_inter_joint_angle_file_name = NULL;
    else
        data->save_inter_joint_angle_file_name = Save_inter_joint_angle_file_name;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Interface_data data;
    //GetData(&data);
    //Inter_Joint_angle = velocity_plan(w.data);
    //velocity_plan(data);

    return a.exec();
}
