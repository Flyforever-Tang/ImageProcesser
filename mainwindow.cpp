#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imageTransshaper.h"
#include "sharpeningOption.h"
#include "templateChart.h"
#include "huffmanTreeChart.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    CreateMsgBox("欢迎使用", "Developer：BIT_TYX");

    ui->setupUi(this);
    setWindowTitle("灰度图处理程序");

    OriImage = nullptr;
    NewImage = nullptr;
    Statiscian = nullptr;

    InitializeUI();

    /*测试用组件，默认为隐藏*/
    ui ->pushButton ->setEnabled(false);
    ui ->pushButton ->setVisible(false);
    ui ->textEdit ->setEnabled(false);
    ui ->textEdit ->setVisible(false);
}

MainWindow::~MainWindow()
{
    CreateMsgBox("感谢使用", "Developer：BIT_TYX");

    delete ui;
}

char* MainWindow::RecentImage() //暂未完成
{
    return nullptr;
}

void MainWindow::InitializeUI()
{
    SetEditDisabled();

    if(!RecentImage())
    {
        ui ->RecentImage ->setEnabled(false);
    }

    OriImageLabel = new QLabel(this);
    NewImageLabel = new QLabel(this);
    OriImageTextLabel = new QLabel(this);
    NewImageTextLabel = new QLabel(this);

    OriImageLabel ->setGeometry(50, 50, 512, 512);
    OriImageLabel ->setStyleSheet("border:2px dashed #242424;");
    NewImageLabel ->setGeometry(width() - 50 - 512, 50, 512, 512);
    NewImageLabel ->setStyleSheet("border:2px dashed #242424;");

    OriImageTextLabel ->setText("原始图像");
    OriImageTextLabel ->setGeometry(256 + 50 - 68, 565, 136, 40);
    OriImageTextLabel ->setFont(QFont("Timers", 20, QFont::Bold));
    OriImageTextLabel ->setStyleSheet("color:red");
    NewImageTextLabel ->setText("处理后图像");
    NewImageTextLabel ->setGeometry(width() - 170 - (256 + 50 - 85), 565, 170, 40);
    NewImageTextLabel ->setFont(QFont("Timers", 20, QFont::Bold));
    NewImageTextLabel ->setStyleSheet("color:red");
}

void MainWindow::SetEditDisabled()
{
    ui ->CompressImage ->setEnabled(false);
    ui ->SaveChange ->setEnabled(false);
    ui ->SaveImageAs ->setEnabled(false);
    ui ->ExportStatisticalResult ->setEnabled(false);

    ui ->SetSamplingRate ->setEnabled(false);
    ui ->SetQuantitativeLevel ->setEnabled(false);
    ui ->PointFunction1 ->setEnabled(false);
    ui ->PointFunction2 ->setEnabled(false);
    ui ->PointFunction3 ->setEnabled(false);
    ui ->PointFunction4 ->setEnabled(false);
    ui ->PointFunction5 ->setEnabled(false);
    ui ->Equalization_Traditional ->setEnabled(false);
    ui ->Equalization_Improved ->setEnabled(false);
    ui ->SmoothingImage_Overlimit ->setEnabled(false);
    ui ->SmoothingImage_MedianFilter ->setEnabled(false);
    ui ->SmoothingImage_KNNFilter ->setEnabled(false);
    ui ->SmoothingImage_ULMFilter ->setEnabled(false);
    ui ->SetSamplingRate ->setEnabled(false);
    ui ->SetQuantitativeLevel ->setEnabled(false);
    ui ->SharpeningImage ->setEnabled(false);
    ui ->EdgeClose ->setEnabled(false);
    ui ->HoughTransform ->setEnabled(false);
    ui ->OptionalConvolution ->setEnabled(false);
    ui ->CreateBinaryImage ->setEnabled(false);
    ui ->TranslateBmp_to_Txt ->setEnabled(false);   //暂未完成
    ui ->ImageTranssharp ->setEnabled(false);
    ui ->GrayStatistics ->setEnabled(false);
    ui ->Edit ->setEnabled(false);

    /*暂未完成*/
    ui ->Views_SaveImageAs ->setEnabled(false);
    ui ->Views_SetSamplingRate ->setEnabled(false);
    ui ->Views_SetQuantitativeLevel ->setEnabled(false);
    ui ->Views_OpenImage ->setEnabled(false);
    ui ->Views ->setEnabled(false);
    /**/
}

