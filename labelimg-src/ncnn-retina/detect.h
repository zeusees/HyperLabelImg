#pragma once
#include "opencv2/core.hpp"
#if defined(NCNNLIBRARY_EXPORT) // inside DLL
#   define NCNNAPI   __declspec(dllexport)
#else // outside DLL
#   define NCNNAPI   __declspec(dllimport)
#endif  // XYZLIBRARY_EXPORT


NCNNAPI bool  ncnn_detect(cv::Mat img , cv::Rect_<double>& outF);