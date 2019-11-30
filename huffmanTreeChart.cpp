#include "huffmanTreeChart.h"
#include "ui_huffmanTreeChart.h"

const int HuffmanTreeChart::NodeRadius = 30;

HuffmanTreeChart::HuffmanTreeChart(Node* HTR, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HuffmanTreeChart),
    HuffmanTreeRoot(HTR),
    BiggestDepth(0)
{
    ui->setupUi(this);

    InitializeUI();
    CreateTree();
}

HuffmanTreeChart::~HuffmanTreeChart()
{
    delete ui;
}

void HuffmanTreeChart::InitializeUI()
{
    setWindowTitle("Huffman树");
    setFixedSize(850, 850);

    HuffmanTreeArea = new QScrollArea(this);
    HuffmanTreeWidget = new QWidget(HuffmanTreeArea);

    HuffmanTreeArea ->setGeometry(25, 25, 800, 800);
}

void HuffmanTreeChart::GetBiggestDepth_DFS(Node *SearchIndex, int CurrentDepth)
{
    if(CurrentDepth > BiggestDepth)
    {
        BiggestDepth = CurrentDepth;
    }

    if(SearchIndex ->Left && SearchIndex ->Right)
    {
        GetBiggestDepth_DFS(SearchIndex ->Left, CurrentDepth + 1);
        GetBiggestDepth_DFS(SearchIndex ->Right, CurrentDepth + 1);
    }
}

void HuffmanTreeChart::CreateNode_DFS(Node* SearchIndex, int Center_x, int Center_y, int CurrentInterval)
{
    QCircularLabel* ThisNode = new QCircularLabel(NodeRadius, HuffmanTreeWidget);
    ThisNode ->setGeometry(Center_x - NodeRadius, Center_y - NodeRadius, NodeRadius * 2, NodeRadius * 2);
    ThisNode ->setAlignment(Qt::AlignCenter);
    ThisNode ->setText(QString::number(SearchIndex ->SortValue));
    if(SearchIndex ->Left && SearchIndex ->Right)
    {
        QLabel* CodeLabel;

        CodeLabel = new QLabel(HuffmanTreeWidget);
        CodeLabel ->setGeometry(Center_x - CurrentInterval / 2,
                                Center_y + NodeRadius * 5 / 4, NodeRadius / 2, NodeRadius / 2);
        CodeLabel ->setAlignment(Qt::AlignCenter);
        CodeLabel ->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
        CodeLabel ->setText(QString::number(0));
        CreateNode_DFS(SearchIndex ->Left, Center_x - CurrentInterval,
                       Center_y + NodeRadius * 3, CurrentInterval / 2);

        CodeLabel = new QLabel(HuffmanTreeWidget);
        CodeLabel ->setGeometry(Center_x + CurrentInterval / 2,
                                Center_y + NodeRadius * 5 / 4 , NodeRadius / 2, NodeRadius / 2);
        CodeLabel ->setAlignment(Qt::AlignCenter);
        CodeLabel ->setFont(QFont("Microsoft YaHei", 12, QFont::Bold));
        CodeLabel ->setText(QString::number(1));
        CreateNode_DFS(SearchIndex ->Right, Center_x + CurrentInterval,
                       Center_y + NodeRadius * 3, CurrentInterval / 2);
    }
    else
    {
        QLabel* ValueLabel = new QLabel(HuffmanTreeWidget);
        ValueLabel ->setGeometry(Center_x - NodeRadius * 3 / 4 + 5, Center_y + NodeRadius * 3 / 2, NodeRadius * 3 / 2, NodeRadius * 3 / 4);
        ValueLabel ->setAlignment(Qt::AlignCenter);
        ValueLabel ->setText(QString::number(SearchIndex ->OtherValue));
        ValueLabel ->setStyleSheet("border:2px solid");
    }
}

void HuffmanTreeChart::CreateTree()
{
    GetBiggestDepth_DFS(HuffmanTreeRoot, 1);
    int WidgetWidth = Pow_int(2, BiggestDepth) * NodeRadius * 2;
    HuffmanTreeWidget ->setFixedSize(WidgetWidth, BiggestDepth * NodeRadius * 3 + NodeRadius * 2);
    CreateNode_DFS(HuffmanTreeRoot, WidgetWidth / 2, NodeRadius * 2, WidgetWidth / 4);
    HuffmanTreeArea ->setWidget(HuffmanTreeWidget);

}
