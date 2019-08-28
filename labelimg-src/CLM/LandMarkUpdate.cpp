#include "LandMarkUpdate.h"
#include "clmInter.h"
#include <vector>
int dstH,dstS,dstV,dstH1,dstS1,dstV1;
bool gBLeftNeedUpdate;
bool gBRightNeedUpdate;
int mIndexIter;
cv::Mat_<double> updateOut3D;
cv::Vec6d   gparamGlobal;

using namespace cv;
using namespace std;
#include <SpecialEfect\SpecialEfect.h>
void FilterTheShapeLeft(cv::Mat_<double>& outShape,Vec6d params_global);
void FilterTheShapeRight(cv::Mat_<double>& outShape,Vec6d params_global);


void RotateBack(Mat_<double>& out_shape,double& angle)
{
	vector<Point_<double> > mainPts;
	for(int i = 0;i<68;++i)
	{
		mainPts.push_back( Point(((double*)(out_shape.data))[i],((double*)(out_shape.data))[i+68]));
	}
	//16个点进行调正
		Point_<double> center(mainPts[27].x,mainPts[27].y);
		
		vector<Point_<double>> tmpPts;	
		for(int i = 0;i < 68;++i)
		{

			if(i != 27)
			{
				Point_<double> pt = mainPts[i];
				Point_<double> tmpPt = GetNewPoint(pt,angle,center);
				tmpPts.push_back(tmpPt);
			}
			else
			{
				tmpPts.push_back(mainPts[i]);
			}



		}
		//从新赋值
		for(int i = 0;i< 68;++i)
		{
			((double*)(out_shape.data))[i] = tmpPts[i].x;
			((double*)(out_shape.data))[i+68] = tmpPts[i].y;
		}
}
bool RotateToVertical(Mat_<double>& out_shape,double& outAngle)
{

	vector<Point_<double> > mainPts;
	for(int i = 0;i<68;++i)
	{
		mainPts.push_back( Point_<double>(((double*)(out_shape.data))[i],((double*)(out_shape.data))[i+68]));
	}

	Point_<double> ptLeft;
	ptLeft.x = (mainPts[37].x+mainPts[38].x)/2;
	ptLeft.y = (mainPts[37].y+mainPts[41].y+mainPts[38].y+mainPts[40].y)/4;
	Point_<double> ptRight;
	ptRight.x = (mainPts[43].x+mainPts[44].x)/2;
	ptRight.y = (mainPts[43].y+mainPts[47].y+mainPts[44].y+mainPts[46].y)/4;


	int X = ptRight.x-ptLeft.x;
	int Y = ptRight.y-ptLeft.y;
	double R = sqrt(X*X+Y*Y*1.0);



	double angle = abs(asin(abs(Y/R)))*180/CV_PI;
	if(angle >= 3)
	{
		if(Y < 0)
		{
			angle = -angle;
		}

		//16个点进行调正
		Point_<double> center(mainPts[27].x,mainPts[27].y);
		
		vector<Point_<double>> tmpPts;	
		for(int i = 0;i < 68;++i)
		{

			if(i != 27)
			{
				Point_<double> pt = mainPts[i];
				//
				Point_<double> tmpPt = GetNewPoint(pt,angle,center);
				tmpPts.push_back(tmpPt);

			}
			else
			{
				tmpPts.push_back(mainPts[i]);
			}
		

		}
		//从新赋值
		for(int i = 0;i< 68;++i)
		{
			((double*)(out_shape.data))[i] = tmpPts[i].x;
			((double*)(out_shape.data))[i+68] = tmpPts[i].y;
		}



		outAngle = -angle;
		return true;
	}

	return false;
		
}


