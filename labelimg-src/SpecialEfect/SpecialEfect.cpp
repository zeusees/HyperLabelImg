#include "SpecialEfect.h"
#include <opencv2\imgproc\types_c.h>

struct S_BGRA
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char A;
};


void average4b(const Mat &img, Point_<int> a, Point_<int> b, Vec4b &p)
{
	if(a.x >=0 && a.x < img.cols && b.x >= 0 && b.x < img.cols
		&& a.y >=0 && a.y < img.rows && b.y >=0 && b.y < img.rows)
	{
		  const Vec4b *pix;
			Vec4i temp;
		    temp[0] = 0;
			temp[1] = 0;
			temp[2] = 0;
			temp[3] = 0;
			for (int i = a.y; i <= b.y; i++){
				pix = img.ptr<Vec4b>(i);
				for (int j = a.x; j <= b.x; j++){
					temp[0] += pix[j][0];
					temp[1] += pix[j][1];
					temp[2] += pix[j][2];
					temp[3] += pix[j][3];
				}
			}

			int count = (b.x - a.x+1 ) * (b.y - a.y+1 );
			if(count > 0)
			{
				p[0] = temp[0] / count;
				p[1] = temp[1] / count;
				p[2] = temp[2] / count;
				p[3] = temp[3] / count;
			}
	}
  
   
}

void average3b(const Mat &img, Point_<int> a, Point_<int> b, Vec3b &p)
{
	if(a.x < img.rows && b.x < img.rows && b.y < img.cols && a.y < img.cols)
	{
		 const Vec3b *pix;
			Vec3i temp;
			for (int i = a.x; i <= b.x; i++){
				pix = img.ptr<Vec3b>(i);
				for (int j = a.y; j <= b.y; j++){
					temp[0] += pix[j][0];
					temp[1] += pix[j][1];
					temp[2] += pix[j][2];
			
				}
			}

			int count = (abs(b.x - a.x) + 1) * (abs(b.y - a.y) + 1);
		    if(count != 0)
			{
				p[0] = temp[0] / count;
				p[1] = temp[1] / count;
				p[2] = temp[2] / count;
			}
			else
			{
				p[0] = temp[0] ;
				p[1] = temp[1];
				p[2] = temp[2];
			}
	

	}
		 
   
}

void scalePartAverage4B(const Mat &src, Mat &dst, int cols, int rows)
{
  
	double yRatio = rows*1.0/src.rows;
	double xRatio = cols*1.0/src.cols;
	dst.create(rows, cols, src.type());
	
    int lastRow = 0;
    int lastCol = 0;

    Vec4b *p;
    for (int i = 0; i < rows; i++) {

        p = dst.ptr<Vec4b>(i);
        int row = static_cast<int>((i + 1) / yRatio + 0.5) - 1;
		if(i == 106)
		{
			int k = 0;
		}
        for (int j = 0; j < cols; j++) {
            int col = static_cast<int>((j + 1) / xRatio + 0.5) - 1;

			
            Vec4b pix;
            average4b(src, Point_<int>(lastCol,lastRow ), Point_<int>(col, row), pix);

            p[j] = pix;
            lastCol = col + 1; //下一个子块左上角的列坐标，行坐标不变
        }
        lastCol = 0; //子块的左上角列坐标，从0开始
        lastRow = row + 1; //子块的左上角行坐标
    }
}
void scalePartAverage3B(const Mat &src, Mat &dst, int cols, int rows)
{
  
	double xRatio = rows*1.0/src.rows;
	double yRatio = cols*1.0/src.cols;
	dst.create(rows, cols, src.type());
	
    int lastRow = 0;
    int lastCol = 0;

    Vec3b *p;
    for (int i = 0; i < rows; i++) {

        p = dst.ptr<Vec3b>(i);
        int row = static_cast<int>((i + 1) / xRatio + 0.5) - 1;

        for (int j = 0; j < cols; j++) {
            int col = static_cast<int>((j + 1) / yRatio + 0.5) - 1;

			
            Vec3b pix;
            average3b(src, Point_<int>(lastRow, lastCol), Point_<int>(row, col), pix);

            p[j] = pix;
            lastCol = col + 1; //下一个子块左上角的列坐标，行坐标不变
        }
        lastCol = 0; //子块的左上角列坐标，从0开始
        lastRow = row + 1; //子块的左上角行坐标
    }
}
Mat gaussian_kernal(int dim,double sigma)
{
	int c = dim / 2;
	Mat K(dim, dim, CV_32FC1);
	//生成二维高斯核
	float s2 = 2.0 * sigma * sigma;
	for(int i = (-c); i <= c; i++)
	{
		int m = i + c;
		for (int j = (-c); j <= c; j++)
		{
			int n = j + c;
			float v = exp(-(1.0*i*i + 1.0*j*j) / s2);
			K.ptr<float>(m)[n] = v;
		}
	}
	Scalar all = sum(K);
	Mat gaussK;
	K.convertTo(gaussK, CV_32FC1, (1/all[0]));
	all = sum(gaussK);
	return gaussK;
}


