#include "grayImageProcesser.h"

QImage* GrayImageProcesser::ErrorImage = new QImage("");
unsigned char* GrayImageProcesser::NewImageData = nullptr;

void GrayImageProcesser::Tracking(const unsigned char* OriData, int OriWidth, int OriHeight, bool TrakedFlag[], int PreList[],
                                  int PreLength, int ThisPosition_x, int ThisPosition_y, int RemainingTolerance, int TackDirectionNo)
{
    static int DirectionType[9][9]
    {
        {
            4, 3, 2,
            5, 0, 1,
            6, 7, 8
        },
        {
            0, 0, 2,
            0, 0, 1,
            0, 0, 8
        },
        {
            0, 3, 2,
            0, 0, 1,
            0, 0, 0
        },
        {
            4, 3, 2,
            0, 0, 0,
            0, 0, 0
        },
        {
            4, 3, 0,
            5, 0, 0,
            0, 0, 0
        },
        {
            4, 0, 0,
            5, 0, 0,
            6, 0, 0
        },
        {
            0, 0, 0,
            5, 0, 0,
            6, 7, 0
        },
        {
            0, 0, 0,
            0, 0, 0,
            6, 7, 8
        },
        {
            0, 0, 0,
            0, 0, 1,
            0, 7, 8
        }
    };

    if(PreLength && OriData[ThisPosition_y * OriWidth + ThisPosition_x] > 50)
    {
        for(int i = 0; i < PreLength; ++i)
        {
            NewImageData[PreList[i]] = 255;
        }
    }
    else if(RemainingTolerance > -1)
    {
        for(int i = -1; i < 2; ++i)
        {
            for(int j = -1; j < 2; ++j)
            {
                int NewPosition = (ThisPosition_y + i) * OriWidth + ThisPosition_x + j;
                if(DirectionType[TackDirectionNo][(i + 1) * 3 + j + 1] && IsIn(0, OriWidth, ThisPosition_x + j) &&
                   IsIn(0, OriHeight, ThisPosition_y + i) && !TrakedFlag[NewPosition])
                {
                    PreList[PreLength] = NewPosition;
                    TrakedFlag[NewPosition] = true;
                    Tracking(OriData, OriWidth, OriHeight, TrakedFlag, PreList, PreLength + 1, ThisPosition_x + j,
                             ThisPosition_y + i, RemainingTolerance - 1, DirectionType[TackDirectionNo][(i + 1) * 3 + j + 1]);
                }
            }
        }
    }
}