void FilterParaLocal(cv::Mat_<double> mean_shape,cv::Mat_<double> princ_comp ,cv::Mat_<double>& params_local)
{
	// get the 3D shape of the object
	bool bNeedUpdate = false;
	Mat_<double> out_shape = mean_shape + princ_comp * params_local;
	for(int k = 0;k<= 5;++k)
	{
			double diffR1 = ((double*)(out_shape.data))[16-k] - ((double*)(out_shape.data))[27];
			double diffL1 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[k];

			double diffRH = ((double*)(out_shape.data))[16-k+68] - ((double*)(out_shape.data))[27+68];
			double diffLH = ((double*)(out_shape.data))[k+68] - ((double*)(out_shape.data))[27+68] ;

			if(diffL1/diffR1 > 1.08 && mIndexIter >= 2 && k<= 5)
			{
				bNeedUpdate = true;

				break;
			}

			if(diffLH/diffRH > 1.18 && k>= 3 && mIndexIter >= 2 )
			{
				bNeedUpdate = true;
				break;
			}

	}

	  if(bNeedUpdate)
		{
			for(int k = 0;k<= 7;++k)
			{
				double diffR1 = ((double*)(out_shape.data))[16-k] - ((double*)(out_shape.data))[27];
				double diffL1 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[k];

				double diffR2 = ((double*)(out_shape.data))[45] - ((double*)(out_shape.data))[27];
				double diffL2 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[36];

				if(mIndexIter >= 1 )
				{
					double biaoValue =abs(((double*)(out_shape.data))[27]-((double*)(out_shape.data))[16-k]);

					//if(((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[k]  > biaoValue)
					{
						double f1 = ((double*)(out_shape.data))[k];
						((double*)(out_shape.data))[k] = ((double*)(out_shape.data))[27] -biaoValue ;
						double f2 = ((double*)(out_shape.data))[k];
						int kk = 0;
					}
					
			
					//if(((double*)(out_shape.data))[k+68] > ((double*)(out_shape.data))[16-k+68])
					 ((double*)(out_shape.data))[k+68] = ((double*)(out_shape.data))[16-k+68];
					 ((double*)(out_shape.data))[k+68+68] = ((double*)(out_shape.data))[16-k+68+68];
				}
			}

			gBLeftNeedUpdate = true;

			//Mat_<double> out_shape = mean_shape + princ_comp * params_local;

			Mat_<double> diffShape = out_shape -mean_shape;
			Mat resMat = (princ_comp.t()*princ_comp).inv()*diffShape;
			//Mat_<double> invMat ;
			//double calR = invert(princ_comp,invMat,DECOMP_SVD);


			params_local = resMat;
			int kk = 0;

		}
}


void FilterTheShapeLeft(cv::Mat_<double>& out_shape,Vec6d params_global)
{
	   gBLeftNeedUpdate = false;

	   bool bNeedUpdate = false;
		double diffR2 = (((double*)(out_shape.data))[45]+ ((double*)(out_shape.data))[42])/2 - ((double*)(out_shape.data))[27];
		double diffL2 = ((double*)(out_shape.data))[27] - (((double*)(out_shape.data))[36]+((double*)(out_shape.data))[39])/2;

		float bDiff = diffL2/diffR2;

		for(int k = 0;k<= 5;++k)
		{
			double diffR1 = ((double*)(out_shape.data))[16-k] - ((double*)(out_shape.data))[27];
			double diffL1 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[k];

			double diffRH = ((double*)(out_shape.data))[16-k+68] - ((double*)(out_shape.data))[27+68];
			double diffLH = ((double*)(out_shape.data))[k+68] - ((double*)(out_shape.data))[27+68] ;
			double tmpDiff = bDiff;
			if(abs(params_global[2]) > 0.06)
			{
				tmpDiff = bDiff+(abs(params_global[2])-0.06)*0.6;
				//偏的太狠不予处理
				return;
			}
			if(diffL1/diffR1 > 1.116*tmpDiff  && k<= 5)
			{
				bNeedUpdate = true;

				break;
			}
			

			tmpDiff = 1.12;
			if(bDiff > 1.3)
			{
				tmpDiff = 1.2+(bDiff-1.1);
			}
			if(diffLH/diffRH > tmpDiff && k>= 3)
			{
				bNeedUpdate = true;
				break;
			}

		}
		
		if(bNeedUpdate)
		{
			if(abs(params_global[2]) > 0.06)
			{
				bDiff = bDiff+(abs(params_global[2])-0.06)*0.6;
			}
			for(int k = 0;k<= 7;++k)
			{
				double diffR1 = ((double*)(out_shape.data))[16-k] - ((double*)(out_shape.data))[27];
				double diffL1 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[k];

				double diffR2 = ((double*)(out_shape.data))[45] - ((double*)(out_shape.data))[27];
				double diffL2 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[36];

				double biaoValue = bDiff*abs(((double*)(out_shape.data))[27]-((double*)(out_shape.data))[16-k]);

				((double*)(out_shape.data))[k] = ((double*)(out_shape.data))[27] -biaoValue ;
				((double*)(out_shape.data))[k+68] = ((double*)(out_shape.data))[16-k+68];
				
			}
			
			gBLeftNeedUpdate = true;
		}
}


