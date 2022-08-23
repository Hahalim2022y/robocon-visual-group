#include <opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;


//抓取轮廓中心
void GetContourCenter(CvSeq *contour,CvPoint &p)
{
    //重心法抓中心点
    int contourlength = contour->total;
    CvPoint *pt = 0;
    double avg_px=0,avg_py=0;
    for(int i = 0 ;i < contourlength; i++)
    { 
        pt=CV_GET_SEQ_ELEM(CvPoint,contour,i);
        avg_px+=pt->x;
        avg_py+=pt->y;
    } 
        p.x=avg_px/contourlength;
        p.y=avg_py/contourlength;
}

int main(){

	VideoCapture vid_capture("/home/hahalim/opencv-4.1.1/task/task6/1.mp4");
	//VideoCapture cap(0);
    Mat imgOriginal;
    Mat gray;
	Mat imgThresholded;
    CvPoint pt;
	int iLowH = 0;
	int iHighH = 180;
 
	int iLowS = 0;
	int iHighS = 30;
 
	int iLowV = 220;
	int iHighV = 255;
 

    while (1){

		vid_capture.read(imgOriginal);

        // resize(img, img, Size(640, 480));
        // cvtColor(img, gray, COLOR_BGR2GRAY); 
        // blur(gray, gray, Size(9, 9), Point(-1, -1));
        // threshold(gray, gray, 220, 255, THRESH_BINARY);

		//GaussianBlur(imgOriginal, imgThresholded, Size(3, 3), 0, 0, BORDER_DEFAULT);

		//去噪点																							  //开操作 (去除一些噪点)
		// Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		// erode(imgOriginal,imgThresholded,element);
		// morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
		// //闭操作 (连接一些连通域)
		// morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);	
		// Mat kernel =  getStructuringElement(MORPH_RECT, Size(5,5));
		// //close processing ，连通区域
		// morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, kernel);

		//blur(imgOriginal,imgThresholded,Size(7,7));

		//color
		Mat imgHSV;
		vector<Mat> hsvSplit;
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
		 //因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
		split(imgHSV, hsvSplit);
		equalizeHist(hsvSplit[2], hsvSplit[2]);
		merge(hsvSplit, imgHSV);
		//Mat imgThresholded;
 
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
 
 		// //开操作 (去除一些噪点)
		// Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		// erode(imgOriginal,imgThresholded,element);
		// morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
		// //闭操作 (连接一些连通域)
		// morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);	
		// Mat kernel =  getStructuringElement(MORPH_RECT, Size(5,5));
		// //close processing ，连通区域
		// morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, kernel);

		//开操作 (去除一些噪点)
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
 
		// //闭操作 (连接一些连通域)
		morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
		//morphologyEx(imgThresholded, imgThresholded, MORPH_BLACKHAT , element);

		Mat dst;
		medianBlur(imgThresholded, dst, 41);
		imshow("dst",dst);

        std::vector<std::vector<cv::Point> > contours;
        findContours(dst, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//Ñ°ÕÒÂÖÀ
        cv::Mat result(dst.size(), CV_8U, cv::Scalar(255,0,255));
        cv::drawContours(imgOriginal, contours,-1, cv::Scalar(255,0,255), 3);



        std::vector<std::vector<Point> >::const_iterator itc = contours.begin(); 
        itc = contours.begin();  
        while(itc != contours.end())  
        {  
        	//计算所有的距  
             Moments mom = moments(Mat(*itc++));  
            //计算并画出质心  
            circle(imgOriginal,Point(mom.m10/mom.m00,mom.m01/mom.m00),2,cv::Scalar(3),2);
			char tam[100]; 
			sprintf(tam, "(%0.0f,%0.0f)",mom.m10/mom.m00,mom.m10/mom.m00);   
			putText(imgOriginal, tam, Point(mom.m10/mom.m00,mom.m01/mom.m00), FONT_HERSHEY_SIMPLEX, 0.4, cvScalar(255,0,255),1); 
        }  

        //imshow("test", img);
		// cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);
		// cv::resizeWindow("imgOriginal",640,480);
		// cv::imshow("imgOriginal", imgOriginal);

		// cv::namedWindow("dst", CV_WINDOW_NORMAL);
		// cv::resizeWindow("dst",640,480);
		// cv::imshow("dst", dst);
		imshow("imgOriginal",imgOriginal);

        char key=waitKey(1);
        if (key == 'q')break;
	
    }

	vid_capture.release();
	//destroyAllWindows();
	return 0;

}


// #include <iostream>
// #include "opencv2/highgui/highgui_c.h"
// #include "opencv2/imgproc/imgproc_c.h"
// #include<opencv2/opencv.hpp>
// using namespace cv;
// using namespace std;
 
// int main(int argc, char** argv)
// {
// 	//VideoCapture cap(0); //capture the video from web cam
// 	VideoCapture vid_capture("/home/hahalim/opencv-4.1.1/task/task6/1.mp4");
 
	
 
// 	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
 
// 	int iLowH = 0;
// 	int iHighH = 90;
 
// 	int iLowS = 0;
// 	int iHighS = 255;
 
// 	int iLowV = 170;
// 	int iHighV = 255;
 
// 	//Create trackbars in "Control" window
// 	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
// 	cvCreateTrackbar("HighH", "Control", &iHighH, 179);
 
// 	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
// 	cvCreateTrackbar("HighS", "Control", &iHighS, 255);
 
// 	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
// 	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
 
// 	while (true)
// 	{
// 		Mat imgOriginal;
// 		vid_capture.read(imgOriginal);
 
// 		Mat imgHSV;
// 		vector<Mat> hsvSplit;
// 		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
// 													  //因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
// 		split(imgHSV, hsvSplit);
// 		equalizeHist(hsvSplit[2], hsvSplit[2]);
// 		merge(hsvSplit, imgHSV);
// 		Mat imgThresholded;
 
// 		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
 
// 																									  //开操作 (去除一些噪点)
// 		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
// 		morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
 
// 		//闭操作 (连接一些连通域)
// 		morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
 
// 		imshow("Thresholded Image", imgThresholded); //show the thresholded image
// 		imshow("Original", imgOriginal); //show the original image
 
// 		char key = (char)waitKey(300);
// 		if (key == 27)
// 			break;
// 	}
 
// 	return 0;
 
// }