QImage* GrayImageProcesser::CreateTestImage(int Width, int Height, unsigned char *Data)
{
    RefrashNewImageData(Width * Height);
    for(int i = 0; i < Height; ++i)
    {
        for(int j = 0; j < Width; ++j)
        {
            NewImageData[i * Width + j] = Data[i * Width + j];
        }
    }

    QImage* NewImage = new QImage(NewImageData, Width, Height, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::CompressImage_Res(QImage* OriImage, int Comper)
{
    if(Comper > 100 || Comper < 1)
    {
        return ErrorImage;
    }

    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();
    const int NewWidth = (OriWidth * Comper / 100 + 3) / 4 * 4;
    const int NewHeight = OriHeight * Comper / 100;
    const double SamplingSpace = 100 / (double)Comper;

    RefrashNewImageData(NewWidth * NewHeight);

    int NewDataIndex = 0;
    for(int i = 0; i < NewHeight; ++i)
    {
        int Index_y;
        if(i == NewHeight - 1)
        {
            Index_y = OriHeight - 1;
        }
        else
        {
            Index_y = (int)(i * SamplingSpace * 1);
        }
        for(int j = 0; j < NewWidth; ++j)
        {
            int Index_x;
            if(j == NewWidth - 1)
            {
                Index_x = OriHeight - 1;
            }
            else
            {
                Index_x = (int)(j * SamplingSpace * 1);
            }
            int OriDataIndex = OriWidth * Index_y + Index_x;
            NewImageData[NewDataIndex++] = OriImageData[OriDataIndex];
        }
    }

    QImage* NewImage = new QImage(NewImageData, NewWidth, NewHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::CompressImage_Quan(QImage* OriImage, int QuanLv)
{
    if(QuanLv > 256)
    {
        return ErrorImage;
    }

    const double Scale = 256 / QuanLv;
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const int OriSize = OriWidth * OriHeight;
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriSize);

    for(int DataIndex = 0; DataIndex < OriSize * 1; ++DataIndex)
    {
        NewImageData[DataIndex] = RoundOff(OriImageData[DataIndex * 1], Scale);
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::SplitGrayImage(QImage* OriImage, int Place)
{
    QImage* ImageArray;
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const int OriSize = OriWidth * OriHeight;
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriSize);

    for(int j = 0; j < OriSize; ++j)
    {
        int Bit = GetBit(OriImageData[j], Place);
        NewImageData[j] = Bit ? 255 : 0;
    }

    QImage *NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    ImageArray = NewImage;

    return ImageArray;
}

QImage* GrayImageProcesser::CreateBinaryImage(QImage* OriImage, int Threshold)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const int OriSize = OriWidth * OriHeight;
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriSize);

    for(int DataIndex = 0; DataIndex < OriSize * 1; ++DataIndex)
    {
        NewImageData[DataIndex] = OriImageData[DataIndex * 1] >= Threshold ? 255 : 0;
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::PointOperation(QImage* OriImage, Function& ConversionFunc)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const int OriSize = OriWidth * OriHeight;
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriSize);

    for(int DataIndex = 0; DataIndex < OriSize * 1; ++DataIndex)
    {
        NewImageData[DataIndex] = ConversionFunc(OriImageData[DataIndex * 1]) % 256;
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::EqualizeImage_Traditional(QImage* OriImage)
{
    GrayImageStatiscian* Statiscian = new GrayImageStatiscian(OriImage);

    double s[256];
    int L[256];

    s[0] = (double)Statiscian ->GrayValueStatisticResult[0] / Statiscian ->PixelSum;
    for(int GrayValue = 1; GrayValue < 256; ++GrayValue)
    {
        s[GrayValue] = (double)Statiscian ->GrayValueStatisticResult[GrayValue] / Statiscian ->PixelSum + s[GrayValue - 1];
    }

    for(int GrayValue = 0; GrayValue < 256; ++GrayValue)
    {
        L[GrayValue] = round(s[GrayValue] * 255 + 0.001);
    }

    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const int OriSize = OriWidth * OriHeight;
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriSize);

    for(int DataIndex = 0; DataIndex < OriSize * 1; ++DataIndex)
    {
        NewImageData[DataIndex] = L[OriImageData[DataIndex * 1]];
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::EqualizeImage_Improved(QImage* OriImage, double Lambda)
{
    QImage* TraditionalImage = EqualizeImage_Traditional(OriImage);
    const unsigned char* TraditionalPicData = TraditionalImage ->bits();
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();

    RefrashNewImageData(OriWidth * OriHeight, false);

    static const int AvgFilter[9] =
    {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    };

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int Sum = 0;

            for(int k = -1; k <= 1; ++k)
            {
                int y;
                y = i + k;
                if(y < 0 || y >= OriHeight)
                {
                    y = i;
                }

                for(int l = -1; l <= 1; ++l)
                {
                    int x;
                    x = j + l;
                    if(x < 0 || x >= OriWidth)
                    {
                        x = j;
                    }

                    Sum += TraditionalPicData[y * OriWidth + x] * AvgFilter[(k + 1) * 3 + l + 1];
                }
            }

            Sum = Sum / 18.0 + 0.5;

            if(Sum < 0)
            {
                Sum = 0;
            }
            else if(Sum >= 256)
            {
                Sum = 255;
            }

            NewImageData[i * OriWidth + j] = Sum;
        }
    }

    if(Lambda > 0)
    {
        QImage* AvgImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
        const unsigned char* AvgPicData = AvgImage ->bits();

        RefrashNewImageData(OriWidth * OriHeight ,false);

        static const int LapFilter[9] =
        {
            -1, -1, -1,
            -1, +8, -1,
            -1, -1, -1
        };

        for(int i = 0; i < OriHeight; ++i)
        {
            for(int j = 0; j < OriWidth; ++j)
            {
                int Sum = 0;

                for(int k = -1; k <= 1; ++k)
                {
                    int y;
                    y = i + k;
                    if(y < 0 || y >= OriHeight)
                    {
                        y = i;
                    }

                    for(int l = -1; l <= 1; ++l)
                    {
                        int x;
                        x = j + l;
                        if(x < 0 || x >= OriWidth)
                        {
                            x = j;
                        }

                        Sum += AvgPicData[y * OriWidth + x] * LapFilter[(k + 1) * 3 + l + 1];
                    }
                }

                if(Sum < 0)
                {
                    Sum = 0;
                }
                else if(Sum >= 256)
                {
                    Sum = 255;
                }

                NewImageData[i * OriWidth + j] = Sum;
            }
        }

        QImage* LapImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
        const unsigned char* LapPicData = LapImage ->bits();
        const int OriSize = OriWidth * OriHeight;

        RefrashNewImageData(OriSize ,false);

        for(int DataIndex = 0; DataIndex < OriSize; ++DataIndex)
        {
            int TempData = Lambda * (AvgPicData[DataIndex] + LapPicData[DataIndex]) + 0.5;

            if(TempData < 0)
            {
                TempData = 0;
            }
            else if(TempData >= 256)
            {
                TempData = 255;
            }

            NewImageData[DataIndex] = TempData;
        }
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::ScaleImage(QImage* OriImage, int ScalePer, unsigned char (*InterpolationFunc)
    (const unsigned char* Array, int ArrayWidth, int ArrayHeight, double Tar_x, double Tar_y, int InvalidValue))
{
    double InverseScale = 100.0 / ScalePer;
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();
    const int NewWidth = (OriWidth * ScalePer / 100 + 3) / 4 * 4;
    const int NewHeight = OriHeight * ScalePer / 100;

    RefrashNewImageData(NewWidth * NewHeight, false);

    for(int i = 0; i < NewHeight; ++i)
    {
        for(int j = 0; j < NewWidth; ++j)
        {
            double Ori_x = j * InverseScale;
            double Ori_y = i * InverseScale;

            NewImageData[i * NewWidth + j] = InterpolationFunc(OriImageData, OriWidth, OriHeight, Ori_x, Ori_y, 255);
        }
    }

    delete OriImage ->bits();
    delete OriImage;
    QImage* NewImage = new QImage(NewImageData, NewWidth, NewHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::RotateImage(QImage* OriImage, int RotateAngle, int Central_x, int Central_y, unsigned char (*InterpolationFunc)
    (const unsigned char* Array, int ArrayWidth, int ArrayHeight, double Tar_x, double Tar_y, int InvalidValue))
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight ,false);

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            double Ori_x = (j - Central_x) * cos(-RotateAngle / 180.0 * M_PI) -
                           (i - Central_y) * sin(-RotateAngle / 180.0 * M_PI) + Central_x;
            double Ori_y = (j - Central_x) * sin(-RotateAngle / 180.0 * M_PI) +
                           (i - Central_y) * cos(-RotateAngle / 180.0 * M_PI) + Central_y;

            NewImageData[i * OriWidth + j] = InterpolationFunc(OriImageData, OriWidth, OriHeight, Ori_x, Ori_y, 255);
        }
    }

    delete OriImage ->bits();
    delete OriImage;
    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::MoveImage(QImage* OriImage, int x, int y)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight, false);

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            const int OriImageIndex_x = j - x;
            const int OriImageIndex_y = i - y;

            if(OriImageIndex_x < 0 || OriImageIndex_x >= OriWidth || OriImageIndex_y < 0 || OriImageIndex_y >= OriHeight)
            {
                NewImageData[i * OriWidth + j] = 255;
            }
            else
            {
                NewImageData[i * OriWidth + j] = OriImageData[OriImageIndex_y * OriWidth + OriImageIndex_x];
            }
        }
    }

    delete OriImage ->bits();
    delete OriImage;
    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::SmoothingImage_Overlimit(QImage *OriImage, int FilterNo, int Threshold)
{
    static const int Filter[4][10] =
    {
        {
            1, 1, 1,
            1, 2, 1,
            1, 1, 1,
            10
        },
        {
            1, 2, 1,
            2, 4, 2,
            1, 2, 1,
            16
        },
        {
            1, 1, 1,
            1, 0, 1,
            1, 1, 1,
            8
        },
        {
            0, 1, 0,
            1, 4, 1,
            0, 1, 0,
            8
        }
    };

    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int Sum = 0;
            for(int k = -1; k < 2; ++k)
            {
                for(int l = -1; l < 2; ++l)
                {
                    Sum += GetArrayValue<unsigned char>(OriImageData, OriWidth, OriHeight, j + k, i + l,
                                                        OriImageData[i * OriWidth + j]) * Filter[FilterNo][(k + 1) * 3 + l + 1];
                }
            }

            Sum = Sum / (float)Filter[FilterNo][9] + 0.5;

            if(Sum < 0)
            {
                Sum = 0;
            }
            else if(Sum >= 256)
            {
                Sum = 255;
            }

            NewImageData[i * OriWidth + j] = fabs(Sum - OriImageData[i * OriWidth + j]) > Threshold ? Sum : OriImageData[i * OriWidth + j];
        }
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::SmoothingImage_MedianFilter(QImage *OriImage, QString WindowType)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);

    static int (*WindowElementNo)[2];
    int ElementSum;

    if(WindowType == "HorLine_5")
    {
        WindowElementNo = new int[5][2]
        {
            {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}
        };
        ElementSum = 5;
    }
    else if(WindowType == "Cross_9")
    {
        WindowElementNo = new int[9][2]
        {
                              {0, -2},
                              {0, -1},
            {-2, 0}, {-1, 0}, {0,  0}, {1, 0}, {2, 0},
                              {0,  1},
                              {0,  2}
        };
        ElementSum = 9;
    }
    else if(WindowType == "Square_9")
    {
        WindowElementNo = new int[9][2]
        {
            {-1, -1}, {0, -1}, {1, -1},
            {-1,  0}, {0,  0}, {1,  0},
            {-1,  1}, {0,  1}, {1,  1}
        };
        ElementSum = 9;
    }
    else
    {
        WindowElementNo = new int[1][2]
        {
            {0, 0}
        };
        ElementSum = 1;
    }

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int AvailableSum = ElementSum;
            unsigned char* DataArray = GetArrayValue(OriImageData, OriWidth, OriHeight, j, i, WindowElementNo, AvailableSum);
            NewImageData[i * OriWidth + j] = GetMedian(DataArray, AvailableSum);

            delete[] DataArray;
        }
    }

    delete[] WindowElementNo;

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::SmoothingImage_KNNFilter(QImage *OriImage, int WindowLength_m, int NeighborSum_k)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);

    int (*WindowElementNo)[2] = new int[WindowLength_m][2];
    int Median = WindowLength_m / 2;
    for(int i = 0; i < WindowLength_m; ++i)
    {
        for(int j = 0; j < WindowLength_m; ++j)
        {
            WindowElementNo[i * WindowLength_m + j][0] = j - Median;
            WindowElementNo[i * WindowLength_m + j][1] = i - Median;
        }
    }

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int AvailableSum = WindowLength_m * WindowLength_m;
            unsigned char* DataArray = GetArrayValue(OriImageData, OriWidth, OriHeight, j, i, WindowElementNo, AvailableSum);
            DataArray = GetNeighbor(DataArray, AvailableSum, OriImageData[i * OriWidth + j], NeighborSum_k);
            NewImageData[i * OriWidth + j] = GetAverage(DataArray, NeighborSum_k);

            delete[] DataArray;
        }
    }

    delete[] WindowElementNo;

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::SmoothingImage_ULMFilter(QImage *OriImage)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);

    static const int (*WindowElementNo[9])[2] =
    {
        new int[7][2]
        {
            {-2, -1}, {-1, -1},
            {-2,  0}, {-1,  0}, { 0,  0},
            {-2,  1}, {-1,  1}
        },
        new int[7][2]
        {
                      { 1, -1}, { 2, -1},
            { 0,  0}, { 1,  0}, { 2,  0},
                      { 1,  1}, { 2,  1}
        },
        new int[7][2]
        {
            {-1, -2}, { 0, -2}, { 1, -2},
            {-1, -1}, { 0, -1}, { 1, -1},
                      { 0,  0}
        },
        new int[7][2]
        {
                      { 0,  0},
            {-1,  1}, { 0,  1}, { 1,  1},
            {-1,  2}, { 0,  2}, { 1,  2}
        },
        new int[7][2]
        {
            {-2, -2}, {-1, -2},
            {-2, -1}, {-1, -1}, { 0, -1},
                      {-1,  0}, { 0,  0}
        },
        new int[7][2]
        {
                      { 1, -2}, { 2, -2},
            { 0, -1}, { 1, -1}, { 2, -1},
            { 0,  0}, { 1,  0}
        },
        new int[7][2]
        {
                      {-1,  0}, { 0,  0},
            {-2,  1}, {-1,  1}, { 0,  1},
            {-2,  2}, {-1,  2}
        },
        new int[7][2]
        {
            { 0,  0}, { 1,  0},
            { 0,  1}, { 1,  1}, { 2,  1},
                      { 1,  2}, { 2,  2}
        },
        new int[9][2]
        {
            {-1, -1}, { 0, -1}, { 1, -1},
            {-1,  0}, { 0,  0}, { 1,  0},
            {-1,  1}, { 0,  1}, { 1,  1}
        }
    };

    unsigned char* DataArray[9];
    int AvailableSum[9];
    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            double Variance[9];
            for(int k = 0; k < 9; ++k)
            {
                AvailableSum[k] = k == 8 ? 7 : 9;
                DataArray[k] = GetArrayValue(OriImageData, OriWidth, OriHeight, j, i, WindowElementNo[k], AvailableSum[k]);
                Variance[k] = GetVariance(DataArray[k], AvailableSum[k]);
            }
            int SuitableNo = GetMostValueNo(Variance, 9);
            NewImageData[i * OriWidth + j] = GetAverage(DataArray[SuitableNo], AvailableSum[SuitableNo]);

            delete[] &DataArray;
        }
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

