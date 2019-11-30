#include "templateChart.h"
#include "ui_templateChart.h"

const int TemplateChart::BlockWidth = 40;
const int TemplateChart::BlockHeight = 25;
const int TemplateChart::TemplateMaxSum = 20;
int TemplateChart::OriTemplateSum = 1;
int* TemplateChart::OriTemplateWidthList = new int[TemplateMaxSum]{5};
int* TemplateChart::OriTemplateHeighList = new int[TemplateMaxSum]{5};
int*** TemplateChart::OriTemplateDataList = new int**[TemplateMaxSum]
{
    new int*[5]
    {
        new int[5]{ 0,  0, -1,  0,  0},
        new int[5]{ 0, -1, -2, -1,  0},
        new int[5]{-1, -2, 16, -2, -1},
        new int[5]{ 0, -1, -2, -1,  0},
        new int[5]{ 0,  0, -1,  0,  0},
    }
};
QString* TemplateChart::OriTemplateNameList = new QString[TemplateMaxSum]{"Laplace(5×5)"};

TemplateChart::TemplateChart(void (MainWindow::*ACF)(int TemplateWidth, int TemplateHeight, int** TemplateData),
                             MainWindow* TMW, int TW, int TH, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemplateChart),
    TemplateWidth(TW),
    TemplateHeight(TH),
    ACFunction(ACF),
    ThisMainWindow(TMW)
{
    ui->setupUi(this);
    InitializeUI();
}

TemplateChart::~TemplateChart()
{
    delete ui;
}

