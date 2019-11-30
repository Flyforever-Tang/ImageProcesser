#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H

#include "imagelimitation.h"
#include <QImage>

/*灰度图*/

class GrayImage
{
protected:
    QImage* ThisImage;  //图片
    int ThisWidth;  //宽
    int ThisHeight; //高
    int ThisSize;   //大小（像素数）
    int ThisInterval;   //像素间隔位数（位深度/8）
    const unsigned char* ThisPicData;   //像素表表头

public:
    GrayImage(QImage* Image, bool Grayed = false, bool Normalized = false);

    QImage* GetQImage();    //返回ThisImage
    QImage* Gray(); //灰度化
    QImage* NormalizeImage();   //规范化灰度图（使宽为4的倍数）
};

inline QImage* GrayImage::GetQImage()
{
    return ThisImage;
}

#endif // GRAYIMAGE_H
