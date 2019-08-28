
#include "Common/gLobalInfo.h"
#include <fstream>
#include "CCNF_patch_expert.h"

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

// Local includes
#include "LandmarkDetectorUtils.h"

using namespace LandmarkDetector;

// Copy constructors of neuron and patch expert
CCNF_neuron::CCNF_neuron(const CCNF_neuron& other) : weights(other.weights.clone())
{
	this->neuron_type = other.neuron_type;
	this->norm_weights = other.norm_weights;
	this->bias = other.bias;
	this->alpha = other.alpha;

	for (std::map<int, cv::Mat_<double> >::const_iterator it = other.weights_dfts.begin(); it != other.weights_dfts.end(); it++)
	{
		// Make sure the matrix is copied.
		this->weights_dfts.insert(std::pair<int, cv::Mat>(it->first, it->second.clone()));
	}
}

// Copy constructor		
CCNF_patch_expert::CCNF_patch_expert(const CCNF_patch_expert& other) : neurons(other.neurons), window_sizes(other.window_sizes), betas(other.betas)
{
	this->width = other.width;
	this->height = other.height;
	this->patch_confidence = other.patch_confidence;

	// Copy the Sigmas in a deep way
	for (std::vector<cv::Mat_<float> >::const_iterator it = other.Sigmas.begin(); it != other.Sigmas.end(); it++)
	{
		// Make sure the matrix is copied.
		this->Sigmas.push_back(it->clone());
	}

}


bool CCNF_patch_expert::ComputeFromFile(std::vector<cv::Mat_<float> > sigma_components, int scale, 
	int viewID, int lmark, int window_size)
{
	//
	for (size_t i = 0; i < window_sizes.size(); ++i)
	{
		if (window_sizes[i] == window_size)
			return true;
	}
	
	////判断指定文件是否存在,计算记过直接从文件获得
	//char szBuf[512];
	//sprintf(szBuf, "model/patch_cal_result/%d_%d_%d_%d.bin",
	//	scale, viewID, lmark, window_size);
	//string filename = std::string(g_rootpath) + szBuf;
	//if (ISFileExist((char*)filename.c_str()))
	//{
	//	FILE * fRead = fopen(filename.c_str(), "rb");
	//	cv::Mat_<float> Sigma_f = cv::Mat::ones(window_size*window_size, window_size*window_size, CV_32F);
	//	int rLen = fread(Sigma_f.data, 1, Sigma_f.step[0] * Sigma_f.rows, fRead);
	//	fclose(fRead);

	//	window_sizes.push_back(window_size);
	//	Sigmas.push_back(Sigma_f);
	//	return true;
	//}
	//else
	{
		// Each of the landmarks will have the same connections, hence constant number of sigma components
		int n_betas = sigma_components.size();

		// calculate the sigmas based on alphas and betas
		float sum_alphas = 0;

		int n_alphas = this->neurons.size();

		// sum the alphas first
		for (int a = 0; a < n_alphas; ++a)
		{
			sum_alphas = sum_alphas + this->neurons[a].alpha;
		}

		cv::Mat_<float> q1 = sum_alphas * cv::Mat_<float>::eye(window_size*window_size, window_size*window_size);

		cv::Mat_<float> q2 = cv::Mat_<float>::zeros(window_size*window_size, window_size*window_size);
		for (int b = 0; b < n_betas; ++b)
		{
			q2 = q2 + ((float)this->betas[b]) * sigma_components[b];
		}

		cv::Mat_<float> SigmaInv = 2 * (q1 + q2);

		cv::Mat Sigma_f;
		cv::invert(SigmaInv, Sigma_f, cv::DECOMP_CHOLESKY);

		window_sizes.push_back(window_size);
		Sigmas.push_back(Sigma_f);

		////对计算结果进行保存成文件，第二次计算直接从文件读取；
		//char szBuf[512];
		//sprintf(szBuf, "model/patch_cal_result/%d_%d_%d_%d.bin",
		//	scale, viewID, lmark, window_size);
		//string pathname;
		//pathname = std::string(g_rootpath) + szBuf;
		//FILE* pfile = fopen(pathname.c_str(), "wb");
		//fwrite(Sigma_f.data, 1, Sigma_f.step[0] * Sigma_f.rows, pfile);
		//fclose(pfile);

		//int sMa = sizeof(Sigma_f);
		//int a = 1;
		return false;
	}
}


// Compute sigmas for all landmarks for a particular view and window size
void CCNF_patch_expert::ComputeSigmas(std::vector<cv::Mat_<float> > sigma_components, int window_size)
{
	for(size_t i=0; i < window_sizes.size(); ++i)
	{
		if( window_sizes[i] == window_size)
			return;
	}
	// Each of the landmarks will have the same connections, hence constant number of sigma components
	int n_betas = sigma_components.size();

	// calculate the sigmas based on alphas and betas
	float sum_alphas = 0;

	int n_alphas = this->neurons.size();

	// sum the alphas first
	for(int a = 0; a < n_alphas; ++a)
	{
		sum_alphas = sum_alphas + this->neurons[a].alpha;
	}

	cv::Mat_<float> q1 = sum_alphas * cv::Mat_<float>::eye(window_size*window_size, window_size*window_size);

	cv::Mat_<float> q2 = cv::Mat_<float>::zeros(window_size*window_size, window_size*window_size);
	for (int b=0; b < n_betas; ++b)
	{			
		q2 = q2 + ((float)this->betas[b]) * sigma_components[b];
	}

	cv::Mat_<float> SigmaInv = 2 * (q1 + q2);
	
	cv::Mat Sigma_f;
	cv::invert(SigmaInv, Sigma_f, cv::DECOMP_CHOLESKY);

	window_sizes.push_back(window_size);
	Sigmas.push_back(Sigma_f);

}

