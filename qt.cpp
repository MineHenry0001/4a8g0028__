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


//QMessageBox::warning(NULL, "Y", "YUYUY");

qt::qt(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	m_timer = new QTimer(this);
	connect(ui.a_image, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
	connect(m_timer, SIGNAL(timeout()), this, SLOT(readFarme()));
	connect(ui.OpenP, SIGNAL(clicked()), this, SLOT(on_openCamera_clicked()));
	//connect(ui.PictureP, SIGNAL(clicked()), this, SLOT(takingPictures()));
	connect(ui.CloseP, SIGNAL(clicked()), this, SLOT(on_closeCamera_clicked()));


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

	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	int width = img.rows;
	int height = img.cols;

	ui.a_label->setText(QString::number(width) + "," + QString::number(height));
}
	
void qt::ROI()
{
	int x, y, w, h;

	cv::Mat logo = cv::imread(filename.toLocal8Bit().toStdString());

	x = ui.X_Slider->value();
	y = ui.Y_Slider->value();
	w = ui.W_Slider->value();
	h = ui.H_Slider->value();

	cv::Mat m_roi = logo(cv::Rect(x, y, w, h)); 

	imshow("ROI", m_roi);
}

void qt::HIST()
{


	cv::Mat srcImage, dstImage;
	cv::Mat srcHist, dstHist;
	srcImage = cv::imread(filename.toLocal8Bit().toStdString());
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
	calcHist(&srcImage, 1, &channels, cv::Mat(), srcHist, dims, &size, ranges);
	calcHist(&dstImage, 1, &channels, cv::Mat(), dstHist, dims, &size, ranges);


	cv::Mat srcHistImage(size, size, CV_8U, cv::Scalar(0));
	cv:: Mat dstHistImage(size, size, CV_8U, cv::Scalar(0));
	//获取最大值和最小值  
	double minValue = 0;
	double srcMaxValue = 0;
	double dstMaxValue = 0;
	minMaxLoc(srcHist, &minValue, &srcMaxValue, 0, 0);
	minMaxLoc(dstHist, &minValue, &dstMaxValue, 0, 0);
	//绘制出直方图  
	//saturate_cast函数的作用即是：当运算完之后，结果为负，则转为0，结果超出255，则为255。  
	int hpt = cv::saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float srcValue = srcHist.at<float>(i);           //   注意hist中是float类型 
		float dstValue = dstHist.at<float>(i);
		//拉伸到0-max  
		int srcRealValue = cv::saturate_cast<int>(srcValue * hpt / srcMaxValue);
		int dstRealValue = cv::saturate_cast<int>(dstValue * hpt / dstMaxValue);

		line(srcHistImage, cv::Point(i, size - 1),cv:: Point(i, size - srcRealValue), cv::Scalar(255));
		line(dstHistImage, cv::Point(i, size - 1), cv::Point(i, size - dstRealValue), cv::Scalar(255));
	}
	imshow("灰階直方圖", srcHistImage);
	imshow("均衡直方圖", dstHistImage);

	cv::waitKey(0);
}

void qt::Thresholding()
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::cvtColor(img, img, CV_BGR2GRAY);//轉灰階
	threshold(img, img, ui.T_Slider->value(), 255, CV_THRESH_BINARY);
	imshow("影像二值化", img);
}

void qt::Color()
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::cvtColor(img, img, cv::COLOR_RGB2HSV);
	imshow("HSV", img);
}

void qt::GLOW()
{

	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	//int x = ui.Ga_Slider->value();
	cv::GaussianBlur(img, img, cv::Size(5, 5), 1.5);
	imshow("Gauss", img);
}

void qt::GHIGH()
{

}	

void qt::Matlab()//均值
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	boxFilter(img, img, -1, cv::Size(5, 5), cv::Point(-1, -1), true);
	imshow("Matlab", img);
}

void qt::Median()//中值
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::medianBlur(img, img, 5);
	imshow("Median", img);
}

void qt::Laplacian()//拉普拉斯 未完成
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::Mat src;

	//Laplacian( img, CV_16S, 7);
}

void qt::Bilateral() //雙邊
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::Mat src;
	bilateralFilter(img, src, 10, 10, 10);
	imshow("Bilateral", src);
}

void qt::Sobel() //未完成
{
	cv::Mat src = cv::imread(filename.toLocal8Bit().toStdString());
	cv::Mat x, y;
	//Sobel(src, CV_16S, 1, 0);
}

