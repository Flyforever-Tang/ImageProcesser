#include "sharpeningOption.h"
#include "ui_sharpeningOption.h"

#include "grayImageProcesser.h"

const int SharpeningOption::Filter[9][9] =
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

SharpeningOption::SharpeningOption(void (MainWindow::*ACF)(int (*GradProceser)
                                                          (unsigned char OriData, int Grad,unsigned char* L, int Threshold),
                                   unsigned char* List_L, int Threshold, int FilterNo),
                                   MainWindow* TMW, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SharpeningOption),
    ACFunction(ACF),
    ThisMainWindow(TMW)
{
    ui->setupUi(this);
    InitializeUI();
}

SharpeningOption::~SharpeningOption()
{
    delete ui;
}

void SharpeningOption::InitializeUI()
{

    setWindowTitle(tr("锐化选项"));
    setFont(QFont("Microsoft YaHei", 10));

    MethodBox = new QComboBox(this);
    FilterBox = new QComboBox(this);
    MethodText = new QLabel(this);
    FilterText = new QLabel(this);
    ThresholdText = new QLabel(this);
    LGText = new QLabel(this);
    LBText = new QLabel(this);
    FilterLabel1 = new QLabel(this);
    FilterLabel2 = new QLabel(this);
    ThresholdEdit = new QLineEdit(this);
    LGEdit = new QLineEdit(this);
    LBEdit = new QLineEdit(this);
    AcceptButton = new QPushButton(this);
    RejectButton = new QPushButton(this);

    MethodBox ->setGeometry(135, 10, 270, 30);
    MethodBox ->addItems(QStringList() << "直接使用梯度值" << "原像素点值<阈值>梯度值" << "原像素点值<阈值>固定灰度LG"
                         << "固定灰度LB<阈值>原像素点值" << "固定灰度LB<阈值>固定灰度LG");
    QObject::connect(MethodBox, SIGNAL(currentIndexChanged(int)), this ,SLOT(MethodBoxIndexChange()));
    FilterBox ->setGeometry(135, 50, 170, 30);
    FilterBox ->addItems(QStringList() << "Laplace模板" << "Laplace增强模板" << "高通模板1"
                         << "高通模板2" << "高通模板3" << "Sobel模板" << "Prewitt模板");
    QObject::connect(FilterBox, SIGNAL(currentIndexChanged(int)), this ,SLOT(FilterBoxIndexChange()));

    MethodText ->setGeometry(20, 10, 110, 30);
    MethodText ->setStyleSheet("font-weight: bold;");
    MethodText ->setAlignment(Qt::AlignCenter);
    MethodText ->setText("梯度处理方法");
    FilterText ->setGeometry(20, 50, 110, 30);
    FilterText ->setStyleSheet("font-weight: bold;");
    FilterText ->setAlignment(Qt::AlignCenter);
    FilterText ->setText("锐化模板");
    ThresholdText ->setGeometry(20, 90, 110, 30);
    ThresholdText ->setStyleSheet("font-weight: bold;");
    ThresholdText ->setAlignment(Qt::AlignCenter);
    ThresholdText ->setText("阈值");
    LGText ->setGeometry(20, 130, 110, 30);
    LGText ->setStyleSheet("font-weight: bold;");
    LGText ->setAlignment(Qt::AlignCenter);
    LGText ->setText("固定灰度LG");
    LBText ->setGeometry(20, 170, 110, 30);
    LBText ->setStyleSheet("font-weight: bold;");
    LBText ->setAlignment(Qt::AlignCenter);
    LBText ->setText("固定灰度LB");

    FilterLabel1 ->setGeometry(250, 95, 100, 100);
    FilterLabel1 ->setStyleSheet("border:2px solid #DEDEDE;");
    FilterLabel1 ->setFont(QFont("Microsoft YaHei", 12));
    SetFilterLabel(FilterLabel1, 0);
    FilterLabel2 ->setGeometry(310, 95, 100, 100);
    FilterLabel2 ->setStyleSheet("border:1px solid #DEDEDE;");
    FilterLabel2 ->setFont(QFont("Microsoft YaHei", 12));
    FilterLabel2 ->setVisible(false);

    QRegExp regx("^(0|1[0-9]?[0-9]?|2[0-4][0-9]|25[0-5]|[2-9]?[0-9]?)$");
    ThresholdEdit ->setGeometry(135, 90, 40, 30);
    ThresholdEdit ->setValidator(new QRegExpValidator(regx, ThresholdEdit));
    ThresholdEdit ->setEnabled(false);
    LGEdit ->setGeometry(135, 130, 40, 30);
    LGEdit ->setValidator(new QRegExpValidator(regx, LGEdit));
    LGEdit ->setEnabled(false);
    LBEdit ->setGeometry(135, 170, 40, 30);
    LBEdit ->setValidator(new QRegExpValidator(regx, LBEdit));
    LBEdit ->setEnabled(false);

    AcceptButton ->setGeometry(230, 205, 120, 40);
    AcceptButton ->setFont(QFont("Microsoft YaHei", 12));
    AcceptButton ->setText("确认");
    QObject::connect(AcceptButton, SIGNAL(clicked()), this ,SLOT(Accept()));
    RejectButton ->setGeometry(80, 205, 120, 40);
    RejectButton ->setFont(QFont("Microsoft YaHei", 12));
    RejectButton ->setText("取消");
    QObject::connect(RejectButton, SIGNAL(clicked()), this ,SLOT(Reject()));
}

