//#pragma once
//#define CAMERAGET_H
//
//#include <QWidget>
//#include <QImage>
//#include <QPixmap>
//#include <QTimer>
//#include <opencv2/opencv.hpp>
//
//
//
//class CameraGet : public QWidget
//{
//    Q_OBJECT
//
//public:
//    explicit CameraGet(QWidget* parent = 0);
//    ~CameraGet();
//
//public slots:
//    void openCamera(void);   //開啟攝像頭
//    void readFrame(void);    //讀取當前幀資訊
//    void closeCamera(void);  //關閉攝像頭
//    void takingPictures(void);  //拍照
//
//private:
//
//
//    QTimer* timer;
//
//    QImage qimage;  //q開頭表示Qt的影象格式
//
//    cv::VideoCapture capture;
//    cv::Mat cvframe;  //cv開頭表示是OpenCv的影象格式
//};
