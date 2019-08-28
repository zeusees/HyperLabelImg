

#include "clmInter.h"
#include "LandmarkCoreIncludes.h"
#include "SpecialEfect/SpecialEfect.h"
#include "opencvFaceDetect.h"
#include "LandMarkUpdate.h"
#include "ncnn-retina\detect.h"
#ifdef WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "LandmarkDetectorModel.h"
#include "Common/gLobalInfo.h"

using namespace LandmarkDetector;

using namespace std;
using namespace cv;

float fx = 640, fy = 480, cx = 320, cy = 240;


Mat clmResult;
LandmarkDetector::FaceModelParameters * clm_parameters;
LandmarkDetector::CLNF * clm_model = 0;





bool BisInit();
//
void DoCompute();

void DoComputeExpert();
vector<string> get_arguments(int argc, char **argv)
{

	vector<string> arguments;

	for(int i = 0; i < argc; ++i)
	{
		arguments.push_back(string(argv[i]));
	}
	return arguments;
}

bool BisInit()
{
	if(clm_model != NULL)
	{
		return true;
	}

	return false;
}
void CLM_CalcParams(cv::Vec6d& out_params_global, cv::Mat_<double>& out_params_local,
	const cv::Mat_<double>& landmark_locations)
{

	clm_model->CLM_CalcParams(out_params_global, out_params_local, landmark_locations);
}

void CLM_GetParamGlobal(cv::Vec6d& out_params_global)
{
	out_params_global = clm_model->params_global;
}

cv::Mat_<double> CLM_Get2d_3MarkNew(std::vector<cv::Point> pts68)
{

	cv::Mat_<double> landmark_locations = cv::Mat_<double>(68*2,1);
	for (int k = 0; k < 68; ++k)
	{
		((double*)landmark_locations.data)[k] = pts68[k].x;
		((double*)landmark_locations.data)[k+68] = pts68[k].y;
	}
	
	cv::Vec6d out_params_global;
	cv::Mat_<double> out_params_local=cv::Mat_<double>(clm_model->pdm.NumberOfModes(), 1, 0.0);

	clm_model->pdm2.CalcParams(out_params_global, out_params_local, landmark_locations);

	
	cv::Mat_<double> landmarks_3D;
	clm_model->pdm.CalcShape2DMY(landmarks_3D, out_params_local, out_params_global);

	return landmarks_3D;
}
cv::Mat_<double> CLM_Get2d_3Mark()
{
	cv::Mat_<double> landmarks_3D;
	clm_model->pdm.CalcShape2DMY(landmarks_3D, clm_model->params_local, clm_model->params_global);
	return landmarks_3D;
}

Mat_<double> CLM_Get3DLandMark()
{
	// 3D points
	cv::Mat_<double> landmarks_3D;
	clm_model->pdm.CalcShape3D(landmarks_3D, clm_model->params_local);
	return landmarks_3D;

}

LandmarkDetector::CLNF * GetCLNF()
{
	return clm_model;
}
bool DoDetectMarksInVideo(cv::Mat& inImg)
{
	Mat grayscale_image;
	if(inImg.type() != CV_8UC1)
	{
		cvtColor(inImg,grayscale_image,CV_BGR2GRAY);
	}
	else
	{
		grayscale_image = inImg.clone();
	}

	Mat depth_image;
	clm_parameters->validate_detections = true;
	bool detection_success = LandmarkDetector::DetectLandmarksInVideo(grayscale_image, depth_image, *clm_model, *clm_parameters);	
	bool bHi = clm_parameters->refine_hierarchical;
	
	
	clmResult = inImg;
	return detection_success;
}


void create_display_image(const Mat& orig, Mat& display_image, LandmarkDetector::CLNF& clm_model)
{
	// preparing the visualisation image
	display_image = orig.clone();		

	// Creating a display image			
	Mat xs = clm_model.detected_landmarks(Rect(0, 0, 1, clm_model.detected_landmarks.rows/2));
	Mat ys = clm_model.detected_landmarks(Rect(0, clm_model.detected_landmarks.rows/2, 1, clm_model.detected_landmarks.rows/2));
	double min_x, max_x, min_y, max_y;

	cv::minMaxLoc(xs, &min_x, &max_x);
	cv::minMaxLoc(ys, &min_y, &max_y);

	double width = max_x - min_x;
	double height = max_y - min_y;

	int minCropX = max((int)(min_x-width/3.0),0);
	int minCropY = max((int)(min_y-height/3.0),0);

	int widthCrop = min((int)(width*5.0/3.0), display_image.cols - minCropX - 1);
	int heightCrop = min((int)(height*5.0/3.0), display_image.rows - minCropY - 1);

	double scaling = 400.0/widthCrop;
	
	// first crop the image
	//display_image = display_image(Rect((int)(minCropX), (int)(minCropY), (int)(widthCrop), (int)(heightCrop)));
		
	// now scale it
	//cv::resize(display_image.clone(), display_image, Size(), scaling, scaling);

	// Make the adjustments to points
	//xs = (xs - minCropX)*scaling;
	//ys = (ys - minCropY)*scaling;

	Mat shape = clm_model.detected_landmarks.clone();

	//xs.copyTo(shape(Rect(0, 0, 1, clm_model.detected_landmarks.rows/2)));
	//ys.copyTo(shape(Rect(0, clm_model.detected_landmarks.rows/2, 1, clm_model.detected_landmarks.rows/2)));
	//LandmarkDetector::Draw(display_image, clm_model);				
}