int GrayImageProcesser::GradProceser_Grad(unsigned char OriData, int Grad,
                                       unsigned char* List_L, int Threshold)
{
    return Grad;
}

int GrayImageProcesser::GradProceser_Ori_Grad(unsigned char OriData, int Grad,
                                           unsigned char* List_L, int Threshold)
{
    return Grad < Threshold ? OriData : Grad;
}

int GrayImageProcesser::GradProceser_Ori_LG(unsigned char OriData, int Grad,
                                         unsigned char* List_L, int Threshold)
{
    return Grad < Threshold ? OriData : List_L[0];
}

int GrayImageProcesser::GradProceser_LB_Grad(unsigned char OriData, int Grad,
                                          unsigned char* List_L, int Threshold)
{
    return Grad < Threshold ? List_L[0] : OriData;
}

int GrayImageProcesser::GradProceser_LB_LG(unsigned char OriData, int Grad,
                                        unsigned char* List_L, int Threshold)
{
    return Grad < Threshold ? List_L[0] : List_L[1];
}

QImage* GrayImageProcesser::SharpeningImage(QImage* OriImage, int (*GradProceser)
                                            (unsigned char OriData, int Grad, unsigned char* List_L, int Threshold),
                                            unsigned char* List_L, int Threshold, int FilterNo)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);

    static const int Filter[9][9] =
    {
        {   //Laplace模板
             0, -1,  0,
            -1,  4, -1,
             0, -1,  0
        },

        {   //Laplace增强模板
            0, -1,  0,
           -1,  5, -1,
            0, -1,  0
        },
        {   //高通模板1
           -1, -1, -1,
           -1,  8, -1,
           -1, -1, -1
        },

        {   //高通模板2
            1, -2,  1,
           -2,  4, -2,
            1, -2,  1
        },
        {   //高通模板3
            0, -1,  0,
           -1,  5, -1,
            0, -1,  0
        },
        {   //Sobel模板_x
            1,  0, -1,
            2,  0, -2,
            1,  0, -1
        },
        {   //Prewitt模板_x
            1,  0, -1,
            1,  0, -1,
            1,  0, -1
        },
        {   //Sobel模板_y
           -1, -2, -1,
            0,  0,  0,
            1,  2,  1
        },
        {   //Prewitt模板_y
           -1, -1, -1,
            0,  0,  0,
            1,  1,  1
        }
    };

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int Grad = 0;
            for(int k = -1; k < 2; ++k)
            {
                for(int l = -1; l < 2; ++l)
                {
                    Grad += GetArrayValue<unsigned char>(OriImageData, OriWidth, OriHeight, j + l, i + k,
                                                         OriImageData[i * OriWidth + j]) * Filter[FilterNo][(k + 1) * 3 + l + 1];
                }
            }
            if(FilterNo > 4)
            {
                int Grad_y = 0;
                for(int k = -1; k < 2; ++k)
                {
                    for(int l = -1; l < 2; ++l)
                    {
                        Grad_y += GetArrayValue<unsigned char>(OriImageData, OriWidth, OriHeight, j + l, i + k,
                                                            OriImageData[i * OriWidth + j]) * Filter[FilterNo + 2][(k + 1) * 3 + l + 1];
                    }
                    Grad = Grad > Grad_y ? Grad : Grad_y;
                }
            }

            Grad = GradProceser(OriImageData[i * OriWidth + j], Grad, List_L, Threshold);

            if(Grad < 0)
            {
                Grad = 0;
            }
            else if(Grad >= 256)
            {
                Grad = 255;
            }

            NewImageData[i * OriWidth + j] = Grad;
        }
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::ConvoluteImage(QImage* OriImage, int TemplateWidth, int TemplateHeight, int** TemplateData)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int Sum = 0;
            for(int k = -((TemplateHeight - 1) / 2); k < TemplateHeight / 2 + 1; ++k)
            {
                for(int l = -((TemplateWidth - 1) / 2); l < TemplateWidth / 2 + 1; ++l)
                {
                    Sum += GetArrayValue<unsigned char>(OriImageData, OriWidth, OriHeight, j + k, i + l,
                                                        OriImageData[i * OriWidth + j]) *
                                                        TemplateData[(k + (TemplateHeight - 1) / 2)][l + (TemplateWidth - 1) / 2];
                }
            }

            if(Sum < 0)
            {
                Sum = 0;
            }
            else if(Sum >= 256)
            {
                Sum = 255;
            }

            NewImageData[i * OriWidth + j] = Sum;
        }
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::RGB_to_256Color(QImage *OriImage, int SampleBits)
{
    class RGB
    {
    public:
        unsigned char R;
        unsigned char G;
        unsigned char B;

        RGB(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) :
            R(r),
            G(g),
            B(b)
        {

        }
    };

    class StatisticalRGB
    {
    public:
        int Sum;
        int HighRGBComplex;
        RGB AverageRGB;
        RGB HighRGB;

        StatisticalRGB() :
            Sum(0)
        {

        }

        void GetNew(RGB NewRGB)
        {
            AverageRGB.R = GetAverage(AverageRGB.R, NewRGB.R, Sum);
            AverageRGB.G = GetAverage(AverageRGB.G, NewRGB.G, Sum);
            AverageRGB.B = GetAverage(AverageRGB.B, NewRGB.B, Sum);
            ++Sum;
        }

        int operator -(StatisticalRGB Subtrahend)
        {
            return Sum - Subtrahend.Sum;
        }
    };

    const int Base = Pow_int(2, SampleBits);
    const int PaletteRGBSum = Pow_int(Base, 3);
    StatisticalRGB* Palette = new StatisticalRGB[PaletteRGBSum];

    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight * 4);

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int HighBits_R, HighBits_G, HighBits_B;
            HighBits_R = OriImageData[i * OriWidth * 4 + j * 4] >> (8 - SampleBits);
            HighBits_G = OriImageData[i * OriWidth * 4 + j * 4 + 1] >> (8 - SampleBits);
            HighBits_B = OriImageData[i * OriWidth * 4 + j * 4 + 2] >> (8 - SampleBits);
            int Complex = HighBits_R * Base * Base + HighBits_G * Base + HighBits_B;
            Palette[Complex].HighRGBComplex = Complex;
            Palette[Complex].HighRGB = RGB(HighBits_R, HighBits_G, HighBits_B);
            Palette[Complex].GetNew(RGB(OriImageData[i * OriWidth * 4 + j * 4], OriImageData[i * OriWidth * 4 + j * 4 + 1],
                                        OriImageData[i * OriWidth * 4 + j * 4 + 2]));
        }
    }

    qsort(Palette, Pow_int(Base, 3), sizeof(StatisticalRGB), Comp_Bigger<StatisticalRGB>);

    int* MappingTable = new int[PaletteRGBSum];

    for(int i = 0; i < 256 && Palette[i].Sum; ++i)
    {
        MappingTable[Palette[i].HighRGBComplex] = i;
    }
    for(int i = 256; i < PaletteRGBSum && Palette[i].Sum; ++i)
    {
        int MinVariance = Pow_int(Palette[i].HighRGB.R - Palette[0].HighRGB.R, 2) +
                          Pow_int(Palette[i].HighRGB.G - Palette[0].HighRGB.G, 2) +
                          Pow_int(Palette[i].HighRGB.B - Palette[0].HighRGB.B, 2);
        int MinNo = 0;

        for(int j = 1; j < 256; ++j)
        {
            int Variance = Pow_int(Palette[i].HighRGB.R - Palette[j].HighRGB.R, 2) +
                           Pow_int(Palette[i].HighRGB.G - Palette[j].HighRGB.G, 2) +
                           Pow_int(Palette[i].HighRGB.B - Palette[j].HighRGB.B, 2);
            if(Variance < MinVariance)
            {
                MinVariance = Variance;
                MinNo = j;
            }
        }
        MappingTable[Palette[i].HighRGBComplex] = MinNo;
    }

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            int HighBits_R, HighBits_G, HighBits_B;
            HighBits_R = OriImageData[i * OriWidth * 4 + j * 4] >> (8 - SampleBits);
            HighBits_G = OriImageData[i * OriWidth * 4 + j * 4 + 1] >> (8 - SampleBits);
            HighBits_B = OriImageData[i * OriWidth * 4 + j * 4 + 2] >> (8 - SampleBits);
            int Complex = HighBits_R * Base * Base + HighBits_G * Base + HighBits_B;

            NewImageData[i * OriWidth * 4 + j * 4] = Palette[MappingTable[Complex]].AverageRGB.R;
            NewImageData[i * OriWidth * 4 + j * 4 + 1] = Palette[MappingTable[Complex]].AverageRGB.G;
            NewImageData[i * OriWidth * 4 + j * 4 + 2] = Palette[MappingTable[Complex]].AverageRGB.B;
        }
    }

    delete[] Palette;
    delete[] MappingTable;

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_RGB32);
    return NewImage;
}

