#include "grayImageStatiscian.h"

GrayImageStatiscian::GrayImageStatiscian(QImage* Image) : GrayImage(Image)
{
    memset(GrayValueStatisticResult, 0, sizeof(GrayValueStatisticResult));
    MaxSum = 0;
    Average = 0;
    MidValue = 0;
    StandardDev = 0;
    PixelSum = ThisSize;

    StatisticGrayValue();
    GetMaxSum();
    GetAverage();
    GetMidValue();
    GetStandardDev();
}

void GrayImageStatiscian::StatisticGrayValue()
{
    for(int i = 0; i < PixelSum; ++i)
    {
        int GrayValue = ThisPicData[i];
        ++GrayValueStatisticResult[GrayValue];
    }
}

void GrayImageStatiscian::GetMaxSum()
{
    for(int GrayValue = 0; GrayValue < 256; ++GrayValue)
    {
        if(GrayValueStatisticResult[GrayValue] > MaxSum)
        {
            MaxSum = GrayValueStatisticResult[GrayValue];
        }
    }
}

void GrayImageStatiscian::GetAverage()
{
    for(int GrayValue = 0; GrayValue < 256; ++GrayValue)
    {
        Average += GrayValueStatisticResult[GrayValue] * GrayValue;
    }
    Average /= PixelSum;
}

void GrayImageStatiscian::GetMidValue()
{
    int Counter = 0;
    for(int GrayValue = 0; GrayValue < 256; ++GrayValue)
    {
        if(GrayValueStatisticResult[GrayValue])
        {
            Counter += GrayValueStatisticResult[GrayValue];
            if(Counter > PixelSum / 2)
            {
                MidValue = GrayValue;
                return;
            }
            else if(Counter == PixelSum / 2)
            {
                int _GrayValue;
                for(_GrayValue = GrayValue; GrayValueStatisticResult[GrayValue]; ++_GrayValue);

                if(PixelSum % 2)
                {
                    MidValue = _GrayValue;
                }
                else
                {
                    MidValue = (GrayValue + _GrayValue) / 2;
                }
                return;
            }
        }
    }
}

void GrayImageStatiscian::GetStandardDev()
{
    for(int GrayValue = 0; GrayValue < 256; ++GrayValue)
    {
        if(GrayValueStatisticResult[GrayValue])
        {
            StandardDev += pow((Average - GrayValue), 2) * GrayValueStatisticResult[GrayValue];
        }
    }
    StandardDev /= PixelSum;
    StandardDev = sqrt(StandardDev);
}