//===========================================================================
void CCNF_neuron::Read(ifstream &stream)
{
	// Sanity check
	int read_type;
	stream.read ((char*)&read_type, 4);
	assert(read_type == 2);

	stream.read ((char*)&neuron_type, 4);
	stream.read ((char*)&norm_weights, 8);
	stream.read ((char*)&bias, 8);
	stream.read ((char*)&alpha, 8);
	
	LandmarkDetector::ReadMatBin(stream, weights); 

}

//===========================================================================
void CCNF_neuron::Response(cv::Mat_<float> &im, cv::Mat_<double> &im_dft, cv::Mat &integral_img, cv::Mat &integral_img_sq, cv::Mat_<float> &resp)
{

	int h = im.rows - weights.rows + 1;
	int w = im.cols - weights.cols + 1;
	
	// the patch area on which we will calculate reponses
	cv::Mat_<float> I;

	if(neuron_type == 3)
	{
		// Perform normalisation across whole patch (ignoring the invalid values indicated by <= 0

		cv::Scalar mean;
		cv::Scalar std;
		
		// ignore missing values
		cv::Mat_<uchar> mask = im > 0;
		cv::meanStdDev(im, mean, std, mask);

		// if all values the same don't divide by 0
		if(std[0] != 0)
		{
			I = (im - mean[0]) / std[0];
		}
		else
		{
			I = (im - mean[0]);
		}

		I.setTo(0, mask == 0);
	}
	else
	{
		if(neuron_type == 0)
		{
			I = im;
		}
		else
		{
			printf("ERROR(%s,%d): Unsupported patch type %d!\n", __FILE__,__LINE__,neuron_type);
			abort();
		}
	}
  
	if(resp.empty())
	{		
		resp.create(h, w);
	}

	// The response from neuron before activation
	if(neuron_type == 3)
	{
		// In case of depth we use per area, rather than per patch normalisation
		matchTemplate_m(I, im_dft, integral_img, integral_img_sq, weights, weights_dfts, resp, CV_TM_CCOEFF); // the linear multiplication, efficient calc of response
	}
	else
	{
		matchTemplate_m(I, im_dft, integral_img, integral_img_sq, weights, weights_dfts, resp, CV_TM_CCOEFF_NORMED); // the linear multiplication, efficient calc of response
	}

	cv::MatIterator_<float> p = resp.begin();

	cv::MatIterator_<float> q1 = resp.begin(); // respone for each pixel
	cv::MatIterator_<float> q2 = resp.end();

	// the logistic function (sigmoid) applied to the response
	while(q1 != q2)
	{
		*p++ = (2 * alpha) * 1.0 /(1.0 + exp( -(*q1++ * norm_weights + bias )));
	}

}

//===========================================================================
void CCNF_patch_expert::Read(ifstream &stream, std::vector<int> window_sizes, std::vector<std::vector<cv::Mat_<float> > > sigma_components)
{

	// Sanity check
	int read_type;

	stream.read ((char*)&read_type, 4);
	assert(read_type == 5);

	// the number of neurons for this patch
	int num_neurons;
	stream.read ((char*)&width, 4);
	stream.read ((char*)&height, 4);
	stream.read ((char*)&num_neurons, 4);

	if(num_neurons == 0)
	{
		// empty patch due to landmark being invisible at that orientation
	
		// read an empty int (due to the way things were written out)
		stream.read ((char*)&num_neurons, 4);
		return;
	}

	neurons.resize(num_neurons);
	for(int i = 0; i < num_neurons; i++)
		neurons[i].Read(stream);

	int n_sigmas = window_sizes.size();

	int n_betas = 0;

	if(n_sigmas > 0)
	{
		n_betas = sigma_components[0].size();

		betas.resize(n_betas);

		for (int i=0; i < n_betas;  ++i)
		{
			stream.read ((char*)&betas[i], 8);
		}
	}	

	// Read the patch confidence
	stream.read ((char*)&patch_confidence, 8);

}

//===========================================================================
void CCNF_patch_expert::Response(cv::Mat_<float> &area_of_interest, cv::Mat_<float> &response)
{
	
	int response_height = area_of_interest.rows - height + 1;
	int response_width = area_of_interest.cols - width + 1;

	if(response.rows != response_height || response.cols != response_width)
	{
		response.create(response_height, response_width);
	}
		
	response.setTo(0);
	
	// the placeholder for the DFT of the image, the integral image, and squared integral image so they don't get recalculated for every response
	cv::Mat_<double> area_of_interest_dft;
	cv::Mat integral_image, integral_image_sq;
	
	cv::Mat_<float> neuron_response;

	// responses from the neural layers
	for(size_t i = 0; i < neurons.size(); i++)
	{		
		// Do not bother with neuron response if the alpha is tiny and will not contribute much to overall result
		if(neurons[i].alpha > 1e-4)
		{
			neurons[i].Response(area_of_interest, area_of_interest_dft, integral_image, integral_image_sq, neuron_response);
			response = response + neuron_response;						
		}
	}

	int s_to_use = -1;

	// Find the matching sigma
	for(size_t i=0; i < window_sizes.size(); ++i)
	{
		if(window_sizes[i] == response_height)
		{
			// Found the correct sigma
			s_to_use = i;			
			break;
		}
	}

	cv::Mat_<float> resp_vec_f = response.reshape(1, response_height * response_width);

	cv::Mat out = Sigmas[s_to_use] * resp_vec_f;
	
	response = out.reshape(1, response_height);

	// Making sure the response does not have negative numbers
	double min;

	minMaxIdx(response, &min, 0);
	if(min < 0)
	{
		response = response - min;
	}

}
