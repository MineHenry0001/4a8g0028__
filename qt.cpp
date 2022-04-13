#include "qt.h"
#include <qmessagebox.h>
#include <fstream>
#include "qmainwindow.h"
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp> // import no include errors
#include <opencv2/imgproc/imgproc.hpp> // import no include errors 
#include <opencv2/core/core.hpp>       // import no include errors


using namespace std;
using namespace cv;

//QMessageBox::warning(NULL, "Y", "YUYUY");

qt::qt(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void qt::open()
{
    filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.png *.jpg *,jpeg *.bmp *.gif"));

    
    if(QString::compare(filename,QString())!=0)
    {
        QImage image;
        bool valid = image.load(filename);

        if (valid)
        {
            image = image.scaledToWidth(ui.a_image->width(), Qt::SmoothTransformation);
            ui.a_image->setPixmap(QPixmap::fromImage(image));

        }
        else
        {
            //error handing
        }
    }
    ui.X_Slider->setMinimum(0);
    ui.X_Slider->setMaximum(ui.a_image->height());
    ui.X_Slider->setSingleStep(1);

    ui.Y_Slider->setMinimum(0);
    ui.Y_Slider->setMaximum(ui.a_image->width());
    ui.Y_Slider->setSingleStep(1);

    ui.W_Slider->setMinimum(0);
    ui.W_Slider->setMaximum(ui.a_image->width());
    ui.W_Slider->setSingleStep(1);

    ui.H_Slider->setMinimum(0);
    ui.H_Slider->setMaximum(ui.a_image->height());
    ui.W_Slider->setSingleStep(1);

    ui.X_Spin->setMinimum(0);
    ui.X_Spin->setMaximum(ui.a_image->height());

    ui.Y_Spin->setMinimum(0);
    ui.Y_Spin->setMaximum(ui.a_image->width()); 

    ui.W_Spin->setMinimum(0);
    ui.W_Spin->setMaximum(ui.a_image->width());

    ui.H_Spin->setMinimum(0);
    ui.H_Spin->setMaximum(ui.a_image->height());
}

void qt::ROI()
{
    int x, y, w, h;

    
    Mat logo = imread(filename.toLocal8Bit().toStdString());

    
    x=ui.X_Slider->value();
    y= ui.Y_Slider->value();
    w= ui.W_Slider->value();
    h= ui.H_Slider->value();
   

    cv:Mat m_roi = logo(Rect(x, y, w, h));
    

    imshow("ROI", m_roi);
}



void qt::HIST()
{


    Mat srcImage, dstImage;
    Mat srcHist, dstHist;
    srcImage = imread(filename.toLocal8Bit().toStdString());
    cv::cvtColor(srcImage, srcImage, CV_BGR2GRAY);
    if (!srcImage.data) 
        printf("读取图片错误");
    imshow("原始图", srcImage);
    //进行直方图均衡化
    equalizeHist(srcImage, dstImage);
    //显示结果
    imshow("直方图均衡化", dstImage);

    //计算图像的直方图  
    //定义变量         
    int dims = 1;
    float hranges[] = { 0, 255 };
    const float* ranges[] = { hranges };   // 这里需要为const类型  
    int size = 256;
    int channels = 0;
    calcHist(&srcImage, 1, &channels, Mat(), srcHist, dims, &size, ranges);
    calcHist(&dstImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);

    Mat srcHistImage(size, size, CV_8U, Scalar(0));
    Mat dstHistImage(size, size, CV_8U, Scalar(0));
    //获取最大值和最小值  
    double minValue = 0;
    double srcMaxValue = 0;
    double dstMaxValue = 0;
    minMaxLoc(srcHist, &minValue, &srcMaxValue, 0, 0);
    minMaxLoc(dstHist, &minValue, &dstMaxValue, 0, 0);
    //绘制出直方图  
    //saturate_cast函数的作用即是：当运算完之后，结果为负，则转为0，结果超出255，则为255。  
    int hpt = saturate_cast<int>(0.9 * size);
    for (int i = 0; i < 256; i++)
    {
        float srcValue = srcHist.at<float>(i);           //   注意hist中是float类型 
        float dstValue = dstHist.at<float>(i);
        //拉伸到0-max  
        int srcRealValue = saturate_cast<int>(srcValue * hpt / srcMaxValue);
        int dstRealValue = saturate_cast<int>(dstValue * hpt / dstMaxValue);

        line(srcHistImage, Point(i, size - 1), Point(i, size - srcRealValue), Scalar(255));
        line(dstHistImage, Point(i, size - 1), Point(i, size - dstRealValue), Scalar(255));
    }
    imshow("原图直方图", srcHistImage);
    imshow("均衡后直方图", dstHistImage);

    waitKey(0);
}






