#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QMouseEvent>

template<typename ClassType>
class ImageLabel : public QLabel
{
public:
    explicit ImageLabel(ClassType* Instances, void (ClassType::*Func)(int, int) = nullptr, QWidget *parent = nullptr);
    ~ImageLabel();


private:
    int MousePos_x; //鼠标位置x坐标
    int MousePos_y; //鼠标位置y坐标
    ClassType* ClassInstances;  //鼠标在Label上移动时希望调用的函数所在类的实例
    void (ClassType::*MouseMoveFunc)(int, int); //鼠标在Label上移动时希望调用的函数

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
};

template<typename ClassType>
ImageLabel<ClassType>::ImageLabel(ClassType* Instances, void (ClassType::*Func)(int, int), QWidget *parent) :
    QLabel(parent),
    ClassInstances(Instances),
    MouseMoveFunc(Func)
{

}

template<typename ClassType>
ImageLabel<ClassType>::~ImageLabel()
{

}

template<typename ClassType>
void ImageLabel<ClassType>::mouseMoveEvent(QMouseEvent* Event)
{
    int MoveDis_x = Event ->pos().x() - MousePos_x;
    int MoveDis_y = Event ->pos().y() - MousePos_y;

    (ClassInstances ->*MouseMoveFunc)(MoveDis_x, MoveDis_y);

    MousePos_x = Event ->pos().x();
    MousePos_y = Event ->pos().y();
}

template<typename ClassType>
void ImageLabel<ClassType>::mousePressEvent(QMouseEvent *Event)
{
    MousePos_x = Event ->pos().x();
    MousePos_y = Event ->pos().y();
}

#endif // IMAGELABEL_H
