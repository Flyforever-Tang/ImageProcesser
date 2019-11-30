#ifndef PROLIB_H
#define PROLIB_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <QString>

/*项目工具库*/

int RoundOff(int OriNum, double Scale); //区间（向下）映射

int* GetSuitableSize(int MaxWidth, int MaxHeight, int OriWidth, int OriHeight); //根据可用区域总大小计算填充矩形的合适大小

int Pow_int(int Base, int Power);   //整形幂运算

int UChar_to_Int(unsigned char String[], int Strleng);  //字符串转整形

bool* UChar_to_Byte(unsigned char TarUChar);    //无符号字符转8位比特

inline unsigned char Byte_to_UChar(bool* Byte)  //8位比特转无符号字符
{
    return (Byte[0] << 7) + (Byte[1] << 6) + (Byte[2] << 5) + (Byte[3] << 4) +
           (Byte[4] << 3) + (Byte[5] << 2) + (Byte[6] << 1) + Byte[7];
}

inline bool GetBit(int OriByte, int Place)    //获得二进制数中特定位的值（最低位为第0位）
{
    return (OriByte >> Place) - (OriByte >> (Place + 1) << 1);
}

inline char* QString_to_Str(QString OriQString)   //QString转字符串
{
    char* String = NULL;
    QByteArray ByteArray = OriQString.toLatin1();
    String = (char *)malloc(ByteArray.length() + 1);
    memset(String, 0, ByteArray.length());
    memcpy(String, ByteArray.data(), ByteArray.length());
    String[ByteArray.length()] = '\0';
    return String;
}

template <typename ValueType>   //获得两者中较大或较小的值（默认较大）
inline ValueType GetBigger_or_SmallerOne(ValueType Value1, ValueType Value2, bool GetBigger = true)
{
    return GetBigger ^ Value1 < Value2 ? Value1 : Value2;
}

template <typename ValueType>   //判断Value是否在Left与Right之间，默认闭区间
inline bool IsIn(ValueType Left, ValueType Right, ValueType Value, bool LeftFlag = true, bool RightFlag = true)
{
    return Value > Left - LeftFlag && Value < Right + RightFlag;
}

template <typename PointerType> //将指针数组置空（赋值为nullptr）
void MemsetPointer(PointerType* Pointer[], int Size)
{
    for(int i = 0; i < Size; ++i)
    {
        Pointer[i] = nullptr;
    }
}

template <typename ValueType>   //快排比较函数模板，从小到大排列
inline int Comp_Smaller(const void* a, const void* b)
{
    return *(ValueType*)a - *(ValueType*)b;
}

template <typename ValueType>   //快排比较函数模板，从大到小排列
inline int Comp_Bigger(const void* a, const void* b)
{
    return *(ValueType*)b - *(ValueType*)a;
}

template <typename ValueType>   //获取数组特定位置的值，超出边界则返回InvalidValue
ValueType GetArrayValue(const ValueType* ValueArray, int ArrayWidth, int ArrayHeight, int x, int y, ValueType InvalidValue)
{
    if(x < 0 || x > ArrayWidth - 1 || y < 0 || y > ArrayHeight - 1)
    {
        return InvalidValue;
    }

    return ValueArray[y * ArrayWidth + x];
}

template <typename ValueType>   //获取数组某些特定位置的值，超出边界则略去
ValueType* GetArrayValue(const ValueType* ValueArray, int ArrayWidth, int ArrayHeight, int x, int y,
                         const int (*PosArray)[2], int& PosArrayLength)
{
    ValueType* Result = new ValueType[PosArrayLength];

    int LengthBackup = PosArrayLength;
    for(int i = 0; i < LengthBackup; ++i)
    {
        int Pos_x = x + PosArray[i][0], Pos_y = y + PosArray[i][1];

        if(Pos_x < 0 || Pos_x > ArrayWidth - 1 || Pos_y < 0 || Pos_y > ArrayHeight - 1)
        {
            --PosArrayLength;
            continue;
        }

        Result[i + PosArrayLength - LengthBackup] = ValueArray[Pos_y * ArrayWidth + Pos_x];
    }

    return Result;
}

template <typename ValueType>   //最近邻插值
ValueType Interpolation_NearestNeighbor(const ValueType* ValueArray, int ArrayWidth, int ArrayHeight,
                                        double Tar_x, double Tar_y, int InvalidValue)
{
    int Re_x = Tar_x + 0.50001;
    int Re_y = Tar_y + 0.50001;
    return GetArrayValue<ValueType>(ValueArray, ArrayWidth, ArrayHeight, Re_x, Re_y, InvalidValue);
}

template <typename ValueType>   //双线性插值
ValueType Interpolation_Bilinear(const ValueType* ValueArray, int ArrayWidth, int ArrayHeight, double Tar_x, double Tar_y, int InvalidValue)
{
    int Floor_x = Tar_x + 0.00001;
    int Floor_y = Tar_y + 0.00001;
    double Interval_x = Tar_x - Floor_x;
    double Interval_y = Tar_y - Floor_y;

    ValueType Fun_0_0 = GetArrayValue<ValueType>(ValueArray, ArrayWidth, ArrayHeight, Floor_x, Floor_y, InvalidValue);
    ValueType Fun_0_1 = GetArrayValue<ValueType>(ValueArray, ArrayWidth, ArrayHeight, Floor_x, Floor_y + 1, InvalidValue);
    ValueType Fun_1_0 = GetArrayValue<ValueType>(ValueArray, ArrayWidth, ArrayHeight, Floor_x + 1, Floor_y, InvalidValue);
    ValueType Fun_1_1 = GetArrayValue<ValueType>(ValueArray, ArrayWidth, ArrayHeight, Floor_x + 1, Floor_y + 1, InvalidValue);

    int Re = (Fun_1_0 - Fun_0_0) * Interval_x + (Fun_0_1 - Fun_0_0) * Interval_y +
             (Fun_1_1 + Fun_0_0 - Fun_1_0 - Fun_0_1) * Interval_x * Interval_y + Fun_0_0;
    return Re;
}