void MainWindow::SetEditEnabled()
{
    ui ->CompressImage ->setEnabled(true);
    ui ->SaveImageAs ->setEnabled(true);

    ui ->SetSamplingRate ->setEnabled(true);
    ui ->SetQuantitativeLevel ->setEnabled(true);
    ui ->PointFunction1 ->setEnabled(true);
    ui ->PointFunction2 ->setEnabled(true);
    ui ->PointFunction3 ->setEnabled(true);
    ui ->PointFunction4 ->setEnabled(true);
    ui ->PointFunction5 ->setEnabled(true);
    ui ->Equalization_Traditional ->setEnabled(true);
    ui ->Equalization_Improved ->setEnabled(true);
    ui ->SmoothingImage_Overlimit ->setEnabled(true);
    ui ->SmoothingImage_MedianFilter ->setEnabled(true);
    ui ->SmoothingImage_KNNFilter ->setEnabled(true);
    ui ->SmoothingImage_ULMFilter ->setEnabled(true);
    ui ->SetSamplingRate ->setEnabled(true);
    ui ->SetQuantitativeLevel ->setEnabled(true);
    ui ->SharpeningImage ->setEnabled(true);
    ui ->EdgeClose ->setEnabled(true);
    ui ->HoughTransform ->setEnabled(true);
    ui ->OptionalConvolution ->setEnabled(true);
    ui ->CreateBinaryImage ->setEnabled(true);
//    ui ->TranslateBmp_to_Txt ->setEnabled(true);    //暂未完成
    ui ->ImageTranssharp ->setEnabled(true);
    ui ->GrayStatistics ->setEnabled(true);
    ui ->Edit ->setEnabled(true);


    /*暂未完成*/
//    ui ->Views_SaveImageAs ->setEnabled(true);
//    ui ->Views_SetSamplingRate ->setEnabled(true);
//    ui ->Views_SetQuantitativeLevel ->setEnabled(true);
//    ui ->Views_OpenImage ->setEnabled(true);
//    ui ->Views ->setEnabled(true);
    /**/
}

void MainWindow::ChangeOriImage(QImage* Image)
{
    ImageControler::ChangeImage(OriImage, Image);
    LabelControler::SetImage(OriImageLabel, OriImage);
    LabelControler::ClearImage(NewImageLabel);

    delete Statiscian;
    Statiscian = new GrayImageStatiscian(Image);

    ui ->SaveChange ->setEnabled(false);
}

void MainWindow::ChangeNewImage(QImage* Image)
{
    ImageControler::ChangeImage(NewImage, Image);
    LabelControler::SetImage(NewImageLabel, NewImage);

    ui ->SaveChange ->setEnabled(true);
}

void MainWindow::CompareImage(QImage* Image_1, QImage* Image_2, QString Title_1, QString Title_2)
{
    StatisticalChart* Chart_1 = new StatisticalChart(10, 550, 512, 2, 256, Image_1);
    Chart_1 ->setWindowTitle(Title_1);
    Chart_1 ->show();

    StatisticalChart* Chart_2 = new StatisticalChart(10, 550, 512, 2, 256, Image_2);
    Chart_2 ->setWindowTitle(Title_2);
    Chart_2 ->show();
}

void MainWindow::PointOperation(Function& ConversionFunc)
{
    QImage* Image = GrayImageProcesser::PointOperation(OriImage, ConversionFunc);
    ChangeNewImage(Image);
    CompareImage(OriImage, NewImage, "灰度统计图-原图片", "灰度统计图-点运算变换");
}

void MainWindow::SharpeningOriImage(int (*GradProceser)(unsigned char, int, unsigned char *, int),
                                    unsigned char *List_L, int Threshold, int FilterNo)
{
    QImage* Image = GrayImageProcesser::SharpeningImage(OriImage, GradProceser, List_L, Threshold, FilterNo);
    ChangeNewImage(Image);
}

void MainWindow::ConvoluteOriImage(int TemplateWidth, int TemplateHeight, int **TemplateData)
{
    QImage* Image = GrayImageProcesser::ConvoluteImage(OriImage, TemplateWidth, TemplateHeight, TemplateData);
    ChangeNewImage(Image);
}

