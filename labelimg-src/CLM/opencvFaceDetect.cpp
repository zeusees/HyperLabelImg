#include "opencvFaceDetect.h"
#include <opencv2/imgproc.hpp>
#include <afxwin.h>
#ifdef WIN32

#include "facedetect-dll.h"
#endif

#define DETECT_BUFFER_SIZE 0xC004
using namespace cv;
using namespace std;

namespace FaceDetector
{


	bool DetectFaceSingleYU(cv::Rect_<double> & o_regions,cv::Mat inImgBGR)
	{
		return ncnn_detect()
	}

	



}