void FilterTheShapeRight(cv::Mat_<double>& out_shape,Vec6d params_global)
{
	    gBRightNeedUpdate = false;
	
	    bool bNeedUpdate = false;
		double diffR2 = (((double*)(out_shape.data))[45]+ ((double*)(out_shape.data))[42])/2 - ((double*)(out_shape.data))[27];
		double diffL2 = ((double*)(out_shape.data))[27] - (((double*)(out_shape.data))[36]+((double*)(out_shape.data))[39])/2;

		float bDiff = diffR2/diffL2;

		for(int k = 0;k<= 5;++k)
		{
			double diffR1 = ((double*)(out_shape.data))[16-k] - ((double*)(out_shape.data))[27];
			double diffL1 = ((double*)(out_shape.data))[27] - ((double*)(out_shape.data))[k];

			double diffRH = ((double*)(out_shape.data))[16-k+68] - ((double*)(out_shape.data))[27+68];
			double diffLH = ((double*)(out_shape.data))[k+68] - ((double*)(out_shape.data))[27+68] ;
			double tmpDiff = bDiff;
			if(abs(params_global[2]) > 0.06)
			{
				//偏的太狠不予处理
				tmpDiff = bDiff+(abs(params_global[2])-0.06)*0.6;
				return;
			}

			if(diffR1/diffL1 > 1.16*tmpDiff  && k<= 5)
			{
				bNeedUpdate = true;

				break;
			}
			

			tmpDiff = 1.13;
			
			if(diffRH/diffLH > tmpDiff && k>= 3)
			{
				bNeedUpdate = true;
				break;
			}

		}
		
		if(bNeedUpdate)
		{
			if(abs(params_global[2]) > 0.06)
			{
				bDiff = bDiff+(abs(params_global[2])-0.06)*0.6;
			}

			for(int k = 0;k<= 7;++k)
			{
		
				double biaoValue = bDiff*abs(((double*)(out_shape.data))[27]-((double*)(out_shape.data))[k]);

				((double*)(out_shape.data))[16-k] = ((double*)(out_shape.data))[27] +biaoValue ;
				((double*)(out_shape.data))[16-k+68] = ((double*)(out_shape.data))[k+68];
				
			}
			
			gBRightNeedUpdate = true;
		}

		
}
void FilterTheShape(Mat_<double>& out_shape,Vec6d params_global)
{

	   double outChangeAngel;
	   bool bChangedAngle = RotateToVertical(out_shape,outChangeAngel);

	   FilterTheShapeLeft(out_shape,params_global);

	   if(!gBLeftNeedUpdate)
	   {
		   FilterTheShapeRight(out_shape,params_global);
	   }

		if(bChangedAngle)
		{
			//进行复原
			RotateBack(out_shape,outChangeAngel);
		}


		if(gBLeftNeedUpdate)
		{
				////统计HSV信息
				//Mat matHsv;
				//cvtColor(clmSrc,matHsv,CV_BGR2HSV);
				//cv::Mat_<double> shape2D = current_shape.clone();
				//vector<Point> pts;
				//int n = shape2D.rows/2;
				//for( int i = 0; i < n; ++i)
				//{		
				//	Point featurePoint((int)shape2D.at<double>(i), (int)shape2D.at<double>(i +n));
				//	pts.push_back(featurePoint);
				//}
				//StatisticNose123HSV_3(matHsv,pts,dstH,dstS,dstV,dstH1,dstS1,dstV1);
		}
}