void MainWindow::on_pushButton_clicked()
{

/*
    QString FileName = "E:/Test2.jpg";
//    int ComPre = 50;

//    QString FileName = QFileDialog::getOpenFileName(this, tr("选择文件"), "E:\\", tr("Image (*.jpg)"));
    QImage* Image = new QImage;
    Image ->load(FileName);
    GrayImageProcesser* NewProcesser = new GrayImageProcesser(Image);

    qDebug()<<Image ->size();

    QImage *image;
    int ComPre = ui ->textEdit ->document() ->toPlainText().toInt();
    image = NewGrayImageProcesser::CompressImage_Res(ComPre);

//    int QuanLv = ui ->textEdit ->document() ->toPlainText().toInt();
//    image = NewGrayImageProcesser::CompressImage_Quan(QuanLv);

    qDebug()<<image ->size();

    int ImageWidth = Image ->width() / 2;
    int ImageHeight = Image ->height() / 2;
    ui ->NewImage ->setPixmap(QPixmap::fromImage((image ->scaled(ImageWidth, ImageHeight))));

*/
/*
    QString FileName = "E:/Test.jpg";
    QImage* Image = new QImage;
    Image ->load(FileName);
    Processer* NewProcesser = new Processer(Image);

    ui ->label_1 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(0) ->scaled(220, 220))));
    ui ->label_2 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(1) ->scaled(220, 220))));
    ui ->label_3 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(2) ->scaled(220, 220))));
    ui ->label_4 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(3) ->scaled(220, 220))));
    ui ->label_5 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(4) ->scaled(220, 220))));
    ui ->label_6 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(5) ->scaled(220, 220))));
    ui ->label_7 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(6) ->scaled(220, 220))));
    ui ->label_8 ->setPixmap(QPixmap::fromImage((NewGrayImageProcesser::SplitGrayImage(7) ->scaled(220, 220))));
*/

    /*
    QString FileName = "E:/Test.jpg";
    QImage* Image = new QImage;
    Image ->load(FileName);
    Processer* NewProcesser = new Processer(Image);

    int temp = ui ->textEdit ->document() ->toPlainText().toInt();

    QImage *image = NewGrayImageProcesser::CreateBinaryImage(temp);
    ui ->label ->setPixmap(QPixmap::fromImage((image ->scaled(720, 450))));*/


//    QString ImagePath = "D:/temp.jpg";
//    QImage* Image = new QImage(ImagePath);
//    ChangeOriImage(Image);

//    on_OptionalConvolution_triggered();

//    ChangeOriImage(NewImage);

//    ChangeNewImage(GrayImageProcesser::EdgeClose(Image, 100));



    QImage* Image = new QImage("D:/123.jpg");
    ChangeOriImage(Image);
    FILE* NewFile = fopen("D:/temp.hfm", "wb+");
    Node* root = GrayImageProcesser::CompressImage_Huffman(OriImage, NewFile);
    fclose(NewFile);
    HuffmanTreeChart* h = new HuffmanTreeChart(root);
    h->show();

//    FILE* OriFile = fopen("D:/tttt.rlen", "rb");

//    QImage* Image;
//    Image = GrayImageProcesser::DecompressImage_RunLength(OriFile);

//    ChangeOriImage(Image);
//    SetEditEnabled();





//    QString ImagePath = "E:/Test.jpg";
//    QImage* Image = new QImage(ImagePath);
//    ChangeOriImage(Image);
//    on_OptionalConvolution_triggered();

//    SharpeningOption S;
//    S.show();

//    unsigned char Test[12] = {0, 0, 255, 0, 255, 255, 255, 0, 0, 255, 255, 0};
//    QImage* Image = GrayImageProcesser::CreateTestImage(12, 1, Test);

//    QImage* Image = new QImage("D:/test.jpg");
//    LabelControler::SetImage(OriImageLabel ,Image);
//    Image = GrayImageProcesser::EdgeClose(Image, 2);
//    LabelControler::SetImage(NewImageLabel ,Image);
}

void MainWindow::on_OpenImage_triggered()
{
    QString ImagePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "D:/", tr("Image (*.jpg; *.png)"));

    if(!ImagePath.isEmpty())
    {
        QImage* Image = new QImage(ImagePath);
        ChangeOriImage(Image);
        SetEditEnabled();
    }
}

void MainWindow::on_RecentImage_triggered()
{

}

void MainWindow::on_SaveChange_triggered()
{
    ChangeOriImage(NewImage);
}

void MainWindow::on_SaveImageAs_triggered()
{
    QString SavePath = QFileDialog::getSaveFileName(this, tr("选择文件"), "D:/", tr("Image (*.jpg)"));
    OriImage ->save(SavePath, "JPG", 100);
}

