#ifndef IMAGECONTROLER_H
#define IMAGECONTROLER_H

#include "grayimage.h"
#include <QImage>

/*图片控制器*/

class ImageControler
{
public:
    static void ChangeImage(QImage*& OriImage, QImage* NewImage);   //改变QImage
};

inline void ImageControler::ChangeImage(QImage*& OriImage, QImage* NewImage)
{
//    if(OriImage && OriImage ->bits())
//    {
//        delete OriImage ->bits();
//        delete OriImage;
//    }
    OriImage = (new GrayImage(NewImage)) ->GetQImage();
}

#endif // IMAGECONTROLER_H