//找出分割线
bool FindSegLine(Mat& labMat,bool bShow)
{
	bool bFind = false;
	int wid = labMat.cols;
	//锟揭憋拷4.3锟斤拷
	vector<Point> linePts2;
	float k_1 = -(wid*0.4)*1.0/wid;
	for(int i = 0;i> -wid;--i )
	{
		int x = k_1*i;
		Point pt(wid+x,-i);
		linePts2.push_back(pt);
	}

	//
	int iCountLeft = 0;
	int iCountRight = 0;

	//统计左右点的个数，然后作一定的逻辑判断
	for(int i=wid*0.8;i>0;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid)
			{
				if(labMat.data[labMat.step[0]*pt.y+pt.x-i] == 255)
				{
					iCountRight++;
				}
			}
			else
			{
				break;
			}

		}

	}

	for(int i=wid*2;i>wid*1.6;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid && pt.x -i > 0)
			{
				if(labMat.data[labMat.step[0]*pt.y+pt.x-i] == 255)
				{
					iCountLeft++;
				}
			}
			else
			{
				continue;
			}

		}

	}

	if(bShow)
	{
		//imshow("1.labMat",labMat);
	}
	for(int i=wid*2;i>wid*0.8;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid && pt.x -i > 0)
			{
				labMat.data[labMat.step[0]*pt.y+pt.x-i] = 0;
			}
			else
			{
				continue;
			}

		}

	}
	for(int i=wid*0.3;i>0;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid)
			{
				labMat.data[labMat.step[0]*pt.y+pt.x-i] = 255;
				
			}
			else
			{
				break;
			}

		}

	}

	if(bShow)
	{
		//imshow("2.labMat",labMat);
	}


	if( iCountRight > iCountLeft && iCountRight*1.0/(wid*wid) > 0.13
		&& iCountLeft*1.0/(wid*wid)  < 0.06)
	{
		return true;
	}
	return false;
}

