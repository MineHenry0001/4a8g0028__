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
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QVTKWidget.h>
#include <MyQLabel.h>
#include "ui_qt.h"
using namespace cv;

//QMessageBox::warning(NULL, "Y", "YUYUY");

qt::qt(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	connect(ui.a_image, SIGNAL(Mouse_Move()), this, SLOT(Mouse_Move()));
	connect(ui.a_image, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
	connect(ui.a_image, SIGNAL(Mouse_Release()), this, SLOT(Mouse_Relese()));
	connect(ui.a_image, SIGNAL(Mouse_Left()), this, SLOT(Mouse_Left()));
}

void qt::open()
{
	filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images(*.png *.jpg *,jpeg *.bmp *.gif"));


	if (QString::compare(filename, QString()) != 0)
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

	Mat img = imread(filename.toLocal8Bit().toStdString());
	int width = img.rows;
	int height = img.cols;

	ui.a_label->setText(QString::number(width) + "," + QString::number(height));
}

void qt::ROI()
{
	int x, y, w, h;

	Mat logo = imread(filename.toLocal8Bit().toStdString());

	x = ui.X_Slider->value();
	y = ui.Y_Slider->value();
	w = ui.W_Slider->value();
	h = ui.H_Slider->value();

	Mat m_roi = logo(Rect(x, y, w, h)); 

	imshow("ROI", m_roi);
}



void qt::HIST()
{


	Mat srcImage, dstImage;
	Mat srcHist, dstHist;
	srcImage = imread(filename.toLocal8Bit().toStdString());
	cv::cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	if (!srcImage.data)
		printf("讀取圖片錯誤");
	imshow("灰階", srcImage);
	//进行直方图均衡化
	equalizeHist(srcImage, dstImage);
	//显示结果
	imshow("均衡化", dstImage);

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
	imshow("灰階直方圖", srcHistImage);
	imshow("均衡直方圖", dstHistImage);

	waitKey(0);
}

void qt::Thresholding()
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	cvtColor(img, img, CV_BGR2GRAY);//轉灰階
	threshold(img, img, ui.T_Slider->value(), 255, CV_THRESH_BINARY);
	imshow("影像二值化", img);
}

void qt::Color()
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	cvtColor(img, img, COLOR_RGB2HSV);
	imshow("HSV", img);
}

void qt::GLOW()
{

	Mat img = imread(filename.toLocal8Bit().toStdString());
	//int x = ui.Ga_Slider->value();
	GaussianBlur(img, img, Size(5, 5), 1.5);
	imshow("Gauss", img);
}

void qt::GHIGH()
{

}

void qt::Matlab()//均值
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	boxFilter(img, img, -1, Size(5, 5), Point(-1, -1), true);
	imshow("Matlab", img);
}

void qt::Median()//中值
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	medianBlur(img, img, 5);
	imshow("Median", img);
}

void qt::Laplacian()//拉普拉斯 未完成
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	Mat src;

	//Laplacian(src, img, CV_16S, 7);
}

void qt::Bilateral() //雙邊
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	Mat src;
	bilateralFilter(img, src, 10, 10, 10);
	imshow("Bilateral", src);
}

void qt::Sobel() //未完成
{
	Mat src = imread(filename.toLocal8Bit().toStdString());
	Mat x, y;
	//Sobel(src, CV_16S, 1, 0);
}

void qt::Rotate()
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	Mat dst;
	double angle = 45;
	Size src_sz = img.size();
	Size dst_sz(src_sz.height, src_sz.width);
	int len = max(img.cols, img.rows);
	Point2f center(len / 2., len / 2.); //指定旋轉中心
	Mat rot_mat = getRotationMatrix2D(center, angle, 1.0); //旋轉矩陣
	warpAffine(img, dst, rot_mat, dst_sz); //根據旋轉矩陣進行仿射轉換
	imshow("Rotate", dst);


}

void qt::Pan()
{
	Mat img = imread(filename.toLocal8Bit().toStdString());
	Mat dst;
	Size dst_sz = img.size();
	Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);
	t_mat.at<float>(0, 0) = 1;
	t_mat.at<float>(0, 2) = 20; //水平平移量
	t_mat.at<float>(1, 1) = 1;
	t_mat.at<float>(1, 2) = 10; //垂直平移量
	warpAffine(img, dst, t_mat, dst_sz);
	
	imshow("Pan", dst);
}

void qt::Mouse_Pressed()
{
	ui.a_label->setText(QString("X = %1, Y = %2").arg(ui.a_image->a).arg(ui.a_image->b));
}		
