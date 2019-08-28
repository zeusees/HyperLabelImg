#ifndef __opencvFaceDetect_h_
#define __opencvFaceDetect_h_

/*
功能  人脸检测实现

作者:sunjunlishi



*/
#include <opencv2/core.hpp>
#include <vector>



namespace FaceDetector
{

	bool  DetectFaceSingleYU(cv::Rect_<double> & o_regions,  unsigned char*  intensity,int w,int h);
	

};

#endif