void MainWindow::on_ExportStatisticalResult_triggered()
{

}

void MainWindow::on_Exit_triggered()
{
    qApp ->quit();
}

void MainWindow::on_SetSamplingRate_triggered()
{
    bool OK = false;
    int Comper = 0;
    Comper = QInputDialog::getInt(this, tr("采样率"),tr("请输入压缩比(1%~100%)"), 100, 1, 100, 1, &OK);
    if(OK && Comper >= 1 && Comper <= 100)
    {
        QImage* Image =  GrayImageProcesser::CompressImage_Res(OriImage, Comper);
        ChangeNewImage(Image);
    }
}

void MainWindow::on_SetQuantitativeLevel_triggered()
{
    bool OK = false;
    int QuanLv = 0;

    QuanLv = QInputDialog::getInt(this, tr("量化等级"),tr("请输入量化等级(2-256)"), 256, 2, 256, 1, &OK);

    if(OK && QuanLv >= 2 && QuanLv <= 256)
    {
        QImage* Image =  GrayImageProcesser::CompressImage_Quan(OriImage, QuanLv);
        ChangeNewImage(Image);
    }
}

void MainWindow::on_CreateBinaryImage_triggered()
{
    BinaryImageBox* ImageBox = new BinaryImageBox;

    for(int i = 0; i < 8; ++i)
    {
        QImage* BinaryImage = GrayImageProcesser::SplitGrayImage(OriImage, i);
        LabelControler::SetImage(ImageBox ->ImageLabel[i], BinaryImage);
    }

    ImageBox ->setWindowTitle("位平面二值图");
    ImageBox ->show();
}

void MainWindow::on_TranslateBmp_to_Txt_triggered()
{

}

void MainWindow::on_GrayStatistics_triggered()
{
    StatisticalChart* Chart = new StatisticalChart(10, 550, 512, 2, 256, OriImage);

    Chart ->setWindowTitle("灰度统计图");
    Chart ->show();

//    ui ->ExportStatisticalResult ->setEnabled(true);  //暂未完成
}

void MainWindow::on_PointFunction1_triggered()
{
    Function_x_plus_50* Function1 = new Function_x_plus_50;
    PointOperation(*Function1);
}

void MainWindow::on_PointFunction2_triggered()
{
    Function_1p5_multi_x* Function2 = new Function_1p5_multi_x;
    PointOperation(*Function2);
}

void MainWindow::on_PointFunction3_triggered()
{
    Function_nega_x_plus_256* Function3 = new Function_nega_x_plus_256;
    PointOperation(*Function3);
}

void MainWindow::on_PointFunction4_triggered()
{
    Function_x_square* Function4 = new Function_x_square;
    PointOperation(*Function4);
}

void MainWindow::on_PointFunction5_triggered()
{
    Function_256_multi_sin_lb_pi_multi_x_div_256_rb* Function5 = new Function_256_multi_sin_lb_pi_multi_x_div_256_rb;
    PointOperation(*Function5);
}

void MainWindow::on_Equalization_Traditional_triggered()
{
    QImage* Image = GrayImageProcesser::EqualizeImage_Traditional(OriImage);
    ChangeNewImage(Image);
    CompareImage(OriImage, NewImage, "灰度统计图-原图片", "灰度统计图-均衡化-传统方法");
}

void MainWindow::on_Equalization_Improved_triggered()
{
    bool OK = false;
    double Lambda = 0;

    Lambda = QInputDialog::getDouble(this, tr("权值"),tr("请输入均衡化权值λ(0-1.5)"), 1.0, 0, 1.5, 3, &OK);
    if(OK && Lambda >= 0 && Lambda <= 1.5)
    {
        QImage* Image = GrayImageProcesser::EqualizeImage_Improved(OriImage, Lambda);
        ChangeNewImage(Image);

        QString NewImageTitle = "灰度统计图-均衡化-改进方法";
        if(!Lambda)
        {
            NewImageTitle += "-未进行Laplacian边缘检测处理";
        }
        else
        {
            NewImageTitle += "-λ:" + QString::number(Lambda);
        }

        CompareImage(OriImage, NewImage, "灰度统计图-原图片", NewImageTitle);
    }
}

void MainWindow::on_ImageTranssharp_triggered()
{
    ImageTransshaper* Transshaper = new ImageTransshaper(this);
    Transshaper ->show();
}

