#include "labelControler.h"

LabelControler::LabelControler()
{

}

void LabelControler::SetImage(QLabel *TarLabel, QImage *TarImage, int ScaleValue)
{
    int* SuitableSize = GetSuitableSize(TarLabel ->width(), TarLabel ->height(), TarImage ->width(), TarImage ->height());
    TarLabel ->setPixmap(QPixmap::fromImage((TarImage ->scaled(SuitableSize[0] * ScaleValue / 100, SuitableSize[1] * ScaleValue / 100))));
    TarLabel ->setAlignment(Qt::AlignCenter);
}
