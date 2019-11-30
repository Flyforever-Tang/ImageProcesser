#ifndef GRAYIMAGEPROCESSER_H
#define GRAYIMAGEPROCESSER_H

#include "prolib.h"
#include "function.h"
#include "grayImageStatiscian.h"
#include <math.h>

/*灰度图处理器*/

class GrayImageProcesser
{
private:
    GrayImageProcesser();

    static void RefrashNewImageData(int DataSize, bool DeleteOriData = true);   //重新申请临时空间，初始化NewPicData

    static void Tracking(const unsigned char* OriData, int OriWidth, int OriHeight, bool TrakedFlag[], int PreList[], int PreLength,
                         int ThisPosition_x, int ThisPosition_y, int RemainingTolerance, int TackDirectionNo);  //边缘追踪

private:
    static QImage* ErrorImage;  //错误图片，处理异常时显示
    static unsigned char* NewImageData; //处理后生成图片的像素表表头

public:
    static QImage* CreateTestImage(int Width, int Height, unsigned char* Data);

    static QImage* CompressImage_Res(QImage* OriImage, int ComPer);  //按特定采样率（压缩百分比）压缩
    static QImage* CompressImage_Quan(QImage* OriImage, int QuanLv); //按特定量化等级压缩

    static QImage* SplitGrayImage(QImage* OriImage, int Place);  //将8位灰度图某一位拆分成位平面二值图
    static QImage* CreateBinaryImage(QImage* OriImage, int Threshold); //按特定阈值生成二值图

    static QImage* PointOperation(QImage* OriImage, Function& ConversionFunc);    //点运算
    static QImage* EqualizeImage_Traditional(QImage* OriImage); //直方图均衡化，传统方法
    static QImage* EqualizeImage_Improved(QImage* OriImage, double Lambda); //直方图均衡化，改进方法

    static QImage* ScaleImage(QImage* OriImage, int ScalePer, unsigned char (*InterpolationFunc)
        (const unsigned char* Array, int ArrayWidth, int ArrayHeight, double Tar_x, double Tar_y, int InvalidValue)); //缩放图片
    static QImage* RotateImage(QImage* OriImage, int RotateAngle, int Central_x, int Central_y, unsigned char (*InterpolationFunc)
        (const unsigned char* Array, int ArrayWidth, int ArrayHeight, double Tar_x, double Tar_y, int InvalidValue)); //旋转图片
    static QImage* MoveImage(QImage* OriImage, int x, int y);   //移动图片

    static QImage* SmoothingImage_Overlimit(QImage* OriImage, int FilterNo, int Threshold);  //图像平滑（超限像素平滑）
    static QImage* SmoothingImage_MedianFilter(QImage* OriImage, QString WindowType);    //图像平滑（中值滤波）
    static QImage* SmoothingImage_KNNFilter(QImage* OriImage, int WindowLength_m, int NeighborSum_k);   //图像平滑（K近邻均值滤波器）
    static QImage* SmoothingImage_ULMFilter(QImage* OriImage);   //图像平滑（最小均方差滤波器）

    static int GradProceser_Grad(unsigned char OriData, int Grad,
                                 unsigned char* List_L, int Threshold); //梯度处理，直接使用梯度
    static int GradProceser_Ori_Grad(unsigned char OriData, int Grad,
                                     unsigned char* List_L, int Threshold); //梯度处理，原像素<阈值>梯度
    static int GradProceser_Ori_LG(unsigned char OriData, int Grad,
                                   unsigned char* List_L, int Threshold);   //梯度处理，原像素<阈值>固定像素LG
    static int GradProceser_LB_Grad(unsigned char OriData, int Grad,
                                    unsigned char* List_L, int Threshold);  //梯度处理，固定像素LB<阈值>梯度
    static int GradProceser_LB_LG(unsigned char OriData, int Grad,
                                  unsigned char* List_L, int Threshold);    //梯度处理，固定像素LG<阈值>固定像素LB
    static QImage* SharpeningImage(QImage* OriImage, int (*GradProceser)
                                                         (unsigned char OriData, int Grad, unsigned char* List_L, int Threshold),
                                   unsigned char* List_L, int Threshold,  int FilterNo);    //图像锐化

    static QImage* ConvoluteImage(QImage* OriImage, int TemplateWidth, int TemplateHeight, int** TemplateData);  //图像卷积

    static QImage* RGB_to_256Color(QImage* OriImage, int SampleBits);   //真彩图转256色图

    static QImage* EdgeClose(QImage* OriImage, int Tolerance);   //轨迹追踪，边缘点闭合

    static QImage* HoughTransform(QImage* OriImage, int LineThreshold);    //霍夫变换检测直线

    static Node* CompressImage_Huffman(QImage* OriImage, FILE* NewFile);    //哈夫曼压缩
    static void CompressImage_RunLength(QImage *OriImage, FILE *NewFile);  //游程编码压缩

    static QImage* DecompressImage_Huffman(FILE* OriFile);  //哈夫曼解码
    static QImage* DecompressImage_RunLength(FILE* OriFile);    //游程编码解码
};

inline void GrayImageProcesser::RefrashNewImageData(int DataSize, bool DeleteOriData)
{
    if(DeleteOriData)
    {
        delete NewImageData;
    }

    NewImageData = new unsigned char[DataSize + 10];
    memset(NewImageData, 0, DataSize);
}

#endif // GRAYIMAGEPROCESSER_H
