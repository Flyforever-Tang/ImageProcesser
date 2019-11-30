#ifndef STATISTICALCHART_H
#define STATISTICALCHART_H

#include "grayImageProcesser.h"
#include "grayImageStatiscian.h"
#include "labelControler.h"
#include <QMainWindow>
#include <QPainter>
#include <QSlider>
#include <QLabel>

namespace Ui {
class StatisticalChart;
}

class StatisticalChart : public QMainWindow
{
    Q_OBJECT

public:
    int ChartLeft, ChartFloor, ChartHeight, LineWidth, LineSum, BiggestData;
    int* Data;

    QSlider* ThresholdSlider;
    QLabel* ValueLabel;

    QLabel* OriImageLabel;
    QLabel* NewImageLabel;

    QLabel* MaxSum;
    QLabel* Average;
    QLabel* MidValue;
    QLabel* StandardDev;
    QLabel* PixelSum;

    QImage* ThisImage;
    GrayImageStatiscian* Statiscian;

public:
    explicit StatisticalChart(int CL, int CF, int CH, int LW, int LS, QImage* QI, QWidget *parent = nullptr);
    ~StatisticalChart();

    void InitializeUI();    //初始化UI
    void ExportPainter();   //导出直方图，暂未实现
    void ShowKeyData();

protected:
//    QPixmap* PainterMap;
    QPainter* Painter;

    void paintEvent(QPaintEvent *);

private slots:
    void on_ThresholdSlider_sliderMoved(int position);

private:
    Ui::StatisticalChart *ui;
};

#endif // STATISTICALCHART_H