QImage* GrayImageProcesser::EdgeClose(QImage* OriImage, int Tolerance)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight);
    memcpy(NewImageData, OriImageData, OriWidth * OriHeight * sizeof(unsigned char));

    int* PreList = new int[Tolerance];
    static bool* TrakedFlag = new bool[MaxSize];
    memset(TrakedFlag, 0, OriWidth * OriHeight * sizeof(bool));

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            if(OriImageData[i * OriWidth + j] > 50)
            {
                Tracking(OriImageData, OriWidth, OriHeight, TrakedFlag, PreList, 0, j, i, Tolerance, 0);
                TrakedFlag[i * OriWidth + j] = true;
            }
        }
    }

    delete[] PreList;

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::HoughTransform(QImage *OriImage, int LineThreshold)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight * 4);

    static const double PI = acos(-1);
    static int StatisticTabel[(MaxWidth + MaxHeight) * 2][360];
    memset(StatisticTabel, 0, 360 * (MaxWidth + MaxHeight) * 2 * sizeof(int));

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            if(OriImageData[i * OriWidth + j] > 200)
            {
                for(int Theta = -180; Theta <= 180; ++Theta)
                {
                    double Radian = Theta / 180.0 * PI;
                    int P = j * cos(Radian) + i * sin(Radian);
                    ++StatisticTabel[P + MaxWidth + MaxHeight][Theta + 180];
                }
            }
            NewImageData[i * OriWidth * 4 + j * 4] = OriImageData[i * OriWidth + j];
            NewImageData[i * OriWidth * 4 + j * 4 + 1] = OriImageData[i * OriWidth + j];
            NewImageData[i * OriWidth * 4 + j * 4 + 2] = OriImageData[i * OriWidth + j];
            NewImageData[i * OriWidth * 4 + j * 4 + 3] = 0;
        }
    }

    for(int i = 0; i < (MaxWidth + MaxHeight) * 2; ++i)
    {
        for(int j = 0; j <= 360; ++j)
        {
            if(StatisticTabel[i][j] > LineThreshold)
            {
                for(int k = 0; k < OriHeight; ++k)
                {
                    for(int l = 0; l < OriWidth; ++l)
                    {
                        double Radian = (j - 180) / 180.0 * PI;
                        if(OriImageData[k * OriWidth + l] > 200 && fabs(l * cos(Radian) + k * sin(Radian) + MaxWidth + MaxHeight - i) < 1)
                        {
                            NewImageData[k * OriWidth * 4 + l * 4] = 0;
                            NewImageData[k * OriWidth * 4 + l * 4 + 1] = 0;
                            NewImageData[k * OriWidth * 4 + l * 4 + 2] = 255;
                            NewImageData[k * OriWidth * 4 + l * 4 + 3] = 0;
                        }
                    }
                }
            }
        }
    }

    QImage* NewImage = new QImage(NewImageData, OriWidth, OriHeight, QImage::Format_RGB32);
    return NewImage;
}

