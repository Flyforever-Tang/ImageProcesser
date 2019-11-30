#include "imageTransshaper.h"
#include "ui_imageTransshaper.h"

ImageTransshaper::ImageTransshaper(MainWindow* Window, QWidget *parent) :
    QMainWindow(parent),
    ThisMainWindow(Window),
    OriImage(Window ->OriImage),
    NewImage(GrayImageProcesser::CompressImage_Quan(OriImage, 256)),
    ui(new Ui::ImageTransshaper)
{
    ui->setupUi(this);

    InitializeUI();
}

ImageTransshaper::~ImageTransshaper()
{
    delete ui;
}

void ImageTransshaper::InitializeUI()
{
    OriImageLabel = new QLabel(this);
    NewImageLabel = new ImageLabel<ImageTransshaper>(this, &ImageTransshaper::MoveImage, this);
    ScaleSlider = new QSlider(this);
    RotateSlider = new QSlider(this);
    ScaleValueLabel = new QLabel(this);
    RotateValueLabel = new QLabel(this);
    MethodLabel = new QLabel(this);
    MethodBox = new QComboBox(this);
    ResetImageButton = new QPushButton("重置", this);
    SaveImageButton = new QPushButton("保存", this);
    SaveImageAsButton = new QPushButton("另存为", this);

    OriImageLabel ->setGeometry(580, 20, 256, 256);
    LabelControler::SetImage(OriImageLabel, OriImage);
    NewImageLabel ->setGeometry(20, 20, 512, 512);
    NewImageLabel ->setStyleSheet("border:2px solid #C4C4C4;");
    LabelControler::SetImage(NewImageLabel, NewImage);

    ScaleSlider ->setOrientation(Qt::Horizontal);
    ScaleSlider ->setGeometry(16, 540, 520, 20);
    ScaleSlider ->setTickPosition(QSlider::TicksAbove);
    ScaleSlider ->setMinimum(0);
    ScaleSlider ->setMaximum(200);
    ScaleSlider ->setValue(100);
    ScaleSlider ->setTickInterval(10);
    QObject::connect(ScaleSlider, SIGNAL(valueChanged(int)), this, SLOT(ScaleSliderMove()));
    RotateSlider ->setOrientation(Qt::Vertical);
    RotateSlider ->setGeometry(540, 16, 20, 520);
    RotateSlider ->setTickPosition(QSlider::TicksAbove);
    RotateSlider ->setMinimum(-180);
    RotateSlider ->setMaximum(180);
    RotateSlider ->setValue(0);
    RotateSlider ->setTickInterval(15);
    QObject::connect(RotateSlider, SIGNAL(valueChanged(int)), this, SLOT(RotateSliderMove()));

    ScaleValueLabel ->setGeometry(520, 555, 40, 20);
    ScaleValueLabel ->setStyleSheet("color:green;");
    ScaleValueLabel ->setText(QString::number(ScaleSlider ->value()) + "%");
    RotateValueLabel ->setGeometry(565, 520, 40, 20);
    RotateValueLabel ->setStyleSheet("color:green;");
    RotateValueLabel ->setText(QString::number(RotateSlider ->value()) + "°");

    MethodLabel ->setGeometry(570, 300, 120, 30);
    MethodLabel ->setFont(QFont("Microsoft YaHei", 11));
    MethodLabel ->setStyleSheet("color:red;");
    MethodLabel ->setText("灰度插值方法:");
    MethodBox ->setGeometry(695, 300, 120, 30);
    MethodBox ->setFont(QFont("Microsoft YaHei", 11));
    MethodBox ->setStyleSheet("color:blue;");
    MethodBox ->addItem("最邻近插值");
    MethodBox ->addItem("双线性插值");
    MethodBox ->setCurrentIndex(0);
    InterpolationFunc = Interpolation_NearestNeighbor<unsigned char>;
    QObject::connect(MethodBox, SIGNAL(currentIndexChanged(int)), this ,SLOT(MethodBoxIndexChange()));

    ResetImageButton ->setGeometry(630, 370, 150, 50);
    ResetImageButton ->setFont(QFont("Microsoft YaHei", 15));
    QObject::connect(ResetImageButton, SIGNAL(clicked()), this, SLOT(ResetImage()));
    SaveImageButton ->setGeometry(630, 430, 150, 50);
    SaveImageButton ->setFont(QFont("Microsoft YaHei", 15));
    QObject::connect(SaveImageButton, SIGNAL(clicked()), this, SLOT(SaveImage()));
    SaveImageAsButton ->setGeometry(630, 490, 150, 50);
    SaveImageAsButton ->setFont(QFont("Microsoft YaHei", 15));
    QObject::connect(SaveImageAsButton, SIGNAL(clicked()), this, SLOT(SaveImageAs()));
}