//int GetValueSpan(const Mat& bigMat,int i,int j,double spanX,double spanY,int depth,int index)
// {
//	 float sum = 0.0; 
//	 int jump = j*depth*spanX;
//
//	 int ispanX = spanX+0.5;
//	 int ispanY = spanY+ 0.5;
//	 for(int m = 0;m<ispanY;++m)
//	 {
//		 int step = bigMat.step[0]*(i*spanY+0.5+m);
//		if((i*spanY+m) < bigMat.rows)
//		 {
//			 int jump1=jump;
//			 for(int n = 0;n<ispanX;++n)
//			 {
//				 if(jump1/depth < bigMat.cols)
//				 {
//					 sum+= bigMat.data[step+jump1+0.5+index];
//					 jump1+= depth;
//				 }
//				
//			 }
//		 }
//		
//	 }
//	 return sum/(ispanY*ispanX);
// }
void scalePartAverage(const Mat &src, Mat &dst,int cols ,int rows )
{
	if(src.type() == CV_8UC4)
	{
		scalePartAverage4B(src,dst,cols,rows);
	}
	else
	{
		scalePartAverage3B(src,dst,cols,rows);
	}
}



void Soft_Lighten(Mat& src1, Mat& src2, Mat& dst)
{
	
    float a=0;
    float b=0;
     for(int index_row=0; index_row<src1.rows; index_row++)
    {
        for(int index_col=0; index_col<src1.cols; index_col++)
        {
            for(int index_c=0; index_c<3; index_c++)
            {
                a=src1.at<Vec3b>(index_row, index_col)[index_c]*1.0/255;
                b=src2.at<Vec3b>(index_row, index_col)[index_c]*1.0/255;
				int iValue = 0;
                if(a<=0.5)
                {
					iValue = ((2*a-1)*(b-b*b)+b)*255;
					iValue = (iValue>255)?255:iValue;
					
					dst.at<Vec3b>(index_row, index_col)[index_c]=iValue;
                }
                else
                {
					iValue = ( (2*a-1)*(sqrt(b)-b)+b)*255;
					iValue = (iValue>255)?255:iValue;
					dst.at<Vec3b>(index_row, index_col)[index_c]=iValue;
                }
            }
        }
    }
}


//提取图像高光区
void GetTheHighLight(Mat& src,Mat& dstHight)
{
	dstHight = Mat(src.rows,src.cols,CV_8UC4);
	for(int i=0;i<src.rows;++i)
		for(int j=0;j<src.cols;++j)
		{
			dstHight.data[dstHight.step[0]*i+j*4+0] = 255;
			dstHight.data[dstHight.step[0]*i+j*4+1] = 255;
			dstHight.data[dstHight.step[0]*i+j*4+2] = 255;
			dstHight.data[dstHight.step[0]*i+j*4+3] = 
				(src.data[src.step[0]*i+3*j+0]+src.data[src.step[0]*i+3*j+1]+src.data[src.step[0]*i+3*j+2])/3;
		}
}

