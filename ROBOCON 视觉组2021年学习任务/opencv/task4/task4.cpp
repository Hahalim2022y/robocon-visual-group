//#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/cxcore.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/types_c.h>
#include <stdio.h>
using namespace std;
using namespace cv;

Mat g_srcImage;
Mat g_grayImage;


double g_dPixelsPerMetric;
vector<vector<cv::Point>> g_vContours;
vector<Vec4i> g_vHierarchy;
bool g_bFirst=true;

static cv::Point2f midpoint(cv::Point2f& ptA, cv::Point2f& ptB);//???
static Point2f getcenter(Point2f pointA, Point2f pointB,Point2f pointC, Point2f pointD);
static bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2);//?? x?? ??


int main(int argc, const char** argv)
{
    Mat g_srcImage;
    Mat g_grayImage;
    cvStartWindowThread();
    double timeStart = (double)getTickCount();
    g_srcImage = imread("/Users/leileihoujie/Desktop/xcode/task/task4/temp01.jpg", 1);
    //?? ?????
    //cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);//COLOR_BGR2GRAY
    cv::cvtColor(g_srcImage, g_grayImage, CV_RGB2GRAY);

    Mat dst;
    medianBlur(g_grayImage, dst, 25);//13 15


    cv::namedWindow("dst", CV_WINDOW_NORMAL);
    cv::resizeWindow("dst",640,480);
    cv::imshow("dst", dst);


    //????
    Mat canny;
    Canny(dst, canny, 40,80);//50,100
    cv::namedWindow("Canny", CV_WINDOW_NORMAL);
    cv::resizeWindow("Canny",640,480);
    cv::imshow("Canny", canny);

    //????
    findContours(canny, g_vContours, g_vHierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE);



    std::sort(g_vContours.begin(),g_vContours.end(), ContoursSortFun);//?????? ??
    for (unsigned i = 0; i < g_vContours.size(); i++) {

        if (contourArea(g_vContours[i]) < 40)//????40 ???
            continue;
        if (i>0 && (contourArea(g_vContours[i])== contourArea(g_vContours[i-1])))//???? ???
            continue;

        //????????????????????g_vContours[i]??????
        RotatedRect box = minAreaRect(g_vContours[i]);
        Point2f boxPoints[4];//Point2f??Point??????x,y?float??????????vector ??????int
        box.points(boxPoints);

        Point2f pointA = midpoint(boxPoints[0], boxPoints[1]);//????
        Point2f pointB = midpoint(boxPoints[1], boxPoints[2]);//????
        Point2f pointC = midpoint(boxPoints[2], boxPoints[3]);//????
        Point2f pointD = midpoint(boxPoints[3], boxPoints[0]);//????

        Point2f pointcenter;
        pointcenter = getcenter(pointA,pointB,pointC,pointD);
        circle(g_srcImage, pointcenter, 6,Scalar(0, 0, 255), 8, 8,0);
        cout <<"x:" <<pointcenter.x<<"   "<<"y:"<< pointcenter.y<< endl;

        for (int i = 0; i <=3; i++)
        {
            line(g_srcImage, boxPoints[i], boxPoints[(i + 1) % 4], Scalar(0, 255, 0),3);
        }
    }
    //circle(g_srcImage, Point(1900,1800), 40,Scalar(0, 0, 255), 20, 8,0);

    cv::namedWindow("lala", CV_WINDOW_NORMAL);
    cv::resizeWindow("lala",640,480);
    cv::imshow("lala", g_srcImage);


    double nTime = ((double)getTickCount() - timeStart) / getTickFrequency();
    cout << "??????????" << nTime << "?\n" << endl;

    
    waitKey(0);
    cvDestroyAllWindows();
    return 0;

}

Point2f midpoint(Point2f& ptA, Point2f& ptB) {
    return Point2f((ptA.x+ ptB.x)*0.5,(ptA.y + ptB.y)*0.5);
}

Point2f getcenter(Point2f pointA, Point2f pointB,Point2f pointC, Point2f pointD)
{
    Point2f pointcenter;
    pointcenter.x = (pointA.x + pointC.x)/2;
    pointcenter.y = (pointB.y + pointD.y)/2;
    return pointcenter;
}

bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2) {
    return  (contour1[0].x<contour2[0].x); // a.x < b.x;
}