Node* GrayImageProcesser::CompressImage_Huffman(QImage *OriImage, FILE *NewFile)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight * 2);

    int GrayValueStatisticResult[256];
    int NewDataIndex = 0;
    HuffmanTree* Tree;

    memset(GrayValueStatisticResult, 0, 256 * sizeof(int));
    for(int i = 0; i < OriWidth * OriHeight; ++i)
    {
        int GrayValue = OriImageData[i];
        ++GrayValueStatisticResult[GrayValue];
    }

    NewImageData[NewDataIndex++] = OriWidth >> 8;
    NewImageData[NewDataIndex++] = OriWidth - (OriWidth >> 8 << 8);
    NewImageData[NewDataIndex++] = OriHeight >> 8;
    NewImageData[NewDataIndex++] = OriHeight - (OriHeight >> 8 << 8);

    Tree = new HuffmanTree(256, GrayValueStatisticResult);
    bool* (*HuffmanCodeList) = Tree ->CodeList;
    int* HuffmanCodeLengthList = Tree ->CodeLengthList;

    for(int i = 0; i < 256; ++i)
    {
        if(HuffmanCodeList[i])
        {
            NewImageData[NewDataIndex++] = i;
            NewImageData[NewDataIndex++] = HuffmanCodeLengthList[i];
            ByteFillter CodeFilter(HuffmanCodeList[i], HuffmanCodeLengthList[i], NewImageData, NewDataIndex, true);
        }
    }

    NewImageData[NewDataIndex++] = 255;
    NewImageData[NewDataIndex++] = 255;

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            ByteFillter CodeFilter(HuffmanCodeList[OriImageData[i * OriWidth + j]],
                                   HuffmanCodeLengthList[OriImageData[i * OriWidth + j]], NewImageData, NewDataIndex);
        }
    }

    fwrite(NewImageData, sizeof(unsigned char), NewDataIndex, NewFile);

    return Tree ->HuffmanRoot;
}

