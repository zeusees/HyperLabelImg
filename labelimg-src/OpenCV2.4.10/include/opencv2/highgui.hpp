/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef __OPENCV_HIGHGUI_HPP__
#define __OPENCV_HIGHGUI_HPP__

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui_c.h"

#ifdef __cplusplus

struct CvCapture;
struct CvVideoWriter;

namespace cv
{

enum {
    // Flags for namedWindow
    WINDOW_NORMAL   = CV_WINDOW_NORMAL,   // the user can resize the window (no constraint) / also use to switch a fullscreen window to a normal size
    WINDOW_AUTOSIZE = CV_WINDOW_AUTOSIZE, // the user cannot resize the window, the size is constrainted by the image displayed
    WINDOW_OPENGL   = CV_WINDOW_OPENGL,   // window with opengl support

    // Flags for set / getWindowProperty
    WND_PROP_FULLSCREEN   = CV_WND_PROP_FULLSCREEN,  // fullscreen property
    WND_PROP_AUTOSIZE     = CV_WND_PROP_AUTOSIZE,    // autosize property
    WND_PROP_ASPECT_RATIO = CV_WND_PROP_ASPECTRATIO, // window's aspect ration
    WND_PROP_OPENGL       = CV_WND_PROP_OPENGL       // opengl support
};





enum
{
    EVENT_MOUSEMOVE      =0,
    EVENT_LBUTTONDOWN    =1,
    EVENT_RBUTTONDOWN    =2,
    EVENT_MBUTTONDOWN    =3,
    EVENT_LBUTTONUP      =4,
    EVENT_RBUTTONUP      =5,
    EVENT_MBUTTONUP      =6,
    EVENT_LBUTTONDBLCLK  =7,
    EVENT_RBUTTONDBLCLK  =8,
    EVENT_MBUTTONDBLCLK  =9
};

enum
{
    EVENT_FLAG_LBUTTON   =1,
    EVENT_FLAG_RBUTTON   =2,
    EVENT_FLAG_MBUTTON   =4,
    EVENT_FLAG_CTRLKEY   =8,
    EVENT_FLAG_SHIFTKEY  =16,
    EVENT_FLAG_ALTKEY    =32
};



//-------------------------

enum
{
    // 8bit, color or not
    IMREAD_UNCHANGED  =-1,
    // 8bit, gray
    IMREAD_GRAYSCALE  =0,
    // ?, color
    IMREAD_COLOR      =1,
    // any depth, ?
    IMREAD_ANYDEPTH   =2,
    // ?, any color
    IMREAD_ANYCOLOR   =4
};

enum
{
    IMWRITE_JPEG_QUALITY =1,
    IMWRITE_PNG_COMPRESSION =16,
    IMWRITE_PNG_STRATEGY =17,
    IMWRITE_PNG_BILEVEL =18,
    IMWRITE_PNG_STRATEGY_DEFAULT =0,
    IMWRITE_PNG_STRATEGY_FILTERED =1,
    IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY =2,
    IMWRITE_PNG_STRATEGY_RLE =3,
    IMWRITE_PNG_STRATEGY_FIXED =4,
    IMWRITE_PXM_BINARY =32
};

CV_EXPORTS_W Mat imread( const string& filename, int flags=1 );
CV_EXPORTS_W bool imwrite( const string& filename, InputArray img,
              const vector<int>& params=vector<int>());
CV_EXPORTS_W Mat imdecode( InputArray buf, int flags );
CV_EXPORTS Mat imdecode( InputArray buf, int flags, Mat* dst );
CV_EXPORTS_W bool imencode( const string& ext, InputArray img,
                            CV_OUT vector<uchar>& buf,
                            const vector<int>& params=vector<int>());

#ifndef CV_NO_VIDEO_CAPTURE_CPP_API






#endif

}

#endif

#endif