void SharpeningOption::SetFilterLabel(QLabel* FilterLabel, int FilterNo)
{
    QString FilterText;
    FilterText.sprintf("%3d%3d%3d\n%3d%3d%3d\n%3d%3d%3d", Filter[FilterNo][0], Filter[FilterNo][1], Filter[FilterNo][2],
                                                          Filter[FilterNo][3], Filter[FilterNo][4], Filter[FilterNo][5],
                                                          Filter[FilterNo][6], Filter[FilterNo][7], Filter[FilterNo][8]);
    FilterLabel ->setText(FilterText);
}

void SharpeningOption::MethodBoxIndexChange()
{
    switch(MethodBox ->currentIndex())
    {
    case 0:
    {
        ThresholdEdit ->setEnabled(false);
        LGEdit ->setEnabled(false);
        LBEdit ->setEnabled(false);
        break;
    }
    case 1:
    {
        ThresholdEdit ->setEnabled(true);
        LGEdit ->setEnabled(false);
        LBEdit ->setEnabled(false);
        break;
    }
    case 2:
    {
        ThresholdEdit ->setEnabled(true);
        LGEdit ->setEnabled(true);
        LBEdit ->setEnabled(false);
        break;
    }
    case 3:
    {
        ThresholdEdit ->setEnabled(true);
        LGEdit ->setEnabled(false);
        LBEdit ->setEnabled(true);
        break;
    }
    case 4:
    {
        ThresholdEdit ->setEnabled(true);
        LGEdit ->setEnabled(true);
        LBEdit ->setEnabled(true);
        break;
    }
    }
}

void SharpeningOption::FilterBoxIndexChange()
{
    int Index = FilterBox ->currentIndex();

    SetFilterLabel(FilterLabel1, Index);
    if(Index > 4)
    {
        SetFilterLabel(FilterLabel2, Index + 2);
        FilterLabel1 ->setGeometry(200, 95, 100, 100);
        FilterLabel2 ->setVisible(true);
    }
    else
    {
        FilterLabel1 ->setGeometry(250, 95, 100, 100);
        FilterLabel2 ->setVisible(false);
    }
}

void SharpeningOption::Accept()
{
    Threshold = ThresholdEdit ->text().toInt();

    switch(MethodBox ->currentIndex())
    {
    case 0:
    {
        GradProceser = &GrayImageProcesser::GradProceser_Grad;
        List_L = nullptr;
        Threshold = -1;
        FilterNo = FilterBox ->currentIndex();
        break;
    }
    case 1:
    {
        if(ThresholdText ->text().isEmpty())
        {
            QMessageBox::warning(nullptr, "参数错误！", "请输入所有必填参数！");
            return;
        }

        GradProceser = &GrayImageProcesser::GradProceser_Ori_Grad;
        List_L = nullptr;
        ThresholdText ->text().toInt();
        FilterNo = FilterBox ->currentIndex();
        break;
    }
    case 2:
    {
        if(ThresholdText ->text().isEmpty() || LGEdit ->text().isEmpty())
        {
            QMessageBox::warning(nullptr, "参数错误！", "请输入所有必填参数！");
            return;
        }

        GradProceser = &GrayImageProcesser::GradProceser_Ori_LG;
        List_L = new unsigned char(static_cast<unsigned char>(LGEdit ->text().toInt()));
        ThresholdText ->text().toInt();
        FilterNo = FilterBox ->currentIndex();
        break;
    }
    case 3:
    {
        if(ThresholdText ->text().isEmpty() || LBEdit ->text().isEmpty())
        {
            QMessageBox::warning(nullptr, "参数错误！", "请输入所有必填参数！");
            return;
        }

        GradProceser = &GrayImageProcesser::GradProceser_LB_Grad;
        List_L = new unsigned char(static_cast<unsigned char>(LBEdit ->text().toInt()));
        ThresholdText ->text().toInt();
        FilterNo = FilterBox ->currentIndex();
        break;
    }
    case 4:
    {
        if(ThresholdText ->text().isEmpty() || LBEdit ->text().isEmpty() || LGEdit ->text().isEmpty())
        {
            QMessageBox::warning(nullptr, "参数错误！", "请输入所有必填参数！");
            return;
        }

        GradProceser = &GrayImageProcesser::GradProceser_LB_LG;
        List_L = new unsigned char[2]{static_cast<unsigned char>(LBEdit ->text().toInt()),
                                      static_cast<unsigned char>(LGEdit ->text().toInt())};
        ThresholdText ->text().toInt();
        FilterNo = FilterBox ->currentIndex();
        break;
    }
    }

    (ThisMainWindow ->*ACFunction)(GradProceser, List_L, Threshold, FilterNo);
    delete this;
}

void SharpeningOption::Reject()
{
    delete this;
}
