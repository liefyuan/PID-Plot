#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <QDebug>


typedef struct {
    double SetTemp;   //定义设定值
    double ActualTemp;//定义实际值
    double err;        //定义偏差值
    double err_next;   //定义上一个偏差值
    double err_last;   //定义最上前的偏差值
    double Kp, Ki, Kd; //定义比例、积分、微分系数
} pid_info_t;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_updateBtn_clicked();

    void on_clearBtn_clicked();

private:
    void InitView();
    void PidInit();
    double PidCalc(double target, double temp);
    double PidRealize(double temp);

private:
    Ui::MainWindow *ui;
    QCustomPlot    *gCustomPlot;
};
#endif // MAINWINDOW_H