void TemplateChart::InitializeUI()
{
    int WindowWidth = TemplateWidth > 8 ? BlockWidth * 2 + TemplateWidth * BlockWidth : 10 * BlockWidth;

    setWindowTitle(QString("自定义模板：%1×%2").arg(TemplateHeight).arg(TemplateWidth));
    setFixedSize(WindowWidth, BlockHeight * 4 + BlockHeight + TemplateHeight * BlockHeight);
    setFont(QFont("Microsoft YaHei", 10));

    QRegExp regx("^-?([1-9]?[0-9]?)$");

    OriTemplateBox = new QComboBox(this);
    SaveTemplateButton = new QPushButton(this);
    SetTemplateButton = new QPushButton(this);
    AcceptButton = new QPushButton(this);
    RejectButton = new QPushButton(this);

    BlockList = new QLineEdit**[TemplateHeight];
    for(int i = 0; i < TemplateHeight; ++i)
    {
        BlockList[i] = new QLineEdit*[TemplateWidth];
        for(int j = 0; j < TemplateWidth; ++j)
        {
            BlockList[i][j] = new QLineEdit(this);
            BlockList[i][j] ->setGeometry(BlockWidth + j * BlockWidth, BlockHeight + i * BlockHeight, BlockWidth, BlockHeight);
            BlockList[i][j] ->setAlignment(Qt::AlignCenter);
            BlockList[i][j] ->setValidator(new QRegExpValidator(regx, BlockList[i][j]));
        }
    }
    BlockList[TemplateHeight / 2][TemplateWidth / 2] ->setStyleSheet("background-color:rgba(255,0,0,128)");

    AxisList_x = new QLabel*[TemplateWidth];
    for(int i = 0; i < TemplateWidth; ++i)
    {
        AxisList_x[i] = new QLabel(this);
        AxisList_x[i] ->setGeometry(BlockWidth + i * BlockWidth, BlockHeight - BlockHeight, BlockWidth, BlockHeight);
        AxisList_x[i] ->setStyleSheet("font-weight: bold;");
        AxisList_x[i] ->setAlignment(Qt::AlignCenter);
        AxisList_x[i] ->setText(QString::number(i - TemplateWidth / 2));
    }
    AxisList_y = new QLabel*[TemplateHeight];
    for(int i = 0; i < TemplateHeight; ++i)
    {
        AxisList_y[i] = new QLabel(this);
        AxisList_y[i] ->setGeometry(0, BlockHeight + i * BlockHeight, BlockWidth, BlockHeight);
        AxisList_y[i] ->setStyleSheet("font-weight: bold;");
        AxisList_y[i] ->setAlignment(Qt::AlignCenter);
        AxisList_y[i] ->setText(QString::number(i - TemplateHeight / 2));
    }

    OriTemplateBox ->setGeometry((WindowWidth) / 2 - 4 * BlockWidth - BlockWidth / 2,
                                  TemplateHeight * BlockHeight + BlockHeight + BlockHeight / 3,
                                  BlockWidth * 5, BlockHeight + BlockHeight / 2);
    OriTemplateBox ->setFont(QFont("Microsoft YaHei", 12));
    for(int i = 0; i < OriTemplateSum; ++i)
    {
        OriTemplateBox ->addItem(OriTemplateNameList[i]);
    }

    SetTemplateButton ->setGeometry((WindowWidth) / 2 + BlockWidth / 2 + BlockWidth / 4, TemplateHeight * BlockHeight + BlockHeight + BlockHeight / 3,
                                     BlockWidth * 2, BlockHeight + BlockHeight / 2);
    SetTemplateButton ->setFont(QFont("Microsoft YaHei", 12));
    SetTemplateButton ->setText("设置");
    QObject::connect(SetTemplateButton, SIGNAL(clicked()), this ,SLOT(SetTemplate()));
    SaveTemplateButton ->setGeometry((WindowWidth) / 2 + BlockWidth * 3 - BlockWidth / 5,
                                      TemplateHeight * BlockHeight + BlockHeight + BlockHeight / 3,
                                      BlockWidth * 2, BlockHeight + BlockHeight / 2);
    SaveTemplateButton ->setFont(QFont("Microsoft YaHei", 12));
    SaveTemplateButton ->setText("保存");
    QObject::connect(SaveTemplateButton, SIGNAL(clicked()), this ,SLOT(SaveTemplate()));
    AcceptButton ->setGeometry((WindowWidth) / 2 + BlockWidth, TemplateHeight * BlockHeight + BlockHeight * 3 + BlockHeight / 3,
                                BlockWidth * 3, BlockHeight + BlockHeight / 2);
    AcceptButton ->setFont(QFont("Microsoft YaHei", 12));
    AcceptButton ->setText("确认");
    QObject::connect(AcceptButton, SIGNAL(clicked()), this ,SLOT(Accept()));
    RejectButton ->setGeometry((WindowWidth) / 2 - 4 * BlockWidth, TemplateHeight * BlockHeight + BlockHeight * 3 + BlockHeight / 3,
                                BlockWidth * 3, BlockHeight + BlockHeight / 2);
    RejectButton ->setFont(QFont("Microsoft YaHei", 12));
    RejectButton ->setText("取消");
    QObject::connect(RejectButton, SIGNAL(clicked()), this ,SLOT(Reject()));
}

void TemplateChart::SetNewTemplate(int NewTemplateWidth, int NewTemplateHeight, int **NewTemplateData)
{
    int OriStart_x = 0, OriStart_y = 0, OriEnd_x = TemplateWidth, OriEnd_y = TemplateHeight;
    int NewStart_x = 0, NewStart_y = 0, NewEnd_x = NewTemplateWidth, NewEnd_y = NewTemplateHeight;

    if(NewTemplateWidth > TemplateWidth || NewTemplateHeight > TemplateHeight)
    {
        QMessageBox::warning(nullptr, "尺寸不匹配！", "当前所选模板宽度或高度过大，程序将截取模板中间部分填入！");
        NewStart_x = (NewTemplateWidth - TemplateWidth + 1 - NewTemplateWidth % 2) / 2;
        NewStart_y = (NewTemplateHeight - TemplateHeight + 1 - NewTemplateHeight % 2) / 2;
        NewEnd_x = NewTemplateWidth - (NewTemplateWidth - TemplateWidth + NewTemplateWidth % 2) / 2;
        NewEnd_y = NewTemplateHeight - (NewTemplateHeight - TemplateHeight + NewTemplateHeight % 2) / 2;
    }
    else if(NewTemplateWidth < TemplateWidth || NewTemplateHeight < TemplateHeight)
    {
        QMessageBox::warning(nullptr, "尺寸不匹配！", "当前所选模板宽度或高度过小，程序将把模板填入中间！");
        OriStart_x = (TemplateWidth - NewTemplateWidth + NewTemplateWidth % 2) / 2;
        OriStart_y = (TemplateHeight - NewTemplateHeight + NewTemplateHeight % 2) / 2;
        OriEnd_x = TemplateWidth - (TemplateWidth - NewTemplateWidth + 1 - NewTemplateWidth % 2) / 2;
        OriEnd_y = TemplateHeight - (TemplateHeight - NewTemplateHeight + 1 - NewTemplateHeight % 2) / 2;
    }

    for(int i = OriStart_y, k = NewStart_y; i < OriEnd_y; ++i, ++k)
    {
        for(int j = OriStart_x, l = NewStart_x; j < OriEnd_y; ++j, ++l)
        {
            BlockList[i][j] ->setText(QString::number(NewTemplateData[k][l]));
        }
    }
}