void qt::Rotate()
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::Mat dst;
	double angle = 90;
	cv::Size src_sz = img.size();
	cv::Size dst_sz(src_sz.height, src_sz.width);
	int len = std::max(img.cols, img.rows);
	cv::Point2f center(len / 2., len / 2.); //指定旋轉中心
	cv::Mat rot_mat = getRotationMatrix2D(center, angle, 1.0); //旋轉矩陣
	warpAffine(img, dst, rot_mat, dst_sz); //根據旋轉矩陣進行仿射轉換
	imshow("Rotate", dst);


}

void qt::Pan()
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::Mat dst;
	cv::Size dst_sz = img.size();
	cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1);
	t_mat.at<float>(0, 0) = 1;
	t_mat.at<float>(0, 2) = 20; //水平平移量
	t_mat.at<float>(1, 1) = 1;
	t_mat.at<float>(1, 2) = 10; //垂直平移量
	warpAffine(img, dst, t_mat, dst_sz);
	
	imshow("Pan", dst);
}

void qt::affine_pan() //仿射平移
{
	cv::Mat img = cv::imread(filename.toLocal8Bit().toStdString());
	cv::Mat dst_warp;
	cv::Point2f srcPoints[3];//原圖中的三點 ,一個包含三維點（x，y）的數組，其中x、y是浮點型數
	cv::Point2f dstPoints[3];//目標圖中的三點  

	srcPoints[0] = cv::Point2f(0, 0);
	srcPoints[1] = cv::Point2f(0, img.rows);
	srcPoints[2] = cv::Point2f(img.cols, 0);
		
	dstPoints[0] = cv::Point2f(0, img.rows * 0.3);
	dstPoints[1] = cv::Point2f(img.cols * 0.25, img.rows * 0.75);
	dstPoints[2] =cv::Point2f(img.cols * 0.75, img.rows * 0.25);

	cv::Mat M1 = getAffineTransform(srcPoints, dstPoints);//由三個點對計算變換矩陣  

	warpAffine(img, dst_warp, M1, img.size());//仿射變換  cv::Mat M4 = getAffineTransform(srcPoints2, dstPoints2);
	imshow("0.", dst_warp);
}

void qt::Mouse_Pressed()
{
	ui.a_label->setText(QString("X = %1, Y = %2").arg(ui.a_image->a).arg(ui.a_image->b));
}		


void qt::readFarme()
{
	m_videoCapture.read(image);
	QImage imag = MatImageToQt(image);
	ui.Cameralabel->setPixmap(QPixmap::fromImage(imag));
}

void qt::on_openCamera_clicked()
{
	
	m_videoCapture.open(0);
	m_timer->start(33);//每33ms刷新一次
}

void qt::on_closeCamera_clicked()
{
	m_timer->stop();
	m_videoCapture.release();
	QApplication* app;	
	app->exit(0);	
}

QImage qt::MatImageToQt(const cv::Mat& src)
{
	//CV_8UC1 8位无符号的单通道---灰度图片
	if (src.type() == CV_8UC1)
	{
		//使用给定的大小和格式构造图像
		//QImage(int width, int height, Format format)
		QImage qImage(src.cols, src.rows, QImage::Format_Indexed8);
		//扩展颜色表的颜色数目
		qImage.setColorCount(256);

		//在给定的索引设置颜色
		for (int i = 0; i < 256; i++)
		{
			//得到一个黑白图
			qImage.setColor(i, qRgb(i, i, i));
		}
		//复制输入图像,data数据段的首地址
		uchar* pSrc = src.data;
		//
		for (int row = 0; row < src.rows; row++)
		{
			//遍历像素指针
			uchar* pDest = qImage.scanLine(row);
			//从源src所指的内存地址的起始位置开始拷贝n个
			//字节到目标dest所指的内存地址的起始位置中
			memcmp(pDest, pSrc, src.cols);
			//图像层像素地址
			pSrc += src.step;
		}
		return qImage;
	}
	//为3通道的彩色图片
	else if (src.type() == CV_8UC3)
	{
		//得到图像的的首地址
		const uchar* pSrc = (const uchar*)src.data;
		//以src构造图片
		QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888);
		//在不改变实际图像数据的条件下，交换红蓝通道
		return qImage.rgbSwapped();
	}
	//四通道图片，带Alpha通道的RGB彩色图像
	else if (src.type() == CV_8UC4)
	{
		const uchar* pSrc = (const uchar*)src.data;
		QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
		//返回图像的子区域作为一个新图像
		return qImage.copy();
	}
	else
	{
		return QImage();
	}
}
