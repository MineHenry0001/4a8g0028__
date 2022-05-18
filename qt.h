﻿#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qt.h"
#include <QFileDialog>
#include <QMainWindow>
#include <opencv2\opencv.hpp>  
#include <opencv2\imgproc\imgproc.hpp>  
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


#include <QWidget>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

class qt : public QMainWindow
{
    Q_OBJECT

public:
    qt(QWidget *parent = Q_NULLPTR);
    QImage MatImageToQt(const cv::Mat& src);

private:
    Ui::qtClass ui;
    QString filename;

    
    cv::Mat image;
    cv::VideoCapture m_videoCapture;
    QTimer* m_timer;
   
    

private slots:
   
    void open(void);
    void ROI(void);
    void HIST(void);
    void Thresholding(void);
    void Color(void);
    void GLOW(void);
    void GHIGH(void);
    void Matlab(void);
    void Median(void);
    void Laplacian(void);
    void Bilateral(void);
    void Sobel(void);
    void Rotate(void);
    void Pan(void);
    void affine_pan(void);
    void Mouse_Pressed();
    void contours();
    void readFarme();
    void on_openCamera_clicked();
    void on_closeCamera_clicked();

};


