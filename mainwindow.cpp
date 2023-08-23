#include "mainwindow.h"
#include "ui_mainwindow.h"

pid_info_t pid;

#define MAX_Y_VALUE   500

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PidInit();

    InitView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitView()
{
    QVector<double> x(MAX_Y_VALUE), y(MAX_Y_VALUE); //初始化向量x和y
    for (int i=0; i<MAX_Y_VALUE; i++)
    {
      x[i] = i; // x范围[-1,1]
      y[i] = PidRealize(100.0); // y=x*x
      ui->plainTextEdit->appendPlainText(QString::number(y[i]));
    }
    ui->customPlot->addGraph();//添加数据曲线（一个图像可以有多个数据曲线）
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // 曲线的颜色

    // graph(0);可以获取某个数据曲线（按添加先后排序）
    // setData();为数据曲线关联数据
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setName("plot");// 设置图例名称
    // 为坐标轴添加标签
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // 设置坐标轴的范围，以看到所有数据
    ui->customPlot->xAxis->setRange(0, MAX_Y_VALUE);
    ui->customPlot->yAxis->setRange(0, 100);
    ui->customPlot->legend->setVisible(true); // 显示图例
    // 重画图像
    ui->customPlot->replot();
}

void MainWindow::PidInit()
{
    pid.SetTemp = 0.0;
    pid.ActualTemp = 0.0;
    pid.err = 0.0;
    pid.err_last = 0.0;
    pid.err_next = 0.0;
    pid.Kp = 0.2;
    pid.Ki = 0.04;
    pid.Kd = 0.2;
}

double MainWindow::PidCalc(double target, double temp)
{
    pid.SetTemp = target;
    pid.ActualTemp = temp;
    pid.err = pid.SetTemp - pid.ActualTemp;
    double incrementSpeed = pid.Kp*(pid.err - pid.err_next) + \
                            pid.Ki*pid.err + pid.Kd*(pid.err - \
                            2 * pid.err_next + pid.err_last);

    pid.err_last = pid.err_next;
    pid.err_next = pid.err;
    return incrementSpeed;
}

double MainWindow::PidRealize(double temp)
{
    pid.SetTemp = temp;
    pid.err = pid.SetTemp - pid.ActualTemp;
    double incrementSpeed = pid.Kp*(pid.err - pid.err_next) + \
                            pid.Ki*pid.err + pid.Kd*(pid.err - \
                            2 * pid.err_next + pid.err_last);
    pid.ActualTemp += incrementSpeed;
    pid.err_last = pid.err_next;
    pid.err_next = pid.err;
    return pid.ActualTemp;
}

void MainWindow::on_updateBtn_clicked()
{
    bool *ok = nullptr;

    ui->customPlot->clearGraphs();
    ui->customPlot->replot();

    PidInit();

    ui->plainTextEdit->clear();

    pid.Kp = ui->kpSpinBox->text().toDouble(ok);
    pid.Ki = ui->kiSpinBox->text().toDouble(ok);
    pid.Kd = ui->kdSpinBox->text().toDouble(ok);

    QVector<double> x(MAX_Y_VALUE), y(MAX_Y_VALUE); //初始化向量x和y
    for (int i=0; i<MAX_Y_VALUE; i++)
    {
      x[i] = i; // x范围[-1,1]
      y[i] = PidRealize(ui->targetSpinBox->text().toDouble(ok)); // y=x*x

       ui->plainTextEdit->appendPlainText(QString::number(y[i]));

    }
    ui->customPlot->addGraph();//添加数据曲线（一个图像可以有多个数据曲线）
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue)); // 曲线的颜色

    // graph(0);可以获取某个数据曲线（按添加先后排序）
    // setData();为数据曲线关联数据
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->setName("plot");// 设置图例名称
    // 为坐标轴添加标签
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // 设置坐标轴的范围，以看到所有数据
    ui->customPlot->xAxis->setRange(0, MAX_Y_VALUE);
    ui->customPlot->yAxis->setRange(0, 100);

//    ui->customPlot->xAxis->setTickLength(5, 0);//设置x轴刻度线的长度
//    ui->customPlot->xAxis->setSubTickLength(2, 0);//设置x轴子刻度线长度

    ui->customPlot->legend->setVisible(true); // 显示图例
    // 重画图像
    ui->customPlot->replot();

}

void MainWindow::on_clearBtn_clicked()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
}
