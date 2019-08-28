#pragma once
/*
	photoshop一些 图层效果的处理
	图层混合方式，带透明通道的混合
	亮度混合-上层的亮度混合到下层，采用lab空间
	sunjunlishi
*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
//photoshop对两个图层进行融合，4个通道，含透明通道
 //为什么不用opnecv的addWeight，呵呵，因为尝试了不中，可能我用错了吧
//src上层，dst下层，alpha上层的透明度
void MixLayer(Mat& src,Mat& dst,float alpha = 1.0);

void MixLayerPos(Mat& src,Mat& dst,int posDstX,int posdstY,float alpha = 1.0);

void MixLayerDarkPos(Mat& src,Mat& dst,int posDstX,int posdstY,float alpha = 1.0);
//photoshop颜色混合模式 
void MatColorMix(Mat& src,Mat& dst);

//用上层的亮度替代下层的亮度
Mat MatMixByLight(Mat&src ,Mat& dst,float alpha = 1.0);

//添加边界，边界颜色黑色
Mat MatMixAddEdge(Mat&src,Mat& dst);

void MixLayerPoint(Mat& src,Mat& dst,int& pos,float& alpha);