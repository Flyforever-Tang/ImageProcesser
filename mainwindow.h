#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "binaryImageBox.h"
#include "statisticalChart.h"
#include <windows.h>
#include <pthread.h>
#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QComboBox>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    GrayImageStatiscian* Statiscian;

    QImage* OriImage;
    QImage* NewImage;

    QLabel* OriImageLabel;
    QLabel* NewImageLabel;
    QLabel* OriImageTextLabel;
    QLabel* NewImageTextLabel;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ChangeOriImage(QImage* Image); //改变原始图片
    void ChangeNewImage(QImage* Image); //改变处理后图片

private:
    void CreateMsgBox(QString Title, QString Msg);  //生成弹窗

    char* RecentImage();    //最近使用图片列表

    void InitializeUI();    //初始化UI
    void SetEditDisabled(); //禁用编辑功能
    void SetEditEnabled();  //启用编辑功能

    void CompareImage(QImage* Image_1, QImage* Image_2, QString Title_1 = "灰度统计图1", QString Title_2 = "灰度统计图2");    //比较灰度直方图

    void PointOperation(Function& ConversionFunc);  //点运算

    void SharpeningOriImage(int (*GradProceser)(unsigned char OriData, int Grad,unsigned char* L, int Threshold),
                            unsigned char* List_L, int Threshold, int FilterNo);

    void ConvoluteOriImage(int TemplateWidth, int TemplateHeight, int** TemplateData);

private slots:
    void on_pushButton_clicked();

    void on_OpenImage_triggered();
    void on_RecentImage_triggered();

    void on_SaveChange_triggered();
    void on_SaveImageAs_triggered();
    void on_ExportStatisticalResult_triggered();

    void on_Exit_triggered();

    void on_SetSamplingRate_triggered();
    void on_SetQuantitativeLevel_triggered();
    void on_CreateBinaryImage_triggered();
    void on_TranslateBmp_to_Txt_triggered();

    void on_GrayStatistics_triggered();

    void on_PointFunction1_triggered();
    void on_PointFunction2_triggered();
    void on_PointFunction3_triggered();
    void on_PointFunction4_triggered();
    void on_PointFunction5_triggered();

    void on_Equalization_Traditional_triggered();
    void on_Equalization_Improved_triggered();

    void on_ImageTranssharp_triggered();

    void on_SmoothingImage_Overlimit_triggered();
    void on_SmoothingImage_MedianFilter_triggered();
    void on_SmoothingImage_KNNFilter_triggered();
    void on_SmoothingImage_ULMFilter_triggered();

    void on_SharpeningImage_triggered();

    void on_OptionalConvolution_triggered();

    void on_RGB_to_256Color_triggered();

    void on_EdgeClose_triggered();

    void on_HoughTransform_triggered();

    void on_CompressImage_triggered();

    void on_DecompressImage_triggered();

private:
    Ui::MainWindow *ui;
};

inline void MainWindow::CreateMsgBox(QString Title, QString Msg)
{
    QMessageBox::information(nullptr, Title, Msg);
}

#endif // MAINWINDOW_H
