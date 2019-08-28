// Precompiled headers stuff
/*
功能  人脸关键点处理对外接口

作者:https://github.com/TadasBaltrusaitis/OpenFace

版权：

*/
// OpenCV stuff
#include<opencv2\core.hpp>
// C++ stuff
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>



#ifdef WIN32

#else
	#include "pthread.h"
	#include <jni.h>
	#include <android/log.h>
	#define LOG_TAG "logfromc"
	#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
	#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

/*
   提供人脸检测 68个关键点的 对外接口 源码来源于 github
*/
#ifdef THIS_CLM_LANDMARK
#define DLL_EXPORT_CLMLANDMARK __declspec(dllexport)
#else
#define DLL_EXPORT_CLMLANDMARK
#endif
namespace LandmarkDetector { class CLNF; }

using namespace std;

DLL_EXPORT_CLMLANDMARK LandmarkDetector::CLNF * GetCLNF();

	DLL_EXPORT_CLMLANDMARK void CLM_CalcParams(cv::Vec6d& out_params_global, cv::Mat_<double>& out_params_local,
		const cv::Mat_<double>& landmark_locations);

	DLL_EXPORT_CLMLANDMARK void CLM_GetParamGlobal(cv::Vec6d& out_params_global);

	cv::Mat_<double> CLM_Get3DLandMark();
	DLL_EXPORT_CLMLANDMARK cv::Mat_<double> CLM_Get2d_3Mark();
	DLL_EXPORT_CLMLANDMARK cv::Mat_<double> CLM_Get2d_3MarkNew(std::vector<cv::Point> pts68);
	
	//
	DLL_EXPORT_CLMLANDMARK bool CLM_DoInit();


	DLL_EXPORT_CLMLANDMARK std::vector<cv::Point2f>  GetMainPts();



	DLL_EXPORT_CLMLANDMARK cv::Mat GetMainSrcMat();
		void ReleaseCLM();

	DLL_EXPORT_CLMLANDMARK bool DoGetMarks(cv::Mat& grayscale_image, cv::Rect_<double> & face_detection);
	
	DLL_EXPORT_CLMLANDMARK bool DoDetectMarksInVideo(cv::Mat& inImg);
	DLL_EXPORT_CLMLANDMARK bool DoDetectMarksInPic(cv::Mat& inImg);


