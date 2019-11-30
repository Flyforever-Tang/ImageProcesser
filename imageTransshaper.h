#ifndef IMAGETRANSSHAPER_H
#define IMAGETRANSSHAPER_H

#include "imageControler.h"
#include "labelControler.h"
#include "grayImageProcesser.h"
#include "mainwindow.h"
#include "imagelabel.h"
#include "prolib.h"
#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>

class MainWindow;

namespace Ui {
class ImageTransshaper;
}

class ImageTransshaper : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow* ThisMainWindow; //主窗口

    QImage* OriImage;   //原图片
    QImage* NewImage;   //处理后图片

    QLabel* OriImageLabel;  //原始图片显示区域
    ImageLabel<ImageTransshaper>* NewImageLabel;    //处理后图片显示区域

    QSlider* ScaleSlider;   //控制图片缩进的滑动条
    QSlider* RotateSlider;  //控制图片旋转的滑动条
    QLabel* ScaleValueLabel;    //缩进值
    QLabel* RotateValueLabel;   //旋转值

    QLabel* MethodLabel;    //插值方法
    QComboBox* MethodBox;   //插值方法列表
    unsigned char (*InterpolationFunc)
        (const unsigned char* Array, int ArrayWidth, int ArrayHeight, double Tar_x, double Tar_y, int InvalidValue);    //插值函数

    QPushButton* ResetImageButton;    //重置图片按钮
    QPushButton* SaveImageButton;   //保存图片修改按钮
    QPushButton* SaveImageAsButton; //将图片另存为按钮


public:
    explicit ImageTransshaper();
    explicit ImageTransshaper(MainWindow* Window, QWidget *parent = nullptr);
    ~ImageTransshaper();

    void InitializeUI();    //初始化UI
    void MoveImage(int MoveDis_x, int MoveDis_y);   //移动图片

protected:
    void closeEvent(QCloseEvent* Event);

private:
    Ui::ImageTransshaper *ui;

private slots:
    void ScaleSliderMove();
    void RotateSliderMove();

    void ResetImage();
    void SaveImage();
    void SaveImageAs();

    void MethodBoxIndexChange();
};

#endif // IMAGETRANSSHAPER_H