void MainWindow::on_SmoothingImage_Overlimit_triggered()
{
    bool OK = false;
    int FilterNo, Threshold;

    FilterNo = QInputDialog::getInt(this, tr("滤波器模板"),tr("请输入模板编号(1-4)"), 1, 1, 4, 1, &OK);
    if(OK && FilterNo >= 1 && FilterNo <= 4)
    {
        OK = false;
        Threshold = QInputDialog::getInt(this, tr("阈值"),tr("请输入阈值(0-255)"), 48, 0, 255, 1, &OK);
        if(OK && Threshold > 0 && Threshold <= 256)
        {
            QImage* Image = GrayImageProcesser::SmoothingImage_Overlimit(OriImage, FilterNo - 1, Threshold);
            ChangeNewImage(Image);
        }
    }
}

void MainWindow::on_SmoothingImage_MedianFilter_triggered()
{
    bool OK = false;
    QString WindowType;

    WindowType = QInputDialog::getItem(this, "滤波窗口", tr("请选择滤波窗口类型"),
                                       QStringList() << "HorLine_5" << "Cross_9" << "Square_9", 0, false, &OK);
    if(OK && (WindowType == "HorLine_5" || WindowType == "Cross_9" || WindowType == "Square_9"))
    {
        QImage* Image = GrayImageProcesser::SmoothingImage_MedianFilter(OriImage, WindowType);
        ChangeNewImage(Image);
    }
}

void MainWindow::on_SmoothingImage_KNNFilter_triggered()
{
    bool OK = false;
    int WindowLength_m, NeighborSum_k;

k:  WindowLength_m = QInputDialog::getInt(this, tr("滤波窗口边长"),tr("请输入边长k(3-9，奇数)"), 3, 3, 9, 2, &OK);
    if(OK && WindowLength_m >= 3 && WindowLength_m <= 15)
    {
        if(!(WindowLength_m % 2))
        {
            QMessageBox::warning(nullptr, "输入错误", "请输入3-15的奇数");
            goto k;
        }

        OK = false;
        NeighborSum_k = QInputDialog::getInt(this, tr("近邻像素数"),tr("请输入近邻数m(2-k*k)"), WindowLength_m, 2,
                                              WindowLength_m * WindowLength_m, 1, &OK);
        if(OK && NeighborSum_k >= 2 && NeighborSum_k <= WindowLength_m * WindowLength_m)
        {
            QImage* Image = GrayImageProcesser::SmoothingImage_KNNFilter(OriImage, WindowLength_m, NeighborSum_k);
            ChangeNewImage(Image);
        }
    }
}

void MainWindow::on_SmoothingImage_ULMFilter_triggered()
{
    QImage* Image = GrayImageProcesser::SmoothingImage_ULMFilter(OriImage);
    ChangeNewImage(Image);
}

void MainWindow::on_SharpeningImage_triggered()
{
    SharpeningOption* Option = new SharpeningOption(&MainWindow::SharpeningOriImage, this);
    Option ->setWindowModality(Qt::ApplicationModal);
    Option ->show();
}

void MainWindow::on_OptionalConvolution_triggered()
{
    int TemplateWidth, TemplateHeight;
    bool OK_SetWidth = false, OK_SetHeight = false;

    TemplateWidth = QInputDialog::getInt(this, "模板宽度", tr("请设置模板宽度"), 3, 1, 29, 1, &OK_SetWidth);
    if(OK_SetWidth && TemplateWidth > 0 && TemplateWidth < 30)
    {
        TemplateHeight = QInputDialog::getInt(this, "模板长度", tr("请设置模板长度"), TemplateWidth, 1, 29, 1, &OK_SetHeight);
        if(OK_SetHeight && TemplateHeight > 0 && TemplateHeight < 30)
        {
            TemplateChart* Chart = new TemplateChart(&MainWindow::ConvoluteOriImage, this, TemplateWidth, TemplateHeight);
            Chart ->setWindowModality(Qt::ApplicationModal);
            Chart ->show();
        }
    }
}

