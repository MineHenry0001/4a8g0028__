#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class MyQLabel : public QLabel
{
	Q_OBJECT
public:
	explicit MyQLabel(QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    int x, y;
    QString a, b;
    
signals:    
    void Mouse_Pressed();
};

