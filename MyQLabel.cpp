#include "MyQLabel.h"


MyQLabel::MyQLabel(QWidget* parent) :QLabel(parent)
{

}

void MyQLabel::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        a = QString::number(ev->x());
        b = QString::number(ev->y());
        emit Mouse_Pressed();
    }
}


