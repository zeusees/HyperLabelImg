/*
功能  SVR响应

作者:https://github.com/TadasBaltrusaitis/OpenFace

版权：

*/

#ifndef __SVR_PATCH_EXPERT_h_
#define __SVR_PATCH_EXPERT_h_

// system includes
#include <map>

// OpenCV includes
#include <opencv2/core/core.hpp>

namespace LandmarkDetector
{
  //===========================================================================
  /** 
      The classes describing the SVR patch experts
  */

class SVR_patch_expert{
	public:

		// Type of data the patch expert operated on (0=raw, 1=grad)
		int     type;					

		// Logistic regression slope
		double  scaling;
		
		// Logistic regression bias
		double  bias;

		// Support vector regression weights
		cv::Mat_<float> weights;

		// Discrete Fourier Transform of SVR weights, precalculated for speed (at different window sizes)
		std::map<int, cv::Mat_<double> > weights_dfts;

		// Confidence of the current patch expert (used for NU_RLMS optimisation)
		double  confidence;

		SVR_patch_expert(){;}
		
		// A copy constructor
		SVR_patch_expert(const SVR_patch_expert& other);

		// Reading in the patch expert
		void Read(std::ifstream &stream);

		// The actual response computation from intensity or depth (for CLM-Z)
		void Response(const cv::Mat_<float> &area_of_interest, cv::Mat_<float> &response);
		void ResponseDepth(const cv::Mat_<float> &area_of_interest, cv::Mat_<float> &response);

};
//===========================================================================
/**
    A Multi-patch Expert that can include different patch types. Raw pixel values or image gradients
*/
class Multi_SVR_patch_expert{
	public:
		
		// Width and height of the patch expert support area
		int width;
		int height;						

		// Vector of all of the patch experts (different modalities) for this particular Multi patch expert
		std::vector<SVR_patch_expert> svr_patch_experts;	

		// Default constructor
		Multi_SVR_patch_expert(){;}
	
		// Copy constructor				
		Multi_SVR_patch_expert(const Multi_SVR_patch_expert& other);

		void Read(std::ifstream &stream);

		// actual response computation from intensity of depth (for CLM-Z)
		void Response(const cv::Mat_<float> &area_of_interest, cv::Mat_<float> &response);
		void ResponseDepth(const cv::Mat_<float> &area_of_interest, cv::Mat_<float> &response);

};
}
#endif
