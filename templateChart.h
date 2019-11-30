#ifndef TEMPLATECHART_H
#define TEMPLATECHART_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>

class MainWindow;

namespace Ui {
class TemplateChart;
}

class TemplateChart : public QDialog
{
    Q_OBJECT

public:
    explicit TemplateChart(void (MainWindow::*ACF)(int TemplateWidth, int TemplateHeight, int** TemplateData),
                           MainWindow* TMW,int TW, int TH, QWidget *parent = nullptr);
    ~TemplateChart();

private:
    Ui::TemplateChart *ui;

    static const int BlockWidth;
    static const int BlockHeight;

    static const int TemplateMaxSum;
    static int OriTemplateSum;
    static int* OriTemplateWidthList;
    static int* OriTemplateHeighList;
    static int* (**OriTemplateDataList);
    static QString* OriTemplateNameList;

    int TemplateWidth;
    int TemplateHeight;

    QLineEdit** (*BlockList);
    QLabel* (*AxisList_x);
    QLabel* (*AxisList_y);

    QComboBox* OriTemplateBox;

    QPushButton* SetTemplateButton;
    QPushButton* SaveTemplateButton;
    QPushButton* AcceptButton;
    QPushButton* RejectButton;

    void (MainWindow::*ACFunction)(int TemplateWidth, int TemplateHeight, int** TemplateData);
    MainWindow* ThisMainWindow;

private:
    void InitializeUI();
    void SetNewTemplate(int NewTemplateWidth, int NewTemplateHeight, int** NewTemplateData);

private slots:
    void SetTemplate();
    void SaveTemplate();
    void Accept();
    void Reject();
};

#endif // TEMPLATECHART_H
