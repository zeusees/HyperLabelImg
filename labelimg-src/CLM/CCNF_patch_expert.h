/*
功能  节点响应图处理

作者:https://github.com/TadasBaltrusaitis/OpenFace



*/
#ifndef __CCNF_PATCH_EXPERT_h_
#define __CCNF_PATCH_EXPERT_h_

#include <opencv2/core/core.hpp>

#include <map>
#include <vector>

namespace LandmarkDetector
{

//===========================================================================
/** 
	A single Neuron response
*/
class CCNF_neuron{

public:

	// Type of patch (0=raw,1=grad,3=depth, other types besides raw are not actually used now)
	int     neuron_type; 

	// scaling of weights (needed as the energy of neuron might not be 1) 
	double  norm_weights; 

	// Weight bias
	double  bias;

	// Neural weights
	cv::Mat_<float> weights; 

	// can have neural weight dfts that are calculated on the go as needed, this allows us not to recompute
	// the dft of the template each time, improving the speed of tracking
	std::map<int, cv::Mat_<double> > weights_dfts;

	// the alpha associated with the neuron
	double alpha; 

	// Default constructor
	CCNF_neuron(){;}

	// Copy constructor
	CCNF_neuron(const CCNF_neuron& other);

	void Read(std::ifstream &stream);
	// The im_dft, integral_img, and integral_img_sq are precomputed images for convolution speedups (they get set if passed in empty values)
	void Response(cv::Mat_<float> &im, cv::Mat_<double> &im_dft, cv::Mat &integral_img, cv::Mat &integral_img_sq, cv::Mat_<float> &resp);

};

//===========================================================================
/**
A CCNF patch expert
*/
class CCNF_patch_expert{
public:
    
	// Width and height of the patch expert support region
	int width;
	int height;             
    
	// Collection of neurons for this patch expert
	std::vector<CCNF_neuron> neurons;

	// Information about the vertex features (association potentials)
	std::vector<int>				window_sizes;
	std::vector<cv::Mat_<float> >	Sigmas;
	std::vector<double>				betas;

	// How confident we are in the patch
	double   patch_confidence;

	// Default constructor
	CCNF_patch_expert(){;}
	bool ComputeFromFile(std::vector<cv::Mat_<float> > sigma_components, int scale,
		int viewID, int lmark, int window_size);
	// Copy constructor		
	CCNF_patch_expert(const CCNF_patch_expert& other);

	void Read(std::ifstream &stream, std::vector<int> window_sizes, std::vector<std::vector<cv::Mat_<float> > > sigma_components);

	// actual work (can pass in an image and a potential depth image, if the CCNF is trained with depth)
	void Response(cv::Mat_<float> &area_of_interest, cv::Mat_<float> &response);

	// Helper function to compute relevant sigmas
	void ComputeSigmas(std::vector<cv::Mat_<float> > sigma_components, int window_size);
	
};
  //===========================================================================
}
#endif
