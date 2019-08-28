
#include "Common/gLobalInfo.h"

#include "LandmarkDetectorParameters.h"
#include "Common\gLobalInfo.h"
// Boost includes

// System includes
#include <sstream>
#include <iostream>

using namespace std;

using namespace LandmarkDetector;

FaceModelParameters::FaceModelParameters()
{
	// initialise the default values
	init();
}

FaceModelParameters::FaceModelParameters(vector<string> &arguments)
{
	// initialise the default values
	init();

	// First element is reserved for the executable location (useful for finding relative model locs)
	//boost::filesystem::path root = boost::filesystem::path(arguments[0]).parent_path();

	bool* valid = new bool[arguments.size()];
	valid[0] = true;

	for (size_t i = 1; i < arguments.size(); ++i)
	{
		valid[i] = true;

		if (arguments[i].compare("-mloc") == 0)
		{
			string model_loc = arguments[i + 1];
			model_location = model_loc;
			valid[i] = false;
			valid[i + 1] = false;
			i++;

		}
		if (arguments[i].compare("-sigma") == 0)
		{
			stringstream data(arguments[i + 1]);
			data >> sigma;
			valid[i] = false;
			valid[i + 1] = false;
			i++;
		}
		else if (arguments[i].compare("-w_reg") == 0)
		{
			stringstream data(arguments[i + 1]);
			data >> weight_factor;
			valid[i] = false;
			valid[i + 1] = false;
			i++;
		}
		else if (arguments[i].compare("-reg") == 0)
		{
			stringstream data(arguments[i + 1]);
			data >> reg_factor;
			valid[i] = false;
			valid[i + 1] = false;
			i++;
		}
		else if (arguments[i].compare("-multi_view") == 0)
		{

			stringstream data(arguments[i + 1]);
			int m_view;
			data >> m_view;

			multi_view = (bool)(m_view != 0);
			valid[i] = false;
			valid[i + 1] = false;
			i++;
		}
		else if (arguments[i].compare("-validate_detections") == 0)
		{
			stringstream data(arguments[i + 1]);
			int v_det;
			data >> v_det;

			validate_detections = (bool)(v_det != 0);
			valid[i] = false;
			valid[i + 1] = false;
			i++;
		}
		else if (arguments[i].compare("-n_iter") == 0)
		{
			stringstream data(arguments[i + 1]);
			data >> num_optimisation_iteration;

			valid[i] = false;
			valid[i + 1] = false;
			i++;
		}
		else if (arguments[i].compare("-gaze") == 0)
		{
			track_gaze = true;

			valid[i] = false;
			i++;
		}
		else if (arguments[i].compare("-q") == 0)
		{

			quiet_mode = true;

			valid[i] = false;
		}
		else if (arguments[i].compare("-wild") == 0)
		{
			// For in the wild fitting these parameters are suitable
			window_sizes_init = vector<int>(4);
			window_sizes_init[0] = 15; window_sizes_init[1] = 13; window_sizes_init[2] = 11; window_sizes_init[3] = 9;

			sigma = 1.25;
			reg_factor = 35;
			weight_factor = 2.5;
			num_optimisation_iteration = 7;

			valid[i] = false;

			// For in-the-wild images use an in-the wild detector				
			curr_face_detector = HOG_SVM_DETECTOR;

		}
	}

	for (int i = (int)arguments.size() - 1; i >= 0; --i)
	{
		if (!valid[i])
		{
			arguments.erase(arguments.begin() + i);
		}
	}

	//// Make sure model_location is valid
	//if (!boost::filesystem::exists(boost::filesystem::path(model_location)))
	//{

	//	face_detector_location = (root / face_detector_location).string();
	//	model_location = (root / model_location).string();
	//	if (!boost::filesystem::exists(boost::filesystem::path(model_location)))
	//	{
	//		std::cout << "Could not find the landmark detection model to load" << std::endl;
	//	}
	//}

}

void FaceModelParameters::init()
{

	// number of iterations that will be performed at each scale
	num_optimisation_iteration = 5;

	// using an external face checker based on SVM
	validate_detections = true;

	// Using hierarchical refinement by default (can be turned off)
	refine_hierarchical = true;

	// Refining parameters by default
	refine_parameters = true;

	window_sizes_small = vector<int>(4);
	window_sizes_init = vector<int>(4);

	// For fast tracking
	window_sizes_small[0] = 0;
	window_sizes_small[1] = 9;
	window_sizes_small[2] = 7;
	window_sizes_small[3] = 5;

	// Just for initialisation
	window_sizes_init.at(0) = 11;
	window_sizes_init.at(1) = 9;
	window_sizes_init.at(2) = 7;
	window_sizes_init.at(3) = 5;

	face_template_scale = 0.3;
	// Off by default (as it might lead to some slight inaccuracies in slowly moving faces)
	use_face_template = false;

	// For first frame use the initialisation
	window_sizes_current = window_sizes_init;

	model_location = g_rootpath + std::string("model/main_clnf_general.txt");
	//main_clnf_general.txt
	sigma = 1.5;
	reg_factor = 25;
	weight_factor = 0; // By default do not use NU-RLMS for videos as it does not work as well for them

	validation_boundary = -0.45;

	limit_pose = true;
	multi_view = true;

	reinit_video_every = 4;

	// Face detection
#if OS_UNIX
	face_detector_location = "classifiers/haarcascade_frontalface_alt.xml";
#else
	face_detector_location = g_rootpath + std::string("classifiers/haarcascade_frontalface_alt2.xml");
#endif

	quiet_mode = false;
	// By default use HOG SVM
	curr_face_detector = HAAR_DETECTOR;

	FILE* fp = fopen("./clm.cfg","r");
	if (fp)
	{
		char szBuf[128];
		fread(szBuf,1,sizeof(szBuf),fp);
		string str = szBuf;
		if (str.compare("HAARDETECT") != 0)
		{
			curr_face_detector = HOG_SVM_DETECTOR;
		}
		
		fclose(fp);
	}
	else
	{
		fp = fopen("./clm.cfg", "w+");
		fwrite("HAARDETECT",1,sizeof("HAARDETECT"),fp);
		fclose(fp);
	}


	// The gaze tracking has to be explicitly initialised
	track_gaze = false;
}

