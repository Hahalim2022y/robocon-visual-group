//#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
 
using namespace std;
 
using namespace cv;
 
 
int main() {
	Mat srcImage = imread("src.jpg");
 
	Mat midImage, dstImage;
 
	cvtColor(srcImage, midImage, COLOR_BGR2GRAY);//灰度
	imshow("灰度", midImage);
 
	GaussianBlur(midImage, midImage, Size(7, 7), 2, 2);//高斯滤波
	imshow("高斯", midImage);
	
	Canny(midImage, midImage, 90, 35, 3);	
	imshow("Canny", midImage);
 
	vector<Vec3f> cicles;
	HoughCircles(midImage, cicles,HOUGH_GRADIENT, 2, 98, 200, 65, 68, 77);//霍夫圆变换
	cout << "count:" << cicles.size();
 
	for (size_t i = 0; i < cicles.size(); i++)
	{
		Point center(cvRound(cicles[i][0]), cvRound(cicles[i][1]));
		int radius = cvRound(cicles[i][2]);
 
		circle(srcImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		circle(srcImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);
		printf("x = %d,y = %d\n",cvRound(cicles[i][0]),cvRound(cicles[i][1]));
 
	}
 
	imshow("效果", srcImage);
 	
	waitKey(0);
	return 0;
}
