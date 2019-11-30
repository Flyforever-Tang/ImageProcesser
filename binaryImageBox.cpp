#include "binaryImageBox.h"
#include "ui_binaryImageBox.h"

BinaryImageBox::BinaryImageBox(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BinaryImageBox)
{
    ui->setupUi(this);

    for(int i = 0; i < 4; ++i)
    {
        ImageLabel[i] = new QLabel(this);
        ImageLabel[i] ->setGeometry(26 + i * 207, 20, 192, 192);
    }
    for (int i = 0; i < 4; ++i)
    {
        ImageLabel[i + 4] = new QLabel(this);
        ImageLabel[i + 4] ->setGeometry(26 + i * 207, 227, 192, 192);
    }
}

BinaryImageBox::~BinaryImageBox()
{
    delete ui;
}

void BinaryImageBox::on_ExportJPG_clicked()
{
    QString SavePath = QFileDialog::getSaveFileName(this, tr("选择文件"), "D:/", tr("Image (*.jpg)"));
    QString PrePath = SavePath.left(SavePath.length() - 4); //去除后缀名

    for(int i = 0; i < 8; ++i)
    {
        QString No(QString::number(i)), ImagePath;
        ImagePath = PrePath + "_B" + No + ".jpg";

        QImage Image = ImageLabel[i] ->pixmap() ->toImage();
        Image.save(ImagePath, "JPG", 100);
    }
}