Mat WhitePic(Mat& src)
{
	Mat blurImg;
	blur(src,blurImg,Size(5,5));
	Mat dstImg = src.clone();
	Soft_Lighten(blurImg,src,dstImg);

	Mat dstHeigh;
	GetTheHighLight(src,dstHeigh);

	//MixLayer(dstHeigh,dstImg,0.3);

	return dstImg;
}

//滤色
void Screen(Mat& src1, Mat& src2, Mat& dst)
{
     for(int index_row=0; index_row<src1.rows; index_row++)
    {
        for(int index_col=0; index_col<src1.cols; index_col++)
        {
	
			float a1 = src1.data[src1.step[0]*index_row+index_col*3]*1.0/255;
			float b1 = src2.data[src2.step[0]*index_row+index_col*3]*1.0/255;

			dst.data[dst.step[0]*index_row+index_col*3] = (1-(1-a1)*(1-b1))*255;

			a1 = src1.data[src1.step[0]*index_row+index_col*3+1]*1.0/255;
			b1 = src2.data[src2.step[0]*index_row+index_col*3+1]*1.0/255;
			dst.data[dst.step[0]*index_row+index_col*3+1] = (1-(1-a1)*(1-b1))*255;


			a1 = src1.data[src1.step[0]*index_row+index_col*3+2]*1.0/255;
			b1 = src2.data[src2.step[0]*index_row+index_col*3+2]*1.0/255;
			dst.data[dst.step[0]*index_row+index_col*3+2] = (1-(1-a1)*(1-b1))*255;

			
       
        }
    }
}

Mat WhitePicReturnU3(Mat& src,vector<Point> mainPts)
{
	//进行处理的时候，首先要对像素点的范围进行判断，对美白的强度进行调整
	Point ptEyeLeft = mainPts[0];
	Point ptEyeRight = mainPts[1];

	Point ptNose = mainPts[2];
	Point ptMouth = mainPts[3];

	//
	int wValue = 0;
	int iCount = 0;
	for(int i=ptNose.y-(ptNose.y-ptEyeRight.y)*0.3;i<ptMouth.y;++i)
	{
		for(int j=ptEyeLeft.x;j<ptEyeRight.x;++j)
		{
			wValue+= src.data[src.step[0]*i+j*3];
			wValue+= src.data[src.step[0]*i+j*3+1];
			wValue+= src.data[src.step[0]*i+j*3+2];
			
			iCount++;
		}
		
	}
	float alpha = 1.0;
	int iResult = wValue/iCount/3;

	
	if(iResult > 190)
	{
		alpha = 0.1;
	}
	else if(iResult > 180)
	{
		alpha = 0.2;
	}
	else if(iResult > 170)
	{
		alpha = 0.3;
	}
	else if(iResult > 160)
	{
		alpha = 0.6;
	}
	else if(iResult > 150)
	{
		alpha = 0.8;
	}



	Mat blurImg;
	blur(src,blurImg,Size(5,5));
	Mat dstImg = src.clone();
	Soft_Lighten(blurImg,src,dstImg);
	cvtColor(dstImg,dstImg,CV_BGR2BGRA);

	Mat dstHeigh;
	GetTheHighLight(src,dstHeigh);

	//MixLayer(dstHeigh,dstImg,alpha);

	cvtColor(dstImg,dstImg,CV_BGRA2BGR);

	return dstImg;
}



