//#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/cxcore.hpp>
#include <stdio.h>
using namespace std;
using namespace cv;
 
Mat g_srcImage;
Mat g_grayImage;
 
const int g_dReferWidth=19;//mm 
double g_dPixelsPerMetric;
vector<vector<cv::Point>> g_vContours;
vector<Vec4i> g_vHierarchy;
bool g_bFirst=true;
 
static cv::Point2f midpoint(cv::Point2f& ptA, cv::Point2f& ptB);//求中点 
static float getDistance(Point2f pointA, Point2f pointB);//求距离
static bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2);//按照 x坐标 排序
float minx(float a,float b,float c,float d);
float maxx(float a,float b,float c,float d);

float zb[100];
double dWidth;
double dHeight;
unsigned t=0;
//-----------------------------------【main( )函数】--------------------------------------------
//      描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{

	double timeStart = (double)getTickCount();
	g_srcImage = imread("src.bmp", 1);
	//灰度 降低计算量
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	
	//高斯滤波 降噪
	// GaussianBlur(g_grayImage, g_grayImage, Size(5,5), 0);
	// cv::namedWindow("高斯滤波", CV_WINDOW_NORMAL);
	// imshow("高斯滤波", g_grayImage);
	Mat dst;
	medianBlur(g_grayImage, dst, 11);//11
	//blur(g_grayImage,dst,Size(7,7));
	imshow("dst",dst);

	
	//边缘检测
	Canny(dst, g_grayImage, 50,100);
	cv::namedWindow("Canny", CV_WINDOW_NORMAL);
	imshow("Canny", g_grayImage);

 
	//寻找轮廓
	findContours(g_grayImage, g_vContours, g_vHierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	std::sort(g_vContours.begin(),g_vContours.end(), ContoursSortFun);//按照从左到右 排序
	for (unsigned i = 0; i < g_vContours.size(); i++) {
		
		if (contourArea(g_vContours[i]) < 100)//面积太小 则忽略
			continue;
 

		//获取（第一个轮廓）点的最小包围旋转矩形，g_vContours[i]是第几个轮廓
		RotatedRect box = minAreaRect(g_vContours[i]);
		Point2f boxPoints[4];//Point2f表示Point类的两个数据x,y为float类型（就是一个点）；vector 表示存放四维int
		box.points(boxPoints);
 
		Point2f pointA = midpoint(boxPoints[0], boxPoints[1]);//左中的点
		Point2f pointB = midpoint(boxPoints[1], boxPoints[2]);//上中的点
		Point2f pointC = midpoint(boxPoints[2], boxPoints[3]);//右中的点
		Point2f pointD = midpoint(boxPoints[3], boxPoints[0]);//下中的点
		
		zb[2*t]=minx(pointA.x,pointB.x,pointC.x,pointD.x);
		zb[2*t+1]=maxx(pointA.x,pointB.x,pointC.x,pointD.x);


		circle(g_srcImage, pointA, 2, Scalar(0, 0, 255));//四条边中点分别画圆
		circle(g_srcImage, pointB, 2, Scalar(0, 0, 255));
		circle(g_srcImage, pointC, 2, Scalar(0, 0, 255));
		circle(g_srcImage, pointD, 2, Scalar(0, 0, 255));
 
		line(g_srcImage, pointA, pointC, Scalar(255, 0, 0));//左中和右中连线
		line(g_srcImage, pointD, pointB, Scalar(255, 0, 0));//上中和下中连线
 
		dWidth = getDistance(pointA, pointC);//宽度：上和下的距离
		dHeight = getDistance(pointD, pointB);//高度：左和右的距离

		if(dWidth > dHeight)
		{
			double t =  dWidth;
			dWidth = dHeight;
			dHeight = t;
		}

		cout <<"dWidth:"<< dWidth <<"   "<<"dHeight:"<<"   "<< dHeight<< endl;

		for (int i = 0; i <=3; i++)
		{
			line(g_srcImage, boxPoints[i], boxPoints[(i + 1) % 4], Scalar(0, 255, 0));
		}

		t++;
	}
	for (t = 0; t < 32; t++)
	{		
		float jianju = fabs(zb[2*t] - zb[2*t+1]);
		if(jianju > 15)
		continue;
		//cout <<"   "<< jianju<< endl;
		cout <<"   "<< zb[t]<< endl;
	}
	
 
	cv::namedWindow("效果", CV_WINDOW_NORMAL);
	cv::imshow("效果", g_srcImage);

	double nTime = ((double)getTickCount() - timeStart) / getTickFrequency();
	cout << "运行上面程序共耗时：" << nTime << "秒\n" << endl;
 
	waitKey(0);
	return 0;
}
 
Point2f midpoint(Point2f& ptA, Point2f& ptB) {
	return Point2f((ptA.x+ ptB.x)*0.5,(ptA.y + ptB.y)*0.5);
}
 
float getDistance(Point2f pointA, Point2f pointB)
{
	float distance;
	distance = powf((pointA.x - pointB.x), 2) + powf((pointA.y - pointB.y), 2);
	distance = sqrtf(distance);
	return distance;
}
 
bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2) {
	return  (contour1[0].x<contour2[0].x); // a.x < b.x;
}

float minx(float a,float b,float c,float d)
{
	float min_x;
	//自右往左
	//min_x = a < b ? a < c ? a < d ? a : d : c < d ? c : d : b < c ? b < d ? b : d : c < d ? c : d;
	min_x = min(a,b);
	min_x = min(min_x,c);
	min_x = min(min_x,d);
	return min_x;
}

float maxx(float a,float b,float c,float d)
{
	float max_x;
	max_x = max(a,b);
	max_x = max(max_x,c);
	max_x = max(max_x,d);
	return max_x;
}