void ImageTransshaper::MoveImage(int MoveDis_x, int MoveDis_y)
{
    const int ResponseRate = 3;
    NewImage = GrayImageProcesser::MoveImage(NewImage, MoveDis_x * ResponseRate, MoveDis_y * ResponseRate);
    LabelControler::SetImage(NewImageLabel, NewImage, ScaleSlider ->value());
}

void ImageTransshaper::ScaleSliderMove()
{
    static int LastScaleValue = 100;

    ScaleValueLabel ->setText(QString::number(ScaleSlider ->value()) + "%");
    NewImage = GrayImageProcesser::ScaleImage(NewImage, ScaleSlider ->value() * 100.0 / LastScaleValue, InterpolationFunc);
    LabelControler::SetImage(NewImageLabel, NewImage, ScaleSlider ->value());

    LastScaleValue = ScaleSlider ->value();
}

void ImageTransshaper::RotateSliderMove()
{
    static int LastRotateValue = 0;

    RotateValueLabel ->setText(QString::number(RotateSlider ->value()) + "°");
    NewImage = GrayImageProcesser::RotateImage(NewImage, RotateSlider ->value() - LastRotateValue,
                                               NewImage ->width() / 2, NewImage ->height() / 2, InterpolationFunc);
    LabelControler::SetImage(NewImageLabel, NewImage, ScaleSlider ->value());

    LastRotateValue = RotateSlider ->value();
}

void ImageTransshaper::ResetImage()
{
    ScaleSlider ->setValue(100);
    RotateSlider ->setValue(0);

    NewImage = GrayImageProcesser::CompressImage_Res(OriImage, 100);
    LabelControler::SetImage(NewImageLabel, NewImage);
}

void ImageTransshaper::SaveImage()
{
    ThisMainWindow ->ChangeOriImage(NewImage);
    ImageControler::ChangeImage(OriImage, ThisMainWindow ->OriImage);
    LabelControler::SetImage(OriImageLabel, OriImage);
    ResetImage();
}

void ImageTransshaper::SaveImageAs()
{
    QString SavePath = QFileDialog::getSaveFileName(this, tr("选择文件"), "D:/", tr("Image (*.jpg)"));
    NewImage ->save(SavePath, "JPG", 100);
}

void ImageTransshaper::closeEvent(QCloseEvent* Event)
{
    ImageControler::ChangeImage(ThisMainWindow ->NewImage, NewImage);
    LabelControler::SetImage(ThisMainWindow ->NewImageLabel, NewImage);
    delete this;
}

void ImageTransshaper::MethodBoxIndexChange()
{
    InterpolationFunc = (MethodBox ->currentIndex() == 0) ? Interpolation_NearestNeighbor<unsigned char> :
                                                            Interpolation_Bilinear<unsigned char>;

    delete NewImage;

    NewImage = GrayImageProcesser::ScaleImage(GrayImageProcesser::CompressImage_Quan(OriImage, 256),
                                              ScaleSlider ->value(), InterpolationFunc);
    NewImage = GrayImageProcesser::RotateImage(NewImage, RotateSlider ->value(),
                                               NewImage ->width() / 2, NewImage ->height() / 2, InterpolationFunc);

    LabelControler::SetImage(NewImageLabel, NewImage, ScaleSlider ->value());
}