void GrayImageProcesser::CompressImage_RunLength(QImage *OriImage, FILE *NewFile)
{
    const int OriWidth = OriImage ->width();
    const int OriHeight = OriImage ->height();
    const unsigned char* OriImageData = OriImage ->bits();

    RefrashNewImageData(OriWidth * OriHeight * 2);

    unsigned char SumLength;
    int DataIndex = 0;

    int SameIndex = 0;
    int* SameSum = new int[OriWidth * OriHeight];
    unsigned char* SameData = new unsigned char[OriWidth * OriHeight];
    memset(SameSum, 0, OriWidth * OriHeight * sizeof(int));
    memset(SameData, 0, OriWidth * OriHeight * sizeof(unsigned char));
    SameData[0] = OriImageData[0];

    for(int i = 0; i < OriHeight; ++i)
    {
        for(int j = 0; j < OriWidth; ++j)
        {
            if(OriImageData[i * OriWidth + j] == SameData[SameIndex])
            {
                ++SameSum[SameIndex];
            }
            else
            {
                ++SameIndex;
                SameData[SameIndex] = OriImageData[i * OriWidth + j];
                SameSum[SameIndex] = 1;
            }
        }
    }

    int MaxSum = SameSum[GetMostValueNo(SameSum, SameIndex, false)];
    for(SumLength = 1; MaxSum >>= 8; ++SumLength)
    {
        continue;
    }

    NewImageData[DataIndex++] = OriWidth >> 8;
    NewImageData[DataIndex++] = OriWidth - (OriWidth >> 8 << 8);
    NewImageData[DataIndex++] = OriHeight >> 8;
    NewImageData[DataIndex++] = OriHeight - (OriHeight >> 8 << 8);

    NewImageData[DataIndex++] = SumLength;

    for(int i = 0; i < SameIndex; ++i)
    {
        for(int k = 1; k <= SumLength; ++k)
        {
            NewImageData[DataIndex++] = (SameSum[i] >> ((SumLength - k) << 3)) -
                                        (SameSum[i] >> ((SumLength - k + 1) << 3) << ((SumLength - k + 1) << 3));
        }
        NewImageData[DataIndex++] = SameData[i];
    }

    fwrite(NewImageData, sizeof(unsigned char), DataIndex, NewFile);
}