void Rotate90_180(Mat& src,Mat& dst,int type)
{
	int jump = 3;
	if(src.type() == CV_8UC1)
	{
		jump = 1;
	}
	int width=src.cols;
	int height=src.rows;
	unsigned char *ptrSrc=(unsigned char *)(src.data);
	unsigned char *ptrDst=(unsigned char *)(dst.data);
	if (-1 == type)//逆时针旋转90
	{
		for (int j=width;j>0;j--)
		{
			int n = 0;
			int step1 = dst.step[0]*(width-j);
			for (int i=0;i<height;i++)
			{
				memcpy((void *)(ptrDst+n+step1),(void *)(ptrSrc+src.step[0]*i+(j-1)*jump),jump);
				n=n+jump;
			}
		}
	}
	else if(type == 1)//顺时针旋转90度
	{
		for (int j=1;j<=width;j++)
		{
			int n = 0;
			int step1 = dst.step[0]*(j-1);
			for (int i=height;i>0;i--)
			{
				memcpy((void *)(ptrDst+step1+n),(void *)(ptrSrc+src.step[0]*(i-1)+(j-1)*jump),jump);
				n=n+jump;
			}
		}
	}
	else
	{
		//垂直镜像
		for(int i=0;i<height;++i)
		{
			int step1 = dst.step[0]*i;
			int step2 = src.step[0]*(height-i-1);
			for(int j=0;j<width;++j)
			{
				memcpy((void*)&(dst.data[step1+j*jump]),
					(const void*)&(src.data[step2+j*jump]),jump);
			}
		}
	}
}

//agnle
Point_<double> GetNewPoint(Point_<double> in, double angle, Point_<double> center)
{
	//要锟接角讹拷转锟斤拷锟斤拷锟斤拷锟斤拷
	angle = (-angle)*CV_PI / 180;
	//锟斤拷锟斤拷锟斤拷为原锟斤拷
	double Tx = in.x - center.x;
	double Ty = in.y - center.y;

	double nowY = cos(angle)*Ty + sin(angle)*Tx;
	double nowX = -sin(angle)*Ty + cos(angle)*Tx;
	Point resultPt;
	resultPt.x = nowX + center.x;
	resultPt.y = center.y + nowY;

	return resultPt;
}



void RotateMatAndPts(vector<cv::Point>& mainPts, cv::Mat& clmResult, double angle, int iColor)
{
		//锟斤拷锟?锟斤拷锟斤拷2锟饺ｏ拷锟斤拷锟斤拷锟斤拷锟阶拷锟斤拷锟?
		Mat tmpMat = rotateImage2(clmResult, angle, iColor);
		Point center(tmpMat.cols / 2, tmpMat.rows / 2);
		vector<Point> tmpPts;
		int X = (tmpMat.cols + 1) / 2 - clmResult.cols / 2;
		int Y = (tmpMat.rows + 1) / 2 - clmResult.rows / 2;
		for (int i = 0; i <mainPts.size(); ++i)
		{
			Point pt = mainPts[i];
			pt.x += X;
			pt.y += Y;
			//
			Point tmpPt = GetNewPoint(pt, angle, center);
			tmpPts.push_back(tmpPt);
		}
		mainPts = tmpPts;
		clmResult = tmpMat;

}


//旋转图像内容不变，尺寸相应变大
Mat rotateImage2(Mat& img, int degree,int iColor)  
{  
	double angle =degree; 

	int width=img.cols, height=img.rows;
	//旋转后的新图尺寸
	
// = Mat(height_rotate,width_rotate,CV_8CU3); 
	//img_rotate.setTo(0);  
	//保证原图可以任意角度旋转的最小尺寸
	int tempLength = ((int)sqrt(width * width + height *height)+3)/4*4;  
	int tempX = (tempLength + 1) / 2 - width / 2;  
	int tempY = (tempLength + 1) / 2 - height / 2;  
  
	int channeles = img.channels();

	Mat temp = Mat(tempLength,tempLength, CV_8UC(channeles));

	temp.setTo(iColor);
	Mat img_rotate = temp.clone();

	//将原图复制到临时图像tmp
	for(int i=0;i<height;++i)
	{
		int step1 = temp.step[0]*(i+tempY);
		int step2 = img.step[0]*i;
		for(int j=0;j<width;++j)
		{
			memcpy((void*)&(temp.data[step1+(j+tempX)*channeles]), 
						(const void*)&(img.data[step2+j*channeles]), channeles);
		}
	}
	//旋转数组map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float m[6];  
	int w = temp.cols;  
	int h = temp.rows;  

	// 将旋转中心移至图像中间  
	m[2] = w * 0.5f;  
	m[5] = h * 0.5f;  

	Mat M = getRotationMatrix2D(Point(m[2],m[5]),angle,1.0);
	warpAffine(temp,img_rotate,M,Size(tempLength,tempLength));

	return img_rotate;
}  