bool RectifyPtsSegLeft(cv::Mat& labMat,cv::Mat& outSeg,bool bDeliate )
{
	int wid = labMat.cols;

	//锟揭憋拷4.3锟斤拷
	vector<Point> linePts2;
	float k_1 = -(wid*0.65)*1.0/wid;
	for(int i = 0;i> -wid;--i )
	{
		int x = k_1*i;
		Point pt(wid+x,-i);
		linePts2.push_back(pt);
	}

	Mat matHsv;
	cvtColor(labMat,matHsv,CV_BGR2HSV);

	Mat tmp2 = Mat(matHsv.rows,matHsv.cols,CV_8UC1);
	tmp2.setTo(0);
	for(int i=0;i<tmp2.rows;++i)
	{
		int step = matHsv.step[0]*i;
		for(int j = 0;j<tmp2.cols;++j)
		{
			if(abs(matHsv.data[step+j*3+0] -dstH) < 15
				&& abs(matHsv.data[step+j*3+1] -dstS) < 15
				&& abs(matHsv.data[step+j*3+2] -dstV) < 30)
			{
				tmp2.data[tmp2.step[0]*i+j] = 255;
			}
		}
	}

	


	//进行水漫操作
	for(int k = 0;k<5;++k)
	{
		int disSpan = 1;


		int yuZhi = 15;
		for(int i=disSpan;i<tmp2.rows-disSpan;++i)
		{
			int step = matHsv.step[0]*i;
		
			int stepBottom = matHsv.step[0]*(i+disSpan);

			int stepTop = matHsv.step[0]*(i-disSpan);
			for(int j = disSpan;j<tmp2.cols-disSpan;++j)
			{
				int dstH = matHsv.data[step+j*3+0];
				int dstS = matHsv.data[step+j*3+1];
				int dstV = matHsv.data[step+j*3+2];

				
				if(tmp2.data[tmp2.step[0]*i+j] == 0)
				{
					//左边
					if(/*(tmp2.data[tmp2.step[0]*i+j+disSpan] == 255
						&& (abs(matHsv.data[step+(j+disSpan)*3+0] -dstH) < yuZhi || abs(matHsv.data[step+(j+disSpan)*3+0] -dstH) > (180-yuZhi))
					    && abs(matHsv.data[step+(j+disSpan)*3+1] -dstS) < yuZhi)||*/
						//下边
						 (tmp2.data[tmp2.step[0]*(i+disSpan)+j] == 255
						    && (abs(matHsv.data[stepBottom+j*3+0] -dstH) < yuZhi || abs(matHsv.data[stepBottom+j*3+0] -dstH) > (180-yuZhi))
					        && abs(matHsv.data[stepBottom+j*3+1] -dstS) < yuZhi)
						  ||  //上面
							 (tmp2.data[tmp2.step[0]*(i-disSpan)+j] == 255
						    && (abs(matHsv.data[stepTop+j*3+0] -dstH) < yuZhi || abs(matHsv.data[stepTop+j*3+0] -dstH) > (180-yuZhi))
					        && abs(matHsv.data[stepTop+j*3+1] -dstS) < yuZhi)
						  
						  )
					{
						tmp2.data[tmp2.step[0]*i+j] = 255;
					}

					

					
				}
	
			}
			
		}
	}

	
	



	
	//进行腐蚀
	int dilation_type;
	dilation_type = MORPH_CROSS;
	int dilation_size = 1;
    Mat element = getStructuringElement( dilation_type,
                                       Size( dilation_size + 1, dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );

	
	 
	
	 Mat tmp = tmp2.clone();
	 imwrite("E:/pix1.bmp",tmp);
	 //if(bDeliate)
	 dilate(tmp,tmp2,element);
	 imwrite("E:/pix2.bmp",tmp2);

	
	 outSeg = tmp2.clone();

		

	imwrite("E:/src.bmp",labMat);
		
	
	bool bRes =  FindSegLineLeft(outSeg,true);

	 tmp = outSeg.clone();
	erode(tmp,outSeg,element);
	 imwrite("E:/pix4.bmp",outSeg);

	 //	Mat tmpMat;
		//resize(outSeg,tmpMat,Size(area_of_interest_width,area_of_interest_height));
		//Mat_<float> outLineF;
		//tmpMat.convertTo(outLineF, CV_32FC1); // or CV_32F works (too)
	return bRes;
}

bool FindSegLineLeft(cv::Mat& labMat,bool bShow )
{
	bool bFind = false;
	int wid = labMat.cols;
	//锟揭憋拷4.3锟斤拷
	vector<Point> linePts2;
	float k_1 = -(wid*0.4)*1.0/wid;
	for(int i = 0;i> -wid;--i )
	{
		int x = k_1*i;
		Point pt(wid+x,-i);
		linePts2.push_back(pt);
	}

	//
	int iCountLeft = 0;
	int iCountRight = 0;

	//统计左右点的个数，然后作一定的逻辑判断
	for(int i=wid*0.8;i>0;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid)
			{
				if(labMat.data[labMat.step[0]*pt.y+pt.x-i] == 255)
				{
					iCountRight++;
				}
			}
			else
			{
				break;
			}

		}

	}

	for(int i=wid*2;i>wid*0.8;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid && pt.x -i > 0)
			{
				if(labMat.data[labMat.step[0]*pt.y+pt.x-i] == 255)
				{
					iCountLeft++;
				}
			}
			else
			{
				continue;
			}

		}

	}

	if(bShow)
	{
		//imshow("1.labMat",labMat);
	}
	for(int i=wid*2;i>wid*1.6;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid && pt.x -i > 0)
			{
				labMat.data[labMat.step[0]*pt.y+pt.x-i] = 0;
			}
			else
			{
				continue;
			}

		}

	}
	for(int i=wid*0.3;i>0;i--)
	{
	
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-i < wid)
			{
				labMat.data[labMat.step[0]*pt.y+pt.x-i] = 255;
				
			}
			else
			{
				break;
			}

		}

	}

	if(bShow)
	{
		imwrite("E:/pix3.bmp",labMat);
		//imshow("2.labMat",labMat);
	}


	if( iCountRight > iCountLeft && iCountRight*1.0/(wid*wid) > 0.08)
	{
		return true;
	}
	return false;
}