void TemplateChart::SetTemplate()
{
    if(OriTemplateSum)
    {
        int Index = OriTemplateBox ->currentIndex();
        SetNewTemplate(OriTemplateWidthList[Index], OriTemplateHeighList[Index], OriTemplateDataList[Index]);
    }
    else
    {
        QMessageBox::warning(nullptr, "模板列表为空！", "模板列表为空，无法设置！");
    }
}

void TemplateChart::SaveTemplate()
{
    bool EmptyFlag = false;
    OriTemplateNameList[OriTemplateSum] = QInputDialog::getText(nullptr, "模板名", "请输入新模板名", QLineEdit::Normal,
                                                                QString().sprintf("Template_%d", OriTemplateSum)) +
                                          "(" + QString::number(TemplateWidth) + "×" + QString::number(TemplateHeight) + ")";

    if(!OriTemplateNameList[OriTemplateSum].isEmpty())
    {
        OriTemplateWidthList[OriTemplateSum] = TemplateWidth;
        OriTemplateHeighList[OriTemplateSum] = TemplateHeight;
        OriTemplateDataList[OriTemplateSum] = new int*[TemplateHeight];
        for(int i = 0; i < TemplateHeight; ++i)
        {
            OriTemplateDataList[OriTemplateSum][i] = new int[TemplateWidth];
            for(int j = 0; j < TemplateWidth; ++j)
            {
                if(BlockList[i][j] ->text().isEmpty())
                {
                    EmptyFlag = true;
                    BlockList[i][j] ->setText(QString::number(0));
                }
                OriTemplateDataList[OriTemplateSum][i][j] = BlockList[i][j] ->text().toInt();
            }
        }
        OriTemplateBox ->addItem(OriTemplateNameList[OriTemplateSum]);
        ++OriTemplateSum;
    }
    if(EmptyFlag)
    {
        QMessageBox::warning(nullptr, "参数不足！", "存在空白单元格，程序将自动补零！");
    }
}

void TemplateChart::Accept()
{
    int** TemplateData = new int*[TemplateHeight];
    bool EmptyFlag = false;
    for(int i = 0; i < TemplateHeight; ++i)
    {
        TemplateData[i] = new int[TemplateWidth];
        for(int j = 0; j < TemplateWidth; ++j)
        {
            if(BlockList[i][j] ->text().isEmpty())
            {
                EmptyFlag = true;
                BlockList[i][j] ->setText(QString::number(0));
            }
            TemplateData[i][j] = BlockList[i][j] ->text().toInt();
        }
    }
    if(EmptyFlag)
    {
        QMessageBox::warning(nullptr, "参数不足！", "存在空白单元格，程序将自动补零！");
    }

    (ThisMainWindow ->*ACFunction)(TemplateWidth, TemplateHeight, TemplateData);
    delete this;
}

void TemplateChart::Reject()
{
    delete this;
}