//USM锐化,0-10的锐化程度
void USMSharp(Mat& imgSrc,int stepCount)
{
	//让参数满足限制条件
	stepCount = abs(stepCount);
	if(stepCount > 10) stepCount = 10;

	if(stepCount > 0)
	{
		double sigma = 3;
		int threshold = 0;
		float amount = 1;
		Mat imgBlurred;

		int sSize = stepCount/2*2+1;
		GaussianBlur(imgSrc, imgBlurred, Size(sSize,sSize), sigma, sigma);
		Mat lowContrastMask = abs(imgSrc-imgBlurred)<threshold;
		Mat imgDst = imgSrc*(1+amount)+imgBlurred*(-amount);
		imgSrc.copyTo(imgDst, lowContrastMask);
		imgSrc = imgDst;
	}

}
 Mat sharpenImage0(const cv::Mat &image)
 {
     //为输出图像分配内存

	 Mat result[3];
	 Mat rgb[3];
	 split(image,rgb);
 
	 for(int n=0;n<3;++n)
	 {
		 result[n] = rgb[n];
	 }

	 cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
	 kernel.at<float>(1,1) = 5.0;
	kernel.at<float>(0,1) = -1.0;
	 kernel.at<float>(1,0) = -1.0;
	 kernel.at<float>(1,2) = -1.0;
	kernel.at<float>(2,1) = -1.0;

	 for(int m=0;m<3;++m)
	 {
		 //对图像进行滤波
		 cv::filter2D(rgb[m],result[m],rgb[m].depth(),kernel);
	 }
     
     Mat dst;
	 merge(result,3,dst);

	 return dst;
 }

void FeatherPtsInstead(Mat& matResult,vector<cv::Point> pts)
{
	Rect boundMouth = boundingRect(pts);
	int yiMouth = boundMouth.height*0.1;

	for(int i=0;i<=6;++i)
	{
		pts[i].y-=yiMouth;
	}
	for(int i=7;i<=11;++i)
	{
		pts[i].y +=yiMouth;
	}


	Mat mask = Mat(matResult.rows,matResult.cols,CV_8UC1);
	mask.setTo(0);
	vector< vector<Point> > arrayPts;
	arrayPts.push_back(pts);
	fillPoly(mask,arrayPts,Scalar(255,255,255));
	

	Rect boundRect = boundingRect(pts);
	for (int i = boundRect.y; i < (boundRect.y+boundRect.height); ++i) 
	{
		int step1 =matResult.cols*i;
		int step2 =matResult.cols*(i+boundRect.height+1);

	
		for (int j = boundRect.x; j < (boundRect.x+boundRect.width); ++j) 
		{
				if(mask.data[mask.step[0]*i+j] == 255)
				{
						((S_BGRA*)matResult.data)[step1+j] = 
							((S_BGRA*)matResult.data)[step2+j];
				}	
			//rgbA.A = mask.data[mask.step[0]*i+j];
		}
	}
}