bool RectifyPtsSeg(Mat& labMat,Mat& outSeg,bool bShow)
{
	int wid = labMat.cols;

	//锟揭憋拷4.3锟斤拷
	vector<Point> linePts2;
	float k_1 = -(wid*0.65)*1.0/wid;
	for(int i = 0;i> -wid;--i )
	{
		int x = k_1*i;
		Point pt(wid+x,-i);
		linePts2.push_back(pt);
	}

	//开始统计像素点分布情况
	int tabValue[26]={0};

	for(int k = 0;k<wid*0.80;++k)
	{
		for(int m = 0;m<linePts2.size();++m)
		{
			Point pt = linePts2[m];
			if(pt.x-k-1 <wid)
			{
				tabValue[labMat.data[labMat.step[0]*pt.y+pt.x-k-1]/10]++;
			}
			else
			{
				break;
			}
		}
	}
	int max = 0;
	int maxK = 0;
	//找出最大值
	for(int k = 0;k<26;++k)
	{
		if(tabValue[k] > max)
		{
			max = tabValue[k] ;
			maxK = k;
		}

	}
	max = maxK*10+5;

	Mat tmp2 = labMat.clone();
	tmp2.setTo(0);
	for(int i=0;i<tmp2.rows;++i)
	{
		for(int j = 0;j<tmp2.cols;++j)
		{
			if(abs(labMat.data[tmp2.step[0]*i+j] -max) < 7 )
			{
				tmp2.data[tmp2.step[0]*i+j] = 255;
			}
		}
	}

	int dilation_type;
	dilation_type = MORPH_CROSS;
	int dilation_size = 1;
	 Mat element = getStructuringElement( dilation_type,
                                       Size( dilation_size + 1, dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );

	//进行腐蚀
	// tmp = tmp2.clone();
	//erode(tmp,tmp2,element);
	//dilate( tmp2, tmp2, element );


	//进行水漫操作
	for(int k = 0;k<4;++k)
	{
		int disSpan = 2;
		for(int i=0;i<tmp2.rows;++i)
		{
			for(int j = 0;j<tmp2.cols-disSpan;++j)
			{
				if(abs(labMat.data[tmp2.step[0]*i+j] -labMat.data[tmp2.step[0]*i+j+disSpan]) < 25
					&& tmp2.data[tmp2.step[0]*i+j+disSpan] == 255)
				{
					tmp2.data[tmp2.step[0]*i+j] = 255;
				}
			}
		}
	}
	
	//进行腐蚀
	 Mat tmp = tmp2.clone();
	erode(tmp,tmp2,element);

	outSeg = tmp2.clone();

	if(bShow)
	{	
		resize(tmp2,tmp2,Size(wid*2,wid*2));
		imwrite("E:/src.bmp",labMat);
		imwrite("E:/pix2.bmp",tmp2);
	}
	
	return FindSegLine(outSeg,bShow);

	

}


void FaceFilterRotate(vector<cv::Point>& mainPts, cv::Mat& clmResult)
{
	Point ptLeft;
	ptLeft.x = (mainPts[37].x + mainPts[38].x) / 2;
	ptLeft.y = (mainPts[37].y + mainPts[41].y + mainPts[38].y + mainPts[40].y) / 4;
	Point ptRight;
	ptRight.x = (mainPts[43].x + mainPts[44].x) / 2;
	ptRight.y = (mainPts[43].y + mainPts[47].y + mainPts[44].y + mainPts[46].y) / 4;


	int X = ptRight.x - ptLeft.x;
	int Y = ptRight.y - ptLeft.y;
	double R = sqrt(X*X + Y*Y*1.0);

	double angle = abs(asin(abs(Y / R))) * 180 / CV_PI;


	RotateMatAndPts(mainPts,clmResult,angle,Y);

}