QImage* GrayImageProcesser::DecompressImage_Huffman(FILE *OriFile)
{
    unsigned char Width_Binary[2];
    unsigned char Height_Binary[2];
    fread(Width_Binary, sizeof(unsigned char), 2, OriFile);
    fread(Height_Binary, sizeof(unsigned char), 2, OriFile);

    int Width = Width_Binary[0] * 256 + Width_Binary[1];
    int Height = Height_Binary[0] * 256 + Height_Binary[1];
    RefrashNewImageData(Width * Height);

    bool* (*HuffmanCodeList) = new bool*[256];
    int* HuffmanCodeLengthList = new int[256];
    memset(HuffmanCodeLengthList, 0, 256 * sizeof(int));

    while(1)
    {
        unsigned char GrayValue[1];
        unsigned char CodeLength[1];
        fread(GrayValue, sizeof(unsigned char), 1, OriFile);
        fread(CodeLength, sizeof(unsigned char), 1, OriFile);

        if(GrayValue[0] == 255 && CodeLength[0] == 255)
        {
            break;
        }
        else
        {
            unsigned char* CodeList = new unsigned char[(CodeLength[0] + 7) / 8];
            fread(CodeList, sizeof(unsigned char), (CodeLength[0] + 7) / 8, OriFile);

            HuffmanCodeList[GrayValue[0]] = new bool[CodeLength[0]];
            HuffmanCodeLengthList[GrayValue[0]] = CodeLength[0];

            int i;
            for(i = 0; (i + 1) * 8 < CodeLength[0]; ++i)
            {
                for(int j = 0; j < 8; ++j)
                {
                    HuffmanCodeList[GrayValue[0]][i * 8 + j] = GetBit(CodeList[i], 7 - j);
                }
            }
            for(int j = CodeLength[0] - i * 8 - 1; j >=0; --j)
            {
                HuffmanCodeList[GrayValue[0]][i * 8 + j] = GetBit(CodeList[i], 7 - j);
            }
        }
    }

    HuffmanTree* Tree = new HuffmanTree(HuffmanCodeList, HuffmanCodeLengthList, 256);
    int DataIndex = 0;

    while(1)
    {
        int DecodeResult = Tree ->Decode();
        while(DecodeResult < 0)
        {
            unsigned char CurrentByte[1];
            if(fread(CurrentByte, sizeof(unsigned char), 1, OriFile) < 1)
            {
                QImage* NewImage = new QImage(NewImageData, Width, Height, QImage::Format_Grayscale8);
                return NewImage;
            }
            DecodeResult = Tree ->Decode(UChar_to_Byte(CurrentByte[0]));
        }

        NewImageData[DataIndex++] = DecodeResult;
    }

    QImage* NewImage = new QImage(NewImageData, Width, Height, QImage::Format_Grayscale8);
    return NewImage;
}

