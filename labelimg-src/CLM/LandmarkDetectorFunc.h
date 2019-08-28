/*
����  �����ؼ�����ʵ��

����:https://github.com/TadasBaltrusaitis/OpenFace

��Ȩ�����������Ƽ�.����

*/
#ifndef __LANDMARK_DETECTOR_FUNC_h_
#define __LANDMARK_DETECTOR_FUNC_h_

// OpenCV includes
#include <opencv2/core/core.hpp>

#include <CLM/LandmarkDetectorParameters.h>
#include <CLM/LandmarkDetectorUtils.h>
#include <CLM/LandmarkDetectorModel.h>

#ifdef THIS_CLM_LANDMARK
#define DLL_EXPORT_CLMLANDMARK __declspec(dllexport)
#else
#define DLL_EXPORT_CLMLANDMARK
#endif

using namespace std;

namespace LandmarkDetector
{

	//================================================================================================================
	// Landmark detection in videos, need to provide an image and model parameters (default values work well)
	// Optionally can provide a bounding box from which to start tracking
	//================================================================================================================
	bool DLL_EXPORT_CLMLANDMARK DetectLandmarksInVideo(const cv::Mat_<uchar> &grayscale_image, CLNF& clnf_model, FaceModelParameters& params);
	bool DLL_EXPORT_CLMLANDMARK DetectLandmarksInVideo(const cv::Mat_<uchar> &grayscale_image, const cv::Mat_<float> &depth_image, CLNF& clnf_model, FaceModelParameters& params);

	bool DLL_EXPORT_CLMLANDMARK DetectLandmarksInVideo(const cv::Mat_<uchar> &grayscale_image, const cv::Rect_<double> bounding_box, CLNF& clnf_model, FaceModelParameters& params);
	bool DLL_EXPORT_CLMLANDMARK DetectLandmarksInVideo(const cv::Mat_<uchar> &grayscale_image, const cv::Mat_<float> &depth_image, const cv::Rect_<double> bounding_box, CLNF& clnf_model, FaceModelParameters& params);

	
	// Providing a bounding box
	bool DetectLandmarksInImage(const cv::Mat_<uchar> &grayscale_image, const cv::Rect_<double> bounding_box, CLNF& clnf_model, FaceModelParameters& params);

	
	bool DetectLandmarksInImage(const cv::Mat_<uchar> &grayscale_image, const cv::Mat_<float> depth_image, const cv::Rect_<double> bounding_box, CLNF& clnf_model, FaceModelParameters& params);

	//================================================================
	// Helper function for getting head pose from CLNF parameters

	// Return the current estimate of the head pose, this can be either in camera or world coordinate space
	// The format returned is [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
	cv::Vec6d GetPoseCamera(const CLNF& clnf_model, double fx, double fy, double cx, double cy);
	cv::Vec6d GetPoseWorld(const CLNF& clnf_model, double fx, double fy, double cx, double cy);
	
	// Getting a head pose estimate from the currently detected landmarks, with appropriate correction for perspective
	// This is because rotation estimate under orthographic assumption is only correct close to the centre of the image
	// These methods attempt to correct for that
	// The pose returned can be either in camera or world coordinates
	// The format returned is [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
	cv::Vec6d DLL_EXPORT_CLMLANDMARK GetCorrectedPoseCamera(const CLNF& clnf_model, double fx, double fy, double cx, double cy);
	cv::Vec6d DLL_EXPORT_CLMLANDMARK GetCorrectedPoseWorld(const CLNF& clnf_model, double fx, double fy, double cx, double cy);

	//===========================================================================

}
#endif