void FeatherPtsOutWhole(Mat& matResult,vector<Point> mainPts,bool bBlack)
{
	int disEye = (mainPts[42].x-mainPts[36].x)/2;

	vector< vector<Point> > arrayPts;

	vector<cv::Point> leftMei;
	for(int i=17;i<=21;++i)
	{
		leftMei.push_back(cv::Point(mainPts[i].x,mainPts[i].y));
	}
	for(int i = 21;i>= 17;--i)
	{
		leftMei.push_back(cv::Point(mainPts[i].x,mainPts[i].y+disEye*0.1));
	}

	vector<cv::Point> leftEyePts;
	for(int i=36;i<=41;++i)
	{
		leftEyePts.push_back(cv::Point(mainPts[i].x,mainPts[i].y));
	}

	vector<cv::Point> rightMei;
	for(int i=22;i<=26;++i)
	{
		rightMei.push_back(cv::Point(mainPts[i].x,mainPts[i].y));
	}
	for(int i=26;i>=22;--i)
	{
		rightMei.push_back(cv::Point(mainPts[i].x,mainPts[i].y+disEye*0.1));
	}

	vector<cv::Point> rightEyePts;
	for(int i=42;i<=47;++i)
	{
		rightEyePts.push_back(cv::Point(mainPts[i].x,mainPts[i].y));
	}
	
	vector<cv::Point> mouthOutPts;
	for(int i=48;i<=59;++i)
	{
		mouthOutPts.push_back(cv::Point(mainPts[i].x,mainPts[i].y));
	}

	Mat mask = Mat(matResult.rows,matResult.cols,CV_8UC1);
	if(bBlack)
	{
		mask.setTo(255);
		arrayPts.clear();
		arrayPts.push_back(leftEyePts);
		fillPoly(mask,arrayPts,Scalar(0,0,0));
		arrayPts.clear();
		arrayPts.push_back(rightEyePts);
		fillPoly(mask,arrayPts,Scalar(0,0,0));
		arrayPts.clear();
		//arrayPts.push_back(mouthOutPts);
		//fillPoly(mask,arrayPts,Scalar(0,0,0));
	}
	else
	{
		mask.setTo(0);
		arrayPts.clear();
		arrayPts.push_back(leftMei);
		//fillPoly(mask,arrayPts,Scalar(255,255,255));

		arrayPts.clear();
		arrayPts.push_back(rightMei);
		//fillPoly(mask,arrayPts,Scalar(255,255,255));

		arrayPts.clear();
		arrayPts.push_back(leftEyePts);
		fillPoly(mask,arrayPts,Scalar(255,255,255));
		arrayPts.clear();
		arrayPts.push_back(rightEyePts);
		fillPoly(mask,arrayPts,Scalar(255,255,255));
		arrayPts.clear();
		//arrayPts.push_back(mouthOutPts);
		//fillPoly(mask,arrayPts,Scalar(255,255,255));
	}


	Rect boundRect = boundingRect(mouthOutPts);

	Mat blurMask;
	GaussianBlur(mask,blurMask,Size(7,7),0);


	//FeatherPtsOut(matResult,leftEyePts,blurMask);
	//FeatherPtsOut(matResult,rightEyePts,blurMask);
	//FeatherPtsOut(matResult,mouthOutPts,blurMask);

	for(int i = 0;i<blurMask.rows;++i)
	{
		int step1 = matResult.cols*i;
		int step2 = blurMask.step[0]*i;
		for(int j=0;j<blurMask.cols;++j)
		{
			S_BGRA& rgbA = ((S_BGRA*)matResult.data)[step1+j];
				
			rgbA.A = blurMask.data[step2+j];
		}
	}
}

void FeatherPtsOut(Mat& matResult,vector<Point> pts,Mat& blurMask)
{


	Rect boundRect = boundingRect(pts);

	double yuZhi = 5.0;
	for (int i = boundRect.y; i < (boundRect.y+boundRect.height); ++i) 
	{
		int step1 =matResult.cols*i;
		for (int j = boundRect.x; j < (boundRect.x+boundRect.width); ++j) 
		{

			S_BGRA& rgbA = ((S_BGRA*)matResult.data)[step1+j];
				
			rgbA.A = blurMask.data[blurMask.step[0]*i+j];
			rgbA.A = (rgbA.A==0)?1:rgbA.A;
		}
	}
}

