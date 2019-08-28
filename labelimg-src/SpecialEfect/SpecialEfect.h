#pragma once
/*
	一些特殊效果，羽化，以及旋转操作
	缩放-局部均值图像缩小，避免缩小后的锯齿效果


*/
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;
struct S_BGR
{
	unsigned B;
	unsigned G;
	unsigned R;
};



//photoshop中的图层柔光处理
void Soft_Lighten(Mat& src1, Mat& src2, Mat& dst);

//提取图像高光区
void GetTheHighLight(Mat& src,Mat& dstHight);

//进行滤色处理
void Screen(Mat& src1, Mat& src2, Mat& dst);

//锐化操作
void USMSharp(Mat& imgSrc,int stepCount = 0);
//锐化操作
Mat sharpenImage0(const cv::Mat &image);

//局部均值图像缩小，避免缩小图片后的锯齿效果
void scalePartAverage(const Mat &src, Mat &dst, int dstcols, int dstrows);

//顺时针90度，逆时针90，垂直镜像，3中转转
void Rotate90_180(Mat& src,Mat& dst,int type);
//旋转任意角度
Mat rotateImage2(Mat& img, int degree, int iWhite);

cv::Point_<double>  GetNewPoint(cv::Point_<double> in, double angle, cv::Point_<double> center);

void RotateMatAndPts(std::vector<cv::Point>& mainPts, cv::Mat& clmResult, double angle, int iTag);

//设置区域透明
void FeatherPtsOut(Mat& matResult,vector<cv::Point> pts,Mat& maskBlur);
void FeatherPtsOutWhole(Mat& matResult,vector<cv::Point> mainPts,bool bBlack = true);
void FeatherPtsInstead(Mat& matResult,vector<cv::Point> pts);


//产生高斯核
Mat gaussian_kernal(int dim,double sigma);

