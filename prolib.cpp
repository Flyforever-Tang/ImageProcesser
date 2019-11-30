#include "prolib.h"

int RoundOff(int OriNum, double Scale)
{
    for(int i = 1; i < 256; ++i)
    {
        int floor = Scale * i + 0.01;
        if(floor >= OriNum)
        {
            OriNum = Scale * (i - 1) + 0.01;
            return OriNum;
        }
    }
}

int* GetSuitableSize(int MaxWidth, int MaxHeight, int OriWidth, int OriHeight)
{
    int* SuitableSize = new int[2];    //SuitableSize[0]:宽; SuitableSize[1]:高
    float WidthRatio, HeightRatio;

    WidthRatio = OriWidth / (float)MaxWidth;
    HeightRatio = OriHeight / (float)MaxHeight;

    if(WidthRatio > HeightRatio)
    {
        SuitableSize[0] = OriWidth / WidthRatio;
        SuitableSize[1] = OriHeight / WidthRatio;
    }
    else
    {
        SuitableSize[0] = OriWidth / HeightRatio;
        SuitableSize[1] = OriHeight / HeightRatio;
    }

    return SuitableSize;
}

int Pow_int(int Base, int Power)
{
    int Result = 1;
    for(int i = 0; i < Power; ++i)
    {
        Result *= Base;
    }
    return Result;
}

int UChar_to_Int(unsigned char String[], int Strleng)
{
    int Result = 0;
    for(int i = 0; i < Strleng; ++i)
    {
        Result += String[i] << ((Strleng - i - 1) << 3);
    }
    return Result;
}

bool* UChar_to_Byte(unsigned char TarUChar)
{
    bool* Byte = new bool[8];
    for(int i = 0; i < 8; ++i)
    {
        Byte[7 - i] = (TarUChar & (1 << i)) >> i;
    }
    return Byte;
}

ByteFillter::ByteFillter(bool* BitsList, int ListLength, unsigned char Container[], int& ContainerIndex, bool FillEndFlag)
{
    static bool Byte[8];
    static int FilledIndex = 0;
    for(int i = 0; i < ListLength; ++i)
    {
        Byte[FilledIndex++] = BitsList[i];
        if(FilledIndex == 8)
        {
            Container[ContainerIndex++] = Byte_to_UChar(Byte);
            FilledIndex = 0;
        }
    }
    if(FillEndFlag && FilledIndex)
    {
        for(int i = FilledIndex; i < 8; ++i)
        {
            Byte[i] = 0;
        }
        Container[ContainerIndex++] = Byte_to_UChar(Byte);
        FilledIndex = 0;
    }
}

Node::Node(int SV, int OV, Node* L, Node* R) :
    SortValue(SV),
    OtherValue(OV),
    Left(L),
    Right(R)
{

}

Node* Node::AddLeftNode(int SV, int OV)
{
    if(Left)
    {
        return Left;
    }
    else
    {
        Left = new Node(SV, OV);
        return Left;
    }
}

Node* Node::AddRightNode(int SV, int OV)
{
    if(Right)
    {
        return Right;
    }
    else
    {
        Right = new Node(SV, OV);
        return Right;
    }
}

HuffmanTree::HuffmanTree(int NodeSum, int ValueArray[]):
    RootSum(NodeSum),
    RootList(new Node*[RootSum]),
    CodeList(new bool*[RootSum]),
    CodeLengthList(new int[RootSum])
{
    MemsetPointer(CodeList, RootSum);
    memset(CodeLengthList, 0, RootSum * sizeof(int));

    for(int i = 0; i < NodeSum; ++i)
    {
        if(ValueArray[i])
        {
            RootList[i + RootSum - NodeSum] = new Node(ValueArray[i], i);
        }
        else
        {
            --RootSum;
        }
    }

    CreateTree();
    HuffmanDFS(HuffmanRoot, nullptr, 0);
}

HuffmanTree::HuffmanTree(bool* (*HCL), int* HCLL, int LL)
{
    CreateTree(HCL, HCLL, LL);
}

void HuffmanTree::CreateTree()
{
    int SmallestNo = 0;
    for(int i = 0; i < RootSum - 1; ++i)
    {
        Node* NewLeft;
        Node* NewRight;
        SmallestNo = GetSmallestNode();
        NewLeft = RootList[SmallestNo];
        RootList[SmallestNo] = nullptr;
        SmallestNo = GetSmallestNode();
        NewRight = RootList[SmallestNo];
        RootList[SmallestNo] = new Node(NewLeft ->SortValue + NewRight ->SortValue, -1, NewLeft, NewRight);
    }
    HuffmanRoot = RootList[SmallestNo];
}

void HuffmanTree::CreateTree(bool *(*HuffmanCodeList), int *HuffmanCodeLengthList, int ListSum)
{
    HuffmanRoot = new Node;
    for(int i = 0; i < ListSum; ++i)
    {
        if(!HuffmanCodeLengthList[i])
        {
            continue;
        }
        Node* TreeIndex = HuffmanRoot;
        for(int j = 0; j < HuffmanCodeLengthList[i]; ++j)
        {
            if(HuffmanCodeList[i][j] == 0)
            {
                TreeIndex = TreeIndex ->AddLeftNode();
            }
            else
            {
                TreeIndex = TreeIndex ->AddRightNode();
            }
        }
        TreeIndex ->OtherValue = i;
    }
}

int HuffmanTree::Decode(bool *CodeList)
{
    static int ListIndex = 0;
    static Node* TreeIndex = HuffmanRoot;
    static bool* CurrentCodeList = nullptr;
    if(CodeList && !ListIndex && !CurrentCodeList)
    {
        CurrentCodeList = CodeList;
    }
    while(1)
    {
        if(TreeIndex ->OtherValue >= 0)
        {
            int DecodeValue = TreeIndex ->OtherValue;
            TreeIndex = HuffmanRoot;
            return DecodeValue;
        }
        else if(CurrentCodeList == nullptr || ListIndex == 8)
        {
            ListIndex = 0;
            delete CurrentCodeList;
            CurrentCodeList = nullptr;
            return -1;
        }
        else
        {
            TreeIndex = CurrentCodeList[ListIndex++] == 0 ? TreeIndex ->Left : TreeIndex ->Right;
        }
    }
}

void HuffmanTree::HuffmanDFS(Node* SearchIndex, bool* PreCodeList, int Deapth)
{
    bool* NewCodeList = new bool[Deapth + 1];
    for(int i = 0; i < Deapth; ++i)
    {
        NewCodeList[i] = PreCodeList[i];
    }
    if(!SearchIndex ->Left)
    {
        CodeList[SearchIndex ->OtherValue] = NewCodeList;
        CodeLengthList[SearchIndex ->OtherValue] = Deapth;
        return;
    }
    else
    {
        NewCodeList[Deapth] = 0;
        HuffmanDFS(SearchIndex ->Left, NewCodeList, Deapth + 1);
        NewCodeList[Deapth] = 1;
        HuffmanDFS(SearchIndex ->Right, NewCodeList, Deapth + 1);
        delete[] NewCodeList;
    }
}

int HuffmanTree::GetSmallestNode()
{
    int SmallestValue = 2147483647, SmallestNo = 0;
    for(int i = 0; i < RootSum; ++i)
    {
        if(RootList[i] && RootList[i] ->SortValue < SmallestValue)
        {
            SmallestValue = RootList[i] ->SortValue;
            SmallestNo = i;
        }
    }
    return SmallestNo;
}