void DoCompute()
{
	clm_model->PreComputeMeanshift_KDE();

}






Mat GetMainSrcMat()
{
	return clmResult.clone();
}


bool DoDetectMarksInPic(Mat& read_image)
{
		clmResult  = read_image;
		// Making sure the image is in uchar grayscale
		Mat_<uchar> grayscale_image;		
		cvtColor(read_image,grayscale_image,CV_BGR2GRAY);
		// Detect faces in an image
		Rect_<double>  face_detection;
		//int t1 = GetTickCount();
		bool bDetectResult = false;
		if (ncnn_detect(read_image, face_detection))
		{
			bDetectResult = DoGetMarks(grayscale_image, face_detection);
		}

		return bDetectResult;

}

bool DoGetMarks(Mat& grayscale_image,Rect_<double> & face_detections)
{
	bool bResult = false;
	// Loading depth file if exists (optional)
	Mat_<float> depth_image;
	
	// if there are multiple detections go through them
	bResult = LandmarkDetector::DetectLandmarksInImage(grayscale_image, depth_image,
				face_detections,
						*clm_model, *clm_parameters);
	if (bResult)
	{
		//锟斤拷取锟截硷拷锟斤拷
		vector<Point2f> mainPts = GetMainPts();
	
		//加验证
		Mat dst = clmResult;

		int disWEyes = abs(mainPts[42].x - mainPts[36].x) / 2;
		int topPosY = mainPts[27].y - disWEyes*2.5;
		/*if (mainPts[0].x < dst.cols && mainPts[0].x >= 0 && mainPts[0].y < dst.rows && mainPts[0].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[2].x < dst.cols && mainPts[2].x >= 0 && mainPts[2].y < dst.rows && mainPts[2].y >= 0
			&& mainPts[3].x < dst.cols && mainPts[3].x >= 0 && mainPts[3].y < dst.rows && mainPts[3].y >= 0
			&& mainPts[8].x < dst.cols && mainPts[8].x >= 0 && mainPts[8].y < dst.rows && mainPts[8].y >= 0
			&& mainPts[14].x < dst.cols && mainPts[14].x >= 0 && mainPts[14].y < dst.rows && mainPts[14].y >= 0
			&& mainPts[15].x < dst.cols && mainPts[15].x >= 0 && mainPts[15].y < dst.rows && mainPts[15].y >= 0
			&& mainPts[16].x < dst.cols && mainPts[16].x >= 0 && mainPts[16].y < dst.rows && mainPts[16].y >= 0

			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& mainPts[1].x < dst.cols && mainPts[1].x >= 0 && mainPts[1].y < dst.rows && mainPts[1].y >= 0
			&& topPosY > 0)
		{
			bResult = true;
		}*/

	}
	
	return bResult;
}



void ReleaseCLM()
{
	if(clm_model != NULL)
	{		

		clmResult.release();

		//clm_model->ReleaseBuf();

		delete clm_model;
		clm_model = NULL;

		delete clm_parameters; 
		clm_parameters = NULL;
	}
}

std::vector<cv::Point2f> GetMainPts()
{
	std::vector<cv::Point2f> tmpPts;
	for (int i = 0; i < clm_model->detected_landmarks.rows/2; ++i)
	{

		cv::Point2f featurePoint(clm_model->detected_landmarks.at<double>(i),
			clm_model->detected_landmarks.at<double>(i + clm_model->detected_landmarks.rows / 2));
		tmpPts.push_back(featurePoint);

	}

	return tmpPts;
}


//锟斤拷前 锟斤拷一些锟斤拷锟捷斤拷锟斤拷 锟斤拷锟斤拷
void DoComputeExpert()
{
	clm_model->patch_experts.DoCompute();
}

bool CLM_DoInit()
{
#ifdef WIN32
	_mkdir("./model/patch_cal_result");
#else
	mkdir("./model/patch_cal_result",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	if(!BisInit())
	{
		#ifdef ANDROID
		//	LOGI("***************************************	DoInit");
		#endif
			//Convert arguments to more convenient vector form
			vector<string> arguments;
			//arguments.push_back("-f");
			//arguments.push_back(g_rootpath+"videos/1.jpg");

			 clm_parameters = new  LandmarkDetector::FaceModelParameters(arguments);

			//判断文件路径是否存在
			if(! ISFileExist((char*)clm_parameters->model_location.c_str()))
			{
				delete clm_parameters;
				clm_parameters = NULL;
				return false;
			}
			 clm_model = new LandmarkDetector::CLNF(clm_parameters->model_location);

			// Some initial parameters that can be overriden from command line
			vector<string> files, depth_files, output_images, output_landmark_locations;

			// Bounding boxes for a face in each image (optional)
			vector<Rect_<double> > bounding_boxes;

			//LandmarkDetector::get_image_input_output_params(files, depth_files, output_images, arguments);

			// No need to validate detections, as we're not doing tracking
			clm_parameters->validate_detections = false;


		


		
		#ifdef ANDROID
			LOGI("***************************************	classifier.load");
		#else
			_mkdir("./model/patch_cal_result");
			//DoCompute();
			//DoComputeExpert();
		#endif
	}

	return true;
}







