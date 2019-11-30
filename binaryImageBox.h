#ifndef BINARYIMAGEBOX_H
#define BINARYIMAGEBOX_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>

namespace Ui {
class BinaryImageBox;
}

class BinaryImageBox : public QMainWindow
{
    Q_OBJECT

public:
    QLabel* ImageLabel[8];

public:
    explicit BinaryImageBox(QWidget *parent = nullptr);
    ~BinaryImageBox();

private slots:
    void on_ExportJPG_clicked();

private:
    Ui::BinaryImageBox *ui;
};

#endif // BINARYIMAGEBOX_H