void MainWindow::on_RGB_to_256Color_triggered()
{
    static QString LastPath = nullptr;
    QString ImagePath;
    QImage* Image = nullptr;

    int RGBFlag = false;
    while(!RGBFlag)
    {
        ImagePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "D:/", tr("Image (*.jpg;*.png)"));

        if(ImagePath.isEmpty())
        {
            ImagePath = LastPath;
        }
        Image = new QImage(ImagePath);
        RGBFlag = Image ->depth() == 32;
        if(!RGBFlag)
        {
            QMessageBox::warning(nullptr, "不是真彩图！", "所选图片不是真彩图，请重新选择！");
        }
    }

    LastPath = ImagePath;
    LabelControler::SetImage(OriImageLabel, Image);

    if(ImagePath.isEmpty())
    {
        return;
    }

    bool OK = false;
    int SampleBits = 0;

    SampleBits = QInputDialog::getInt(this, tr("截取位数"),tr("请输入截取位数(3-6)"), 4, 3, 6, 1, &OK);

    if(OK && SampleBits >= 3 && SampleBits <= 6)
    {
        LabelControler::SetImage(NewImageLabel, GrayImageProcesser::RGB_to_256Color(Image, SampleBits));
    }

    SetEditDisabled();
}

void MainWindow::on_EdgeClose_triggered()
{
    bool OK = false;
    int Tolerance = 0;

    Tolerance = QInputDialog::getInt(this, tr("间断容忍度"),tr("请输入间断容忍度(1-10)"), 1, 1, 10, 1, &OK);

    if(OK && Tolerance >= 1 && Tolerance <= 10)
    {
        QImage* Image =  GrayImageProcesser::EdgeClose(OriImage, Tolerance);
        ChangeNewImage(Image);
    }
}

void MainWindow::on_HoughTransform_triggered()
{
    bool OK = false;
    int LineThreshold = 0;

    LineThreshold = QInputDialog::getInt(this, tr("直线识别阈值"),tr("请输入直线识别阈值(20-500)"), 100, 20, 500, 1, &OK);

    if(OK && LineThreshold >= 20 && LineThreshold <= 500)
    {
        QImage* Image = GrayImageProcesser::HoughTransform(OriImage, LineThreshold);
        LabelControler::SetImage(NewImageLabel, Image);
    }
}

void MainWindow::on_CompressImage_triggered()
{
    bool OK = false;
    QString CompressAlgorithmic;

    CompressAlgorithmic = QInputDialog::getItem(this, "压缩算法", tr("请选择压缩算法"),
                                                QStringList() << "Huffman编码" << "游程编码", 0, false, &OK);
    if(OK)
    {
        QString SavePath;
        FILE* NewFile = nullptr;

        if(CompressAlgorithmic == "Huffman编码")
        {
            SavePath = QFileDialog::getSaveFileName(this, tr("压缩文件名"), "D:/", tr("Huffman (*.hfm)"));
            if(SavePath.isNull())
            {
                return;
            }
            NewFile = fopen(QString_to_Str(SavePath), "wb+");

            Node* HuffmanTreeRoot = GrayImageProcesser::CompressImage_Huffman(OriImage, NewFile);
            HuffmanTreeChart* Chart = new HuffmanTreeChart(HuffmanTreeRoot);
            Chart ->show();
        }
        else if(CompressAlgorithmic == "游程编码")
        {
            SavePath = QFileDialog::getSaveFileName(this, tr("压缩文件名"), "D:/", tr("RunLength (*.rlen)"));
            if(SavePath.isNull())
            {
                return;
            }
            NewFile = fopen(QString_to_Str(SavePath), "wb+");

            GrayImageProcesser::CompressImage_RunLength(OriImage, NewFile);
        }
        else
        {
            QMessageBox::warning(nullptr, "未知算法！", "请选择Huffman编码或游程编码算法！");
        }

        fclose(NewFile);
    }
}

void MainWindow::on_DecompressImage_triggered()
{
    QString ImagePath = QFileDialog::getOpenFileName(this, tr("选择文件"), "D:/", tr("Image (*.hfm; *.rlen)"));

    if(!ImagePath.isEmpty())
    {
        FILE* OriFile = fopen(QString_to_Str(ImagePath), "rb");

        QImage* Image = nullptr;
        if(ImagePath.right(4) == ".hfm")
        {
            Image = GrayImageProcesser::DecompressImage_Huffman(OriFile);
        }
        else if(ImagePath.right(5) == ".rlen")
        {
            Image = GrayImageProcesser::DecompressImage_RunLength(OriFile);
        }
        else
        {
            QMessageBox::warning(nullptr, "文件格式错误！", "请选择.hfm或.rlen文件！");
        }

        ChangeOriImage(Image);
        SetEditEnabled();
    }
}
