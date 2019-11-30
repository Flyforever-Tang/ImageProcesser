#include "statisticalChart.h"
#include "ui_statisticalChart.h"

StatisticalChart::StatisticalChart(int CL, int CF, int CH, int LW, int LS, QImage* QI, QWidget *parent) :
    QMainWindow(parent),
    ChartLeft(CL),
    ChartFloor(CF),
    ChartHeight(CH),
    LineWidth(LW),
    LineSum(LS),
    ThisImage(QI),
    ui(new Ui::StatisticalChart)
{
    ui->setupUi(this);

    Statiscian = new GrayImageStatiscian(ThisImage);
    Data = Statiscian ->GrayValueStatisticResult;
    BiggestData = Statiscian ->MaxSum;

    InitializeUI();
}

StatisticalChart::~StatisticalChart()
{
    delete ui;
}

void StatisticalChart::InitializeUI()
{
    ThresholdSlider = ui ->ThresholdSlider;
    ThresholdSlider ->setOrientation(Qt::Horizontal);
    ThresholdSlider ->setGeometry(ChartLeft - 5, ChartFloor + 5, LineSum * LineWidth + 10, 20);
    ThresholdSlider ->setTickPosition(QSlider::TicksAbove);

    ThresholdSlider ->setMinimum(-1);
    ThresholdSlider ->setMaximum(257);
    ThresholdSlider ->setValue(128);

    ValueLabel = new QLabel(this);
    ValueLabel ->setGeometry(ChartLeft + ThresholdSlider ->width(), ChartFloor + 5, 30, 20);
    ValueLabel ->setStyleSheet("color:green;");
    ValueLabel ->setText(QString::number(ThresholdSlider ->value()));

    OriImageLabel = new QLabel(this);
    OriImageLabel ->setGeometry(ChartLeft + ThresholdSlider ->width() + 50, 10, 256, 256);
    NewImageLabel = new QLabel(this);
    NewImageLabel ->setGeometry(ChartLeft + ThresholdSlider ->width() + 50, 20 + 256, 256, 256);

    QImage* StanderBinaryImage = GrayImageProcesser::CreateBinaryImage(ThisImage, 128);
    LabelControler::SetImage(OriImageLabel, ThisImage);
    LabelControler::SetImage(NewImageLabel, StanderBinaryImage);

    MaxSum = new QLabel(this);
    MaxSum ->setStyleSheet("color:red;");
    Average = new QLabel(this);
    Average ->setGeometry(ChartLeft,ChartFloor + 30, 130, 20);
    Average ->setStyleSheet("color:red;");
    Average ->setText("平均灰度：" + QString::number(Statiscian ->Average, 'f', 2));
    MidValue = new QLabel(this);
    MidValue ->setGeometry(ChartLeft + 150,ChartFloor + 30, 120, 20);
    MidValue ->setStyleSheet("color:red;");
    MidValue ->setText("中值灰度：" + QString::number(Statiscian ->MidValue, 'f', 1));
    StandardDev = new QLabel(this);
    StandardDev ->setGeometry(ChartLeft + 300,ChartFloor + 30, 130, 20);
    StandardDev ->setStyleSheet("color:red;");
    StandardDev ->setText("标准差：" + QString::number(Statiscian ->StandardDev, 'f', 2));
    PixelSum = new QLabel(this);
    PixelSum ->setGeometry(ChartLeft + 450, ChartFloor + 30, 150, 20);
    PixelSum ->setStyleSheet("color:red;");
    PixelSum ->setText("像素总数：" + QString::number(Statiscian ->PixelSum));
}

void StatisticalChart::ExportPainter()
{
//    QString SavePath = QFileDialog::getSaveFileName(this, tr("选择文件"), "E:\\", tr("Image (*.png)"));
//    PainterMap ->save("SavePath", "PNG");
}

void StatisticalChart::paintEvent(QPaintEvent *)
{
    int MaxSumGrayValue[256] = {}, EqualMaxNum = 0;

//    PainterMap = new QPixmap(512, 512);
//    PainterMap ->fill(Qt::transparent);
//    QPainter* MapPainter = new QPainter(PainterMap);
//    MapPainter ->setPen(QPen(Qt::blue, LineWidth));
//    MapPainter ->begin(PainterMap);

    Painter = new QPainter(this);
    Painter ->setPen(QPen(Qt::blue, LineWidth));

    for(int i = 0; i < LineSum; ++i)
    {
        int LineHeight, y_Start, y_End, x;
        LineHeight = ChartHeight * Data[i] / BiggestData;
        y_Start = ChartFloor;
        y_End = y_Start - LineHeight;
        x = ChartLeft + i * LineWidth;

        Painter ->drawLine(x, y_Start, x, y_End);
//        MapPainter ->drawLine(x, y_Start, x, y_End);

        if(Data[i] == Statiscian ->MaxSum)
        {
            MaxSumGrayValue[EqualMaxNum++] = i;
        }
    }

    Painter ->setPen(QPen(Qt::red, LineWidth / 2));
    Painter ->drawLine(MaxSumGrayValue[0] * 2 - 6 + ChartLeft, ChartFloor - ChartHeight - 1, MaxSumGrayValue[EqualMaxNum - 1] * 2 + 5 + ChartLeft, ChartFloor - ChartHeight - 1);
//    MapPainter ->setPen(QPen(Qt::red, LineWidth / 2));
//    MapPainter ->drawLine(MaxSumGrayValue[0] * 2 - 5 + ChartLeft, ChartFloor - ChartHeight, MaxSumGrayValue[EqualMaxNum - 1] * 2 + 5 + ChartLeft, ChartFloor - ChartHeight);
    MaxSum ->setGeometry(MaxSumGrayValue[0] + MaxSumGrayValue[EqualMaxNum - 1], ChartFloor - ChartHeight - 25, 100, 20);
    MaxSum ->setText("峰值：" + QString::number(Statiscian ->MaxSum));

//    MapPainter ->end();
    Painter ->end();

//    for(int i = 0; i < LineSum; ++i)
//    {
//        int LineHeight, y_Start, y_End, x;
//        LineHeight = ChartHeight * Data[i] / BiggestData;
//        y_Start = ChartFloor;
//        y_End = y_Start - LineHeight;
//        x = ChartLeft + i * LineWidth;

//        Painter ->drawLine(x, y_Start, x, y_End);
//    }

//    PainterMap ->save("E:\map.jpg", "JPG", 100);
//    Painter ->drawPixmap(0, 0, *PainterMap);

}

void StatisticalChart::ShowKeyData()
{

}

void StatisticalChart::on_ThresholdSlider_sliderMoved(int position)
{
    ValueLabel ->setText(QString::number(ThresholdSlider ->value()));
    QImage* NewBinaryImage = GrayImageProcesser::CreateBinaryImage(ThisImage, ThresholdSlider ->value());

    LabelControler::SetImage(NewImageLabel, NewBinaryImage);
}
