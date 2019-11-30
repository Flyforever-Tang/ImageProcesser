#include "grayimage.h"

GrayImage::GrayImage(QImage* Image, bool Grayed, bool Normalized)
{
    ThisImage = Image;
    ThisWidth = ThisImage ->width();
    ThisHeight = ThisImage ->height();
    ThisSize = ThisWidth * ThisHeight;
    ThisPicData = ThisImage ->bits();

    ThisInterval = Image ->depth() / 8;
    if(!Grayed)
    {
        ThisImage = Gray();
        ThisPicData = ThisImage ->bits();
    }
    if(!Normalized)
    {
        ThisImage = NormalizeImage();

        ThisWidth = ThisImage ->width();
        ThisHeight = ThisImage ->height();
        ThisSize = ThisWidth * ThisHeight;
        ThisPicData = ThisImage ->bits();
    }
}

QImage* GrayImage::Gray()
{
    const int Width = ThisWidth;
    const int Height = ThisHeight;
    QImage* NewImage = new QImage(Width, Height, QImage::Format_Indexed8);
    NewImage ->setColorCount(256);
    for(int i = 0; i < 256; ++i)
    {
        NewImage ->setColor(i, qRgb(i, i, i));
    }

    if(ThisInterval == 1)
    {
        for(int i = 0; i < Height; ++i)
        {
            const unsigned char* Src = (unsigned char*)ThisImage ->constScanLine(i);
            unsigned char* Dest = (unsigned char*)NewImage ->scanLine(i);
            memcpy(Dest, Src, Width);
        }
    }
    else
    {
        for(int i = 0; i < Height; ++i)
        {
            const QRgb* Src = (QRgb*)ThisImage ->constScanLine(i);
            unsigned char* Dest = (unsigned char*)NewImage ->scanLine(i);
            for(int j = 0; j < Width; ++j)
            {
                 Dest[j] = qGray(Src[j]);
            }
        }
    }

    ThisInterval = 1;
    return NewImage;
}

QImage* GrayImage::NormalizeImage()
{
    const int NewWidth = (ThisWidth + 3) / 4 * 4;
    const int NewHeight = ThisHeight;

    if(NewWidth == ThisWidth)  //原图片已规范化
    {
        return ThisImage;
    }

    QImage* NewImage = new QImage(ThisPicData, NewWidth, NewHeight, QImage::Format_Grayscale8);

    return NewImage;
}
