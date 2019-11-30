#ifndef SHARPENINGOPTION_H
#define SHARPENINGOPTION_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QtCore>

class MainWindow;

namespace Ui {
class SharpeningOption;
}

class SharpeningOption : public QDialog
{
    Q_OBJECT

public:
    explicit SharpeningOption(void (MainWindow::*ACF)(int (*GradProceser)
                                                     (unsigned char OriData, int Grad,unsigned char* L, int Threshold),
                              unsigned char* List_L, int Threshold, int FilterNo),
                              MainWindow* TMW, QWidget *parent = nullptr);
    ~SharpeningOption();

private:
    Ui::SharpeningOption *ui;

    static const int Filter[9][9];

    void (MainWindow::*ACFunction)(int (*GradProceser)(unsigned char OriData, int Grad,unsigned char* L, int Threshold),
                                   unsigned char* List_L, int Threshold, int FilterNo);
    unsigned char* List_L;
    int Threshold;
    int FilterNo;
    MainWindow* ThisMainWindow;
    int (*GradProceser)(unsigned char OriData, int Grad,unsigned char* L, int Threshold);

    QComboBox* MethodBox;
    QComboBox* FilterBox;
    QLabel* MethodText;
    QLabel* FilterText;
    QLabel* ThresholdText;
    QLabel* LGText;
    QLabel* LBText;
    QLabel* FilterLabel1;
    QLabel* FilterLabel2;
    QLineEdit* ThresholdEdit;
    QLineEdit* LGEdit;
    QLineEdit* LBEdit;
    QPushButton* AcceptButton;
    QPushButton* RejectButton;

private:
    void InitializeUI();
    void SetFilterLabel(QLabel* FilterLabel, int FilterNo);

private slots:
    void MethodBoxIndexChange();
    void FilterBoxIndexChange();
    void Accept();
    void Reject();
};



#endif // SHARPENINGOPTION_H
