#ifndef GRAYIMAGESTATISCIAN_H
#define GRAYIMAGESTATISCIAN_H

#include "grayimage.h"
#include <math.h>

/*灰度图统计器*/

class GrayImageStatiscian : public GrayImage
{
public:
    int GrayValueStatisticResult[256];    //不同灰度值像素数
    int MaxSum; //各灰度像素数中的最大值
    double Average;  //灰度平均值
    double MidValue; //灰度中值
    double StandardDev; //灰度标准差
    int PixelSum;   //像素总数

public:
    GrayImageStatiscian(QImage* Image);

    void StatisticGrayValue();   //统计图片灰度值
    void GetMaxSum();   //计算最大值
    void GetAverage();   //计算平均值
    void GetMidValue();  //计算中值
    void GetStandardDev();   //计算标准差
};

#endif // GRAYIMAGESTATISCIAN_H
