#ifndef LABLECONTROLER_H
#define LABLECONTROLER_H

#include "prolib.h"
#include <QLabel>
#include <QImage>

/*Lable控制器*/

class LabelControler
{
private:
    LabelControler();

public:
    static void SetImage(QLabel* TarLable, QImage* TarImage, int ScaleValue = 100);  //在指定lable以合适大小(缩放后)显示灰度图片
    static void ClearImage(QLabel* TarLable); //清除指定lable处的图片
};

inline void LabelControler::ClearImage(QLabel* TarLable)
{
    TarLable ->clear();
}

#endif // LABLECONTROLER_H
