#include "anchor_generator.h"
#include "config.h"
#include "tools.h"
#include <time.h>
#include <opencv2\imgproc.hpp>
#include "detect.h"
ncnn::Net *  _net = NULL;
 bool  ncnn_detect(cv::Mat img, cv::Rect_<double>& outF)
{
    extern float pixel_mean[3];
    extern float pixel_std[3];

	if (_net == NULL)
	{
		std::string param_path = "./models/retina.param";
		std::string bin_path = "./models/retina.bin";

		_net = new ncnn::Net;
		_net->load_param(param_path.data());
		_net->load_model(bin_path.data());

	}

	cv::Mat imgSrc = img.clone();
    if(!img.data)
    	printf("load error");

	int span = 3;

	ncnn::Mat input = ncnn::Mat::from_pixels_resize(imgSrc.data, ncnn::Mat::PIXEL_BGR2RGB, img.cols, img.rows, img.cols/ span, img.rows/ span);
    cv::resize(img, imgSrc, cv::Size(img.cols / span, img.rows / span));

    input.substract_mean_normalize(pixel_mean, pixel_std);
	ncnn::Extractor _extractor = _net->create_extractor();

	clock_t t1 = clock();
	
	std::vector<Anchor> result;
	
		result.clear();
	_extractor.input("data", input);


    std::vector<AnchorGenerator> ac(_feat_stride_fpn.size());
    for (int i = 0; i < _feat_stride_fpn.size(); ++i) 
	{
        int stride = _feat_stride_fpn[i];
        ac[i].Init(stride, anchor_cfg[stride], false);
    }

    std::vector<Anchor> proposals;
    proposals.clear();

    for (int i = 0; i < _feat_stride_fpn.size(); ++i) { 
    	ncnn::Mat cls;
    	ncnn::Mat reg;
    	ncnn::Mat pts;

        // get blob output
        char clsname[100];
		sprintf(clsname, "face_rpn_cls_prob_reshape_stride%d", _feat_stride_fpn[i]);
        char regname[100]; 
		sprintf(regname, "face_rpn_bbox_pred_stride%d", _feat_stride_fpn[i]);
        char ptsname[100]; 
		sprintf(ptsname, "face_rpn_landmark_pred_stride%d", _feat_stride_fpn[i]);
        _extractor.extract(clsname, cls);
        _extractor.extract(regname, reg);
        _extractor.extract(ptsname, pts);

        ac[i].FilterAnchor(cls, reg, pts, proposals);

       // printf("stride %d, res size %d\n", _feat_stride_fpn[i], proposals.size());

      /*  for (int r = 0; r < proposals.size(); ++r) {
            proposals[r].print();
        }*/
    }

    // nms
    nms_cpu(proposals, nms_threshold, result);
	clock_t t2 = clock();

	//选择最大的一个人脸
	int max = 0;

    for(int i = 0; i < result.size(); i ++)
    {
      /*  cv::rectangle(imgSrc, 
			cv::Point((int)result[i].finalbox.x*imgSrc.cols/img.cols, 
			(int)result[i].finalbox.y*imgSrc.rows / img.rows),
			cv::Point((int)result[i].finalbox.width*imgSrc.cols / img.cols, 
			(int)result[i].finalbox.height*imgSrc.rows / img.rows),
			cv::Scalar(0, 255, 255), 2, 8, 0);*/
		if ((result[i].finalbox.width - result[i].finalbox.x) > max)
		{
			max = result[i].finalbox.width - result[i].finalbox.x;

			outF.x = result[i].finalbox.x*img.cols / imgSrc.cols*1.0;
			outF.y = result[i].finalbox.y*img.rows / imgSrc.rows;
			outF.width = result[i].finalbox.width*img.cols / imgSrc.cols - outF.x;
			outF.height = result[i].finalbox.height*img.rows / imgSrc.rows - outF.y;
		}
    }

	if (result.size() > 0)
		return true;

    return 0;
}

