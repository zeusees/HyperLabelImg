#include "ImageLayer.h"
	struct GBRAItem
	{
		unsigned char G;
		unsigned char B;
		unsigned char R;
		unsigned char A;
	};

void MixLayerDarkPos(Mat& src,Mat& dst,int posDstX,int posdstY,float alpha)
{


	for(int i=0;i<src.rows;++i)
	{

		if(i+posdstY < dst.rows)
		{
			int step = (i+posdstY)*dst.cols;
		
			int step1 = i*src.cols;
			

			for(int j=0;j<src.cols;++j)
			{

				GBRAItem  &dstGBRA = ((GBRAItem*)dst.data)[step+j+posDstX];
				GBRAItem  &srcGBRA = ((GBRAItem*)src.data)[step1+j];	
			
				if((srcGBRA.B+srcGBRA.G+srcGBRA.R) > (dstGBRA.B+dstGBRA.G+dstGBRA.R))
				{
					srcGBRA.A = 0;
				}

				int dstAlpha = dstGBRA.A;
				int srcAlpha = srcGBRA.A;
				if(dstAlpha == 0)
				{
				   dstGBRA = srcGBRA;
				   continue;
				}
				int dstAlpha1 = dstAlpha+srcAlpha*alpha-dstAlpha*srcAlpha*alpha/255;

				int dstRGB = dstGBRA.G;
				int srcRGB = srcGBRA.G;
				dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;
	
	
				dstGBRA.G = (dstRGB>255)?255:dstRGB;


				dstRGB = dstGBRA.B;
				srcRGB = srcGBRA.B;

				dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;
		

				dstGBRA.B = (dstRGB>255)?255:dstRGB;


				dstRGB = dstGBRA.R;
				srcRGB = srcGBRA.R;

				dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;

				dstGBRA.R = (dstRGB>255)?255:dstRGB;

	
			
				dstGBRA.A = dstAlpha1;
			}
		}
	
	}
}


//photoshop
//dstRGB = dstRGB * dstAlpha / 255

//dstRGB = dstRGB +  (srcRGB - dstRGB) * srcAlpha * alpha / 255

//dstAlpha = dstAlpha + srcAlpha * alpha - dstAlpha * srcAlpha * alpha / 255

//dstRGB = dstRGB * 255 / dstAlpha
void MixLayerPoint(Mat& src,Mat& dst,int& pos,float& alpha)
{
		float a1 ;
		float a2 ;
		GBRAItem  &dstGBRA = ((GBRAItem*)dst.data)[pos];
		GBRAItem  &srcGBRA = ((GBRAItem*)src.data)[pos];	
				
		int dstAlpha = dstGBRA.A;
		int srcAlpha = srcGBRA.A;
		if(dstAlpha == 0)
		{
			  dstGBRA = srcGBRA;
			return;
		}
			int dstRGB = dstGBRA.G;
			int srcRGB = srcGBRA.G;

			if(srcAlpha == 255 && dstAlpha == 255)
			{
				a1 = alpha;
				a2 = 1-alpha;
			}
			else
			{
				a1 = srcAlpha*alpha/dstAlpha;
				a2 = 1-srcAlpha*alpha/255;
			}
	

			dstRGB = srcRGB*a1+dstRGB*a2;
			dstGBRA.G = dstRGB;


			dstRGB = dstGBRA.B;
			srcRGB = srcGBRA.B;
			dstRGB = srcRGB*a1+dstRGB*a2;
			dstGBRA.B = dstRGB;


			dstRGB = dstGBRA.R;
			srcRGB = srcGBRA.R;
			dstRGB = srcRGB*a1+dstRGB*a2;
			dstGBRA.R = dstRGB;

	
			dstAlpha = dstAlpha+srcAlpha*alpha-dstAlpha*srcAlpha*alpha/255;
			dstGBRA.A = dstAlpha;
}

void MixLayerPos(Mat& src,Mat& dst,int posDstX,int posdstY,float alpha)
{
	struct GBRAItem
	{
		unsigned char G;
		unsigned char B;
		unsigned char R;
		unsigned char A;
	};


	for(int i=0;i<src.rows;++i)
	{

		if(i+posdstY < dst.rows)
		{
			int step = (i+posdstY)*dst.cols;
		
			int step1 = i*src.cols;
			

			for(int j=0;j<src.cols;++j)
			{

				GBRAItem  &dstGBRA = ((GBRAItem*)dst.data)[step+j+posDstX];
				GBRAItem  &srcGBRA = ((GBRAItem*)src.data)[step1+j];	
			
			
				int dstAlpha = dstGBRA.A;
				int srcAlpha = srcGBRA.A;
				if(dstAlpha == 0)
				{
				   dstGBRA = srcGBRA;
				   continue;
				}
				int dstAlpha1 = dstAlpha+srcAlpha*alpha-dstAlpha*srcAlpha*alpha/255;

				int dstRGB = dstGBRA.G;
				int srcRGB = srcGBRA.G;
				dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;
	
	
				dstGBRA.G = (dstRGB>255)?255:dstRGB;


				dstRGB = dstGBRA.B;
				srcRGB = srcGBRA.B;

				dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;
		

				dstGBRA.B = (dstRGB>255)?255:dstRGB;


				dstRGB = dstGBRA.R;
				srcRGB = srcGBRA.R;

				dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;

				dstGBRA.R = (dstRGB>255)?255:dstRGB;

	
			
				dstGBRA.A = dstAlpha1;
			}
		}
	
	}
}

