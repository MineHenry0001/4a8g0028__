#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qt.h"
#include <QFileDialog>
#include <QMainWindow>
#include <opencv2\opencv.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class qt : public QMainWindow
{
    Q_OBJECT

public:
    qt(QWidget *parent = Q_NULLPTR);

private:
    Ui::qtClass ui;
    QString filename;


private slots:
   
    void open(void);
    void ROI(void);
    void HIST(void);
    
};


