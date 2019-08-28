/*
功能  全局参数

作者:https://github.com/TadasBaltrusaitis/OpenFace

版权：

*/
//  Parameters of the CLNF, CLM-Z and CLM trackers
#ifndef __LANDMARK_DETECTOR_PARAM_H
#define __LANDMARK_DETECTOR_PARAM_H

#include <vector>

using namespace std;

#ifdef THIS_CLM_LANDMARK
#define DLL_EXPORT_CLMLANDMARK __declspec(dllexport)
#else
#define DLL_EXPORT_CLMLANDMARK
#endif
namespace LandmarkDetector
{

struct DLL_EXPORT_CLMLANDMARK FaceModelParameters
{

	// A number of RLMS or NU-RLMS iterations
	int num_optimisation_iteration;
	
	// Should pose be limited to 180 degrees frontal
	bool limit_pose;
	
	// Should face validation be done
	bool validate_detections;

	// Landmark detection validator boundary for correct detection, the regressor output -1 (perfect alignment) 1 (bad alignment), 
	double validation_boundary;

	// Used when tracking is going well
	vector<int> window_sizes_small;

	// Used when initialising or tracking fails
	vector<int> window_sizes_init;
	
	// Used for the current frame
	vector<int> window_sizes_current;
	
	// How big is the tracking template that helps with large motions
	double face_template_scale;	
	bool use_face_template;

	// Where to load the model from
	string model_location;
	
	// this is used for the smooting of response maps (KDE sigma)
	double sigma;

	double reg_factor;	// weight put to regularisation
	double weight_factor; // factor for weighted least squares

	// should multiple views be considered during reinit
	bool multi_view;
	
	// How often should face detection be used to attempt reinitialisation, every n frames (set to negative not to reinit)
	int reinit_video_every;

	// Determining which face detector to use for (re)initialisation, HAAR is quicker but provides more false positives and is not goot for in-the-wild conditions
	// Also HAAR detector can detect smaller faces while HOG SVM is only capable of detecting faces at least 70px across
	enum FaceDetector{HAAR_DETECTOR, HOG_SVM_DETECTOR};

	string face_detector_location;
	FaceDetector curr_face_detector;

	// Should the results be visualised and reported to console
	bool quiet_mode;

	// Should the model be refined hierarchically (if available)
	bool refine_hierarchical;

	// Should the parameters be refined for different scales
	bool refine_parameters;

	// Using the brand new and experimental gaze tracker
	bool track_gaze;

	FaceModelParameters();

	FaceModelParameters(vector<string> &arguments);

	private:
		void init();
};

}

#endif // __LANDMARK_DETECTOR_PARAM_H