//photoshop图层叠加公式
//3-1）dstRGB = dstRGB * dstAlpha / 255

//3-2）dstRGB = dstRGB +  (srcRGB - dstRGB) * srcAlpha * alpha / 255

//3-3）dstAlpha = dstAlpha + srcAlpha * alpha - dstAlpha * srcAlpha * alpha / 255

//3-4）dstRGB = dstRGB * 255 / dstAlpha

void MixLayer(Mat& src,Mat& dst,float alpha)
{
	struct GBRAItem
	{
		unsigned char G;
		unsigned char B;
		unsigned char R;
		unsigned char A;
	};


	for(int i=0;i<src.rows;++i)
	{

		int step = i*src.cols;
		for(int j=0;j<src.cols;++j)
		{

			GBRAItem  &dstGBRA = ((GBRAItem*)dst.data)[step+j];
			GBRAItem  &srcGBRA = ((GBRAItem*)src.data)[step+j];	
			
			
			int dstAlpha = dstGBRA.A;
			int srcAlpha = srcGBRA.A;
			if(dstAlpha == 0)
			{
			   dstGBRA = srcGBRA;
			   continue;
			}
			int dstAlpha1 = dstAlpha+srcAlpha*alpha-dstAlpha*srcAlpha*alpha/255;

			int dstRGB = dstGBRA.G;
			int srcRGB = srcGBRA.G;
			dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;
	
	
			dstGBRA.G = (dstRGB>255)?255:dstRGB;


			dstRGB = dstGBRA.B;
			srcRGB = srcGBRA.B;

			dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;
		

			dstGBRA.B = (dstRGB>255)?255:dstRGB;


			dstRGB = dstGBRA.R;
			srcRGB = srcGBRA.R;

			dstRGB = (dstRGB*dstAlpha+ (srcRGB-dstRGB*dstAlpha/255)*srcAlpha*alpha)/dstAlpha1;

			dstGBRA.R = (dstRGB>255)?255:dstRGB;

	
			
			dstGBRA.A = dstAlpha1;
		}
	}
}
Mat MatMixAddEdge(Mat&src,Mat& dst)
{
	Mat resMat = dst.clone();
	for(int i=0;i<src.rows;++i)
	{
		int step = src.step[0]*i;
		int jump = 0;
		for(int j = 0;j<src.cols;++j)
		{
			if(src.data[step+jump+0] < 128)
			{
				resMat.data[step+jump+0] = resMat.data[step+jump+1] = resMat.data[step+jump+2]
				= src.data[step+jump+0] ;
			}
			jump+=3;
		}
	}
	return resMat;
}
Mat MatMixByLight(Mat&src ,Mat& dst,float alpha )
{

	Mat srcHSV;
	Mat dstHSV;
	cvtColor(src,srcHSV,CV_BGR2Lab);
	cvtColor(dst,dstHSV,CV_BGR2Lab);
	

	for(int i=0;i<src.rows;++i)
	{
		int step = src.step[0]*i;
		int jump = 0;
		for(int j = 0;j<src.cols;++j)
		{
			dstHSV.data[step+jump+0] = srcHSV.data[step+jump+0]*alpha+
				dstHSV.data[step+jump+0]*(1-alpha) ;
			
			
			jump+=3;
		}
	}
	Mat resMat;
	cvtColor(dstHSV,resMat,CV_Lab2BGR);
	return resMat;
}
//---------------------------------------------------------------------------
void MatColorMix(Mat& src,Mat& dst)
{
	Mat srcBGR;
	Mat dstBGR;
	Mat srcLab;
	Mat dstLab;
	cvtColor(src,srcBGR,CV_BGRA2BGR);
	cvtColor(dst,dstBGR,CV_BGRA2BGR);

	cvtColor(srcBGR,srcLab,CV_BGR2Lab);
	cvtColor(dstBGR,dstLab,CV_BGR2Lab);
	//先上色，后进行通道的融合
	for(int i=0;i<srcLab.rows;++i)
	{
		int step1 = dstLab.step[0]*i;
		int jump = 0;

		int step2 = dst.step[0]*i;
		int jump2 = 0;
		for(int j=0;j<srcLab.cols;++j)
		{
			if(dst.data[step2+jump2+3] > 0)
			{
				dstLab.data[step1+jump+1] = srcLab.data[step1+jump+1];
				dstLab.data[step1+jump+2] = srcLab.data[step1+jump+2];
			}	
		
			jump+=3;
			jump2+=4;
		}
	}
	//
	cvtColor(dstLab,dstBGR,CV_Lab2BGR);
	//
	for(int i=0;i<dst.rows;++i)
	{
		int step1 = dst.step[0]*i;
		int step2 = dstBGR.step[0]*i;
		int jump1 = 0;
		int jump2 = 0;
		for(int j=0;j<dst.cols;++j)
		{
			memcpy(&(dst.data[step1+jump1]),&(dstBGR.data[step2+jump2]),3);

			jump1+=4;
			jump2+=3;
		}
	}
		
	MixLayer(dst,src,1.0);

	dst = src;
}