template <typename ValueType>   //快排比较函数，从小到大排
int Comp(const void* a, const void* b)
{
    return *(ValueType*)a - *(ValueType*)b;
}

template <typename ValueType>   //计算中值
ValueType GetMedian(const ValueType* ValueArray, int ArrayLength)
{
    ValueType* ArrayBackup = new ValueType[ArrayLength];
    for(int i = 0; i < ArrayLength; ++i)
    {
        ArrayBackup[i] = ValueArray[i];
    }
    qsort(ArrayBackup, ArrayLength, sizeof(ValueType), Comp<ValueType>);
    if(ArrayLength % 2)
    {
        return ArrayBackup[ArrayLength / 2];
    }
    else
    {
        return (ArrayBackup[ArrayLength / 2 - 1] + ArrayBackup[ArrayLength / 2]) / 2;
    }
}

template <typename ValueType>   //计算平均值
double GetAverage(const ValueType* ValueArray, int ArrayLength)
{
    double Sum = 0;
    for(int i = 0; i < ArrayLength; ++i)
    {
        Sum += ValueArray[i];
    }
    return Sum / ArrayLength;
}

template <typename ValueType>   //计算加入新值后的平均值
inline double GetAverage(double OriAverage, ValueType NewValue, int OriLength)
{
    return OriAverage + (NewValue - OriAverage) / (OriLength + 1);
}

template <typename ValueType>   //计算方值
double GetVariance(const ValueType* ValueArray, int ArrayLength)
{
    double Average = GetAverage(ValueArray, ArrayLength);
    double Sum = 0;
    for(int i = 0; i < ArrayLength; ++i)
    {
        Sum += (ValueArray[i] - Average) * (ValueArray[i] - Average);
    }
    return Sum / ArrayLength;
}

template <typename ValueType>   //获得最值编号（默认最小值）
int GetMostValueNo(const ValueType* TarArray, int TarArrayLength, bool GetSmallest = true)
{
    ValueType Smallest = TarArray[0], SmallestNo = 0;
    for(int i = 1; i < TarArrayLength; ++i)
    {
        if(GetSmallest ^ (TarArray[i] > Smallest))
        {
            Smallest = TarArray[i];
            SmallestNo = i;
        }
    }
    return SmallestNo;
}

template <typename ValueType>   //判断数组中离Median最远的值是否比Challenger远。若是，返回最远值编号；否则返回-1
int NearerThanFarestOne(const ValueType* TarArray, int TarArrayLength, ValueType Challenger, ValueType Median)
{
    int Farest = -1, FarestNo = 0;
    int DValue_Cha, DValue_Tar;
    for(int i = 0; i < TarArrayLength; ++i)
    {
        DValue_Tar = fabs((double)TarArray[i] - Median) + 0.00001;
        if(DValue_Tar > Farest)
        {
            Farest = DValue_Tar;
            FarestNo = i;
        }
    }
    DValue_Cha = fabs((double)Challenger - Median) + 0.00001;
    if(DValue_Cha < Farest)
    {
        return FarestNo;
    }
    return -1;
}

template <typename ValueType>   //获取数组中Host的NeighborSum个最近邻（数值最接近的值）
ValueType* GetNeighbor(const ValueType* ValueArray, int ValueArrayLength, ValueType Host, int NeighborSum)
{
    ValueType* NeighborList = new ValueType[NeighborSum];

    if(NeighborSum > ValueArrayLength)
    {
        NeighborSum = ValueArrayLength;
    }

    for(int i = 0; i < ValueArrayLength; ++i)
    {
        if(i < NeighborSum)
        {
            NeighborList[i] = ValueArray[i];
            continue;
        }
        int FarestNeighborNo = NearerThanFarestOne<unsigned char>(NeighborList, NeighborSum, ValueArray[i], Host);
        if(FarestNeighborNo != -1)
        {
            NeighborList[FarestNeighborNo] = ValueArray[i];
        }
    }

    return NeighborList;
}

/*字节填充器，默认不补0*/
class ByteFillter
{
public:
    ByteFillter(bool* ByteList, int ListLength, unsigned char Container[], int& ContainerIndex, bool FillEndFlag = false);
};

/*结点*/
class Node
{
public:
    int SortValue;
    int OtherValue;
    Node* Left;
    Node* Right;

public:
    Node(int SV = -1, int OV = -1, Node* L = nullptr, Node* R = nullptr);

    Node* AddLeftNode(int SV = -1, int OV = -1);    //添加左结点（如果没有的话）并返回左结点
    Node* AddRightNode(int SV = -1, int OV = -1);   //添加右结点（如果没有的话）并返回右结点
};

/*Huffman树*/
class HuffmanTree
{
public:
    int RootSum;
    Node* (*RootList);
    Node* HuffmanRoot;
    bool* (*CodeList);
    int* CodeLengthList;

public:
    HuffmanTree(int NodeSum, int ValueArray[]); //编码用
    HuffmanTree(bool* (*HCL), int* HCLL, int LL);   //解码用

    void CreateTree();  //编码用
    void CreateTree(bool* (*HuffmanCodeList), int* HuffmanCodeLengthList, int ListSum); //解码用

    int Decode(bool* CodeList = nullptr);   //解码

private:
    void HuffmanDFS(Node* SearchIndex, bool* PreCodeList, int Deapth);  //DFS，根据Huffman树生成编码序列
    int GetSmallestNode();  //获得SortValue最小的结点编号
};

#endif // PROLIB_H