QImage* GrayImageProcesser::DecompressImage_RunLength(FILE *OriFile)
{
    unsigned char Width_Binary[2];
    unsigned char Height_Binary[2];
    fread(Width_Binary, sizeof(unsigned char), 2, OriFile);
    fread(Height_Binary, sizeof(unsigned char), 2, OriFile);

    int Width = Width_Binary[0] * 256 + Width_Binary[1];
    int Height = Height_Binary[0] * 256 + Height_Binary[1];
    RefrashNewImageData(Width * Height);

    unsigned char SumLength[1];
    fread(SumLength, sizeof(unsigned char), 1, OriFile);

    static unsigned char* SameSumByte = new unsigned char[SumLength[0]];
    int SameSum;
    unsigned char SameData[1];
    int DataIndex = 0;
    while(fread(SameSumByte, sizeof(unsigned char), SumLength[0], OriFile) == SumLength[0])
    {
        SameSum = UChar_to_Int(SameSumByte, SumLength[0]);

        fread(SameData, sizeof(unsigned char), 1, OriFile);
        for(int i = 0; i < SameSum; ++i)
        {
            NewImageData[DataIndex++] = SameData[0];
        }
    }

    QImage* NewImage = new QImage(NewImageData, Width, Height, QImage::Format_Grayscale8);
    return NewImage;
}
