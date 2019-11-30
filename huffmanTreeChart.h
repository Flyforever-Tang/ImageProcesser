#ifndef HUFFMANTREECHART_H
#define HUFFMANTREECHART_H

#include "prolib.h"
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPainter>

namespace Ui {
class HuffmanTreeChart;
}

class HuffmanTreeChart : public QWidget
{
    Q_OBJECT

    class QCircularLabel : public QLabel
    {
    public:
        QCircularLabel(int Radius, QWidget* Parents): QLabel(Parents)
        {
           setStyleSheet(QString("min-width: %1px; min-height: %1px;max-width: %1px; max-height: %1px;"
                                 "border-radius: %2px; border: 2px solid black;").arg(Radius * 2).arg(Radius));
        }
        ~QCircularLabel(){}
    };

public:
    explicit HuffmanTreeChart(Node* HTR = nullptr, QWidget *parent = nullptr);
    ~HuffmanTreeChart();

private:
    Ui::HuffmanTreeChart *ui;

    static const int NodeRadius;

    Node* HuffmanTreeRoot;
    int BiggestDepth;

    QScrollArea* HuffmanTreeArea;
    QWidget* HuffmanTreeWidget;

private:
    void InitializeUI();

    void GetBiggestDepth_DFS(Node* SearchIndex, int CurrentDepth);
    void CreateNode_DFS(Node* SearchIndex, int Center_x, int Center_y, int CurrentInterval);
    void CreateTree();
};

#endif // HUFFMANTREECHART_H
