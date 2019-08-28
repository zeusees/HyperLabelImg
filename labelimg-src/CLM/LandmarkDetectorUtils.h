/*
功能  人脸检测接口

作者:https://github.com/TadasBaltrusaitis/OpenFace

版权：

*/
//  Header for all external CLNF/CLM-Z/CLM methods of interest to the user
#ifndef __LANDMARK_DETECTOR_UTILS_h_
#define __LANDMARK_DETECTOR_UTILS_h_

// OpenCV includes
#include <opencv2/core/core.hpp>

#include "LandmarkDetectorModel.h"

#ifdef THIS_CLM_LANDMARK
#define DLL_EXPORT_CLMLANDMARK __declspec(dllexport)
#else
#define DLL_EXPORT_CLMLANDMARK
#endif
using namespace std;

namespace LandmarkDetector
{
	//===========================================================================	
	// Defining a set of useful utility functions to be used within CLNF


	//=============================================================================================
	// Helper functions for parsing the inputs
	//=============================================================================================
	/*void get_video_input_output_params(vector<string> &input_video_file, vector<string> &depth_dir, vector<string> &output_files,
		vector<string> &output_video_files, bool& world_coordinates_pose, vector<string> &arguments);*/

	void get_camera_params(int &device, float &fx, float &fy, float &cx, float &cy, vector<string> &arguments);



	//===========================================================================
	// Fast patch expert response computation (linear model across a ROI) using normalised cross-correlation
	//===========================================================================
	// This is a modified version of openCV code that allows for precomputed dfts of templates and for precomputed dfts of an image
	// _img is the input img, _img_dft it's dft (optional), _integral_img the images integral image (optional), squared integral image (optional), 
	// templ is the template we are convolving with, templ_dfts it's dfts at varying windows sizes (optional),  _result - the output, method the type of convolution
	void matchTemplate_m( const cv::Mat_<float>& input_img, cv::Mat_<double>& img_dft, cv::Mat& _integral_img, cv::Mat& _integral_img_sq, const cv::Mat_<float>&  templ, map<int, cv::Mat_<double> >& templ_dfts, cv::Mat_<float>& result, int method );

	//===========================================================================
	// Point set and landmark manipulation functions
	//===========================================================================
	// Using Kabsch's algorithm for aligning shapes
	//This assumes that align_from and align_to are already mean normalised
	 cv::Matx22d AlignShapesKabsch2D(const cv::Mat_<double>& align_from, const cv::Mat_<double>& align_to );

	//=============================================================================
	// Basically Kabsch's algorithm but also allows the collection of points to be different in scale from each other
	DLL_EXPORT_CLMLANDMARK cv::Matx22d AlignShapesWithScale(cv::Mat_<double>& src, cv::Mat_<double> dst);

	//===========================================================================
	// Visualisation functions
	//===========================================================================
	void Project(cv::Mat_<double>& dest, const cv::Mat_<double>& mesh, double fx, double fy, double cx, double cy);
	void DLL_EXPORT_CLMLANDMARK DrawBox(cv::Mat image, cv::Vec6d pose, cv::Scalar color, int thickness, float fx, float fy, float cx, float cy);

	// Drawing face bounding box
	vector<std::pair<cv::Point2d, cv::Point2d>> DLL_EXPORT_CLMLANDMARK CalculateBox(cv::Vec6d pose, float fx, float fy, float cx, float cy);
	void DLL_EXPORT_CLMLANDMARK DrawBox(vector<pair<cv::Point, cv::Point>> lines, cv::Mat image, cv::Scalar color, int thickness);

	vector<cv::Point2d> DLL_EXPORT_CLMLANDMARK CalculateLandmarks(const cv::Mat_<double>& shape2D, cv::Mat_<int>& visibilities);
	vector<cv::Point2d> DLL_EXPORT_CLMLANDMARK CalculateLandmarks(CLNF& clnf_model);
	void DLL_EXPORT_CLMLANDMARK DrawLandmarks(cv::Mat img, vector<cv::Point> landmarks);

	void Draw(cv::Mat img, const cv::Mat_<double>& shape2D, const cv::Mat_<int>& visibilities);
	void Draw(cv::Mat img, const cv::Mat_<double>& shape2D);
	void Draw(cv::Mat img, const CLNF& clnf_model);


	//===========================================================================
	// Angle representation conversion helpers
	//===========================================================================
	DLL_EXPORT_CLMLANDMARK cv::Matx33d Euler2RotationMatrix(const cv::Vec3d& eulerAngles);

	// Using the XYZ convention R = Rx * Ry * Rz, left-handed positive sign
	cv::Vec3d RotationMatrix2Euler(const cv::Matx33d& rotation_matrix);

	cv::Vec3d Euler2AxisAngle(const cv::Vec3d& euler);

	cv::Vec3d AxisAngle2Euler(const cv::Vec3d& axis_angle);

	cv::Matx33d AxisAngle2RotationMatrix(const cv::Vec3d& axis_angle);

	cv::Vec3d RotationMatrix2AxisAngle(const cv::Matx33d& rotation_matrix);

	//============================================================================
	// Face detection helpers
	//============================================================================

	// Face detection using Haar cascade classifier
	
	
	// Face detection using HOG-SVM classifier
	
	bool DetectFacesHOG(vector<cv::Rect_<double> >& o_regions, const cv::Mat_<uchar>& intensity, 
		void * classifier, std::vector<double>& confidences);
	// The preference point allows for disambiguation if multiple faces are present (pick the closest one), if it is not set the biggest face is chosen
	bool DetectSingleFaceHOG(cv::Rect_<double>& o_region, const cv::Mat_<uchar>& intensity, 
		void *  classifier, double& confidence, const cv::Point preference = cv::Point(-1,-1));

	//============================================================================
	// Matrix reading functionality
	//============================================================================

	// Reading a matrix written in a binary format
	DLL_EXPORT_CLMLANDMARK	void ReadMatBin(std::ifstream& stream, cv::Mat &output_mat);

	// Reading in a matrix from a stream
	DLL_EXPORT_CLMLANDMARK void ReadMat(std::ifstream& stream, cv::Mat& output_matrix);

	// Skipping comments (lines starting with # symbol)
	void SkipComments(std::ifstream& stream);

}
#endif
