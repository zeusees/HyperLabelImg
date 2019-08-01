// BezierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Bezier.h"
#include "BezierDlg.h"
#include <fstream>
#include <iostream>
#include<Common\CommonWin.h>
#include <stdio.h>
#include "atlimage.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "ImageLayer.h"
#include "resource.h"

/*
  sunjunlishi

*/
using namespace std;
using namespace cv;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierDlg dialog

CBezierDlg::CBezierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBezierDlg::IDD, pParent)
{


	m_bMouseMove = false;
	m_leftBtnDown = false;
	m_bKeyOper = false;

	m_bDeleteFile = false;
	//移动矩形框
	mBMovingRect = false;

	m_nowMultiShow = 1.0;

	//鼠标点击起始位置
	mStartPt.x = 0;
	mStartPt.y = 0;
	//鼠标移动位置
	mNowPt.x = 0;
	mNowPt.y = 0;
	//新的初始化位置
	mInitPt.x = 0;
	mInitPt.y = 0;
	m_currentMark = -1;
}

void CBezierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBezierDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_LIST1, mListBox);
}

BEGIN_MESSAGE_MAP(CBezierDlg, CDialog)
	//{{AFX_MSG_MAP(CBezierDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()


	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_SAVE, &CBezierDlg::OnBnClickedBtnSave)
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOMMAND()

	

	ON_BN_CLICKED(IDC_BUTTON_IN, &CBezierDlg::OnBnClickedButtonIn)
	ON_WM_MOVING()
	ON_BN_CLICKED(IDC_BTN_FOLDER, &CBezierDlg::OnBnClickedBtnFolder)
	ON_LBN_SELCHANGE(IDC_LIST1, &CBezierDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BTN_DEL, &CBezierDlg::OnBnClickedBtnDel)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CBezierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_currentMark = -1;



	CRect rect;
	GetClientRect(rect);


	//关键点，仅仅就是一个
	m_points.push_back(cv::Point(500, 500));
	m_points.push_back(cv::Point(750, 500));
	m_points.push_back(cv::Point(750, 550));
	m_points.push_back(cv::Point(500, 550));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/*

	功能：画人脸关键点对应的 移动框
*/
void CBezierDlg::DrawPoint(int start,int end,CDC* pDC)
{
	
	// //用tmpNowPts做处理显示，m_points作为原始数据保持不变
	vector<Point2f> tmpNowPts;
	for (int k = 0; k < m_points.size(); ++k)
    {
		tmpNowPts.push_back(cv::Point2f(m_points[k].x*m_nowMultiShow, m_points[k].y*m_nowMultiShow));
	}


	int disX = mNowPt.x - mStartPt.x;
	int disY = mNowPt.y - mStartPt.y;
	if (!mBMovingRect)
	{
		disX = 0;
		disY = 0;
	}

	pDC->MoveTo(tmpNowPts[start].x+ disX, tmpNowPts[start].y+disY);
	for(int i = start; i<=end; i++)
	{
			CPen penStroke(PS_SOLID,1,0x007700);
			CPen *ppenPrevious=pDC->SelectObject(&penStroke);
			pDC->LineTo(tmpNowPts[i].x+disX, tmpNowPts[i].y+disY);
			if(i == end)
				pDC->LineTo(tmpNowPts[start].x+disX, tmpNowPts[start].y+disY);

			pDC->SelectObject(ppenPrevious);

			CPen penStroke1(PS_SOLID,1,0x00FFFF);
		    CPen penStroke2(PS_SOLID,1,0x0000FF);

			CPen *ppenPrevious2;
			if(m_currentMark == i)
			{
				ppenPrevious2 = pDC->SelectObject(&penStroke2);
			}
			else
			{
				ppenPrevious2 = pDC->SelectObject(&penStroke1);
			}
			

			pDC->SetBkMode(TRANSPARENT);
		
			cv::Point2d pt = cv::Point(tmpNowPts[i].x+disX, tmpNowPts[i].y+disY);
			CPoint tmpPts[4];
			int spanD = 6;
			tmpPts[0].x = pt.x - spanD;
			tmpPts[0].y = pt.y - spanD;

			tmpPts[1].x = pt.x + spanD;
			tmpPts[1].y = pt.y - spanD;
			tmpPts[2].x = pt.x + spanD;
			tmpPts[2].y = pt.y + spanD;
			tmpPts[3].x = pt.x - spanD;
			tmpPts[3].y = pt.y + spanD;
		
			
			 pDC->SelectStockObject(NULL_BRUSH);
			 pDC->Ellipse(tmpPts[0].x,tmpPts[0].y,tmpPts[2].x,tmpPts[2].y);

		
			pDC->SelectObject(ppenPrevious2);
	}
	
}
/*

功能：本绘制主要是  因为在进行微调关键点的时候，防止闪烁， 采用双缓冲区绘制
  绘制 背景待处理图片，然后绘制关键点，绘制到缓冲区，接着整体绘制
  
*/
void CBezierDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	}
	else
	{
		CDialog::OnPaint();

		CDC * pDC = GetDC();
		int iCanvaW = 1560;
		if(!m_nowImg.empty() && pDC)
		{

			   CImage ImageBk;
			   ImageBk.Create(iCanvaW, 960, 24);
				CImage Image;
				cv::Mat dst = m_nowImg.clone();
				cv::resize(dst, dst, cv::Size(m_nowImg.cols*m_nowMultiShow, m_nowImg.rows*m_nowMultiShow));


				Image.Create(dst.cols,dst.rows,24);
			
				int i;	int j;

				for (i = 0; i < Image.GetHeight(); i++)
				{
					int step = dst.step[0]*i;
					int jump = 0;
					for (j = 0; j < Image.GetWidth(); j++)
					{
						byte *pByte = (byte *)Image.GetPixelAddress(j,i);
						pByte[0] = (unsigned char)dst.data[step+jump+0];
						pByte[1] = (unsigned char)dst.data[step+jump+1];
						pByte[2] = (unsigned char)dst.data[step+jump+2];
		
						jump+=3;
					}
				}


				CDC   MemDC;   //首先定义一个显示设备对象 
				CBitmap   MemBitmap;//定义一个位图对象 

				pDC->SetBkMode(TRANSPARENT);
				//随后建立与屏幕显示兼容的内存显示设备 
				MemDC.CreateCompatibleDC(pDC); 
				//这时还不能绘图，因为没有地方画   ^_^ 
				//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小 
				MemBitmap.CreateCompatibleBitmap(pDC, iCanvaW,960);
			
				//将位图选入到内存显示设备中 
				//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上 
				CBitmap  *pOldBit=MemDC.SelectObject(&MemBitmap); 

				ImageBk.Draw(MemDC.m_hDC, 0, 0, iCanvaW,960);
				if (mBMovingRect || m_currentMark >=0)
				{//仅仅移动框 或者 移动点的情况下，背景画面不需要移动
					Image.Draw(MemDC.m_hDC, mInitPt.x, mInitPt.y,Image.GetWidth(), Image.GetHeight());
				}
				else
				Image.Draw(MemDC.m_hDC, mInitPt.x + mNowPt.x - mStartPt.x, mInitPt.y + mNowPt.y - mStartPt.y, 
					Image.GetWidth(),Image.GetHeight());

				if(m_points.size() > 0) 
				{
					//脸轮廓
					DrawPoint(0,3,&MemDC);
			
				}
				//将内存中的图拷贝到屏幕上进行显示 
				pDC->BitBlt(0,0 ,
					iCanvaW, 960,&MemDC,0,0,SRCCOPY);


				MemDC.SelectObject(pOldBit);
				//绘图完成后的清理 
				MemBitmap.DeleteObject(); 
				MemDC.DeleteDC();

				ReleaseDC(pDC);

		}

	}
}

void CBezierDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	mStartPt = point;
	mNowPt = point;
	m_leftBtnDown = true;
	// TODO: Add your message handler code here and/or call default
	m_currentMark = -1;

	// //用tmpNowPts做处理显示，m_points作为原始数据保持不变
	vector<Point2f> tmpNowPts;
	for (int k = 0; k < m_points.size(); ++k)
	{
		tmpNowPts.push_back(cv::Point2f(m_points[k].x*m_nowMultiShow, m_points[k].y*m_nowMultiShow));
	}

		double x, y;
		double t=56;
		for(int i = 0; i < tmpNowPts.size(); i++)
		{
			x = tmpNowPts[i].x - point.x, y = tmpNowPts[i].y - point.y;
			 x*=x; y*=y;
			if(x + y < t)
			{	
				m_currentMark = i;		
			}

		}

		
		//判断是否移动方框
		if (cv::pointPolygonTest(tmpNowPts, cv::Point(point.x, point.y), true) > 4)
		{
				mBMovingRect = true;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		}
		else
		{
				mBMovingRect = false;
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		
	
	CDialog::OnLButtonDown(nFlags, point);
	//RedrawWindow();
}

void CBezierDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//所有的数据进行初始化或者更新
	if ( m_leftBtnDown )
	{
		//说明不是移动点，则是整张图片的移动
		if (m_currentMark < 0 && !mBMovingRect)
		{
			//把原始位置进行保存
			mInitPt.x = mInitPt.x + mNowPt.x - mStartPt.x;
			mInitPt.y = mInitPt.y + mNowPt.y - mStartPt.y;
		}

		if ( mBMovingRect)
		{
			for (int k = 0; k < m_points.size(); ++k)
			{
				m_points[k].x += (mNowPt.x - mStartPt.x)/m_nowMultiShow;
				m_points[k].y += (mNowPt.y - mStartPt.y) / m_nowMultiShow;
			}
			mBMovingRect = false;
		}
	}


	mNowPt.x = 0;
	mNowPt.y = 0;
	mStartPt.x = 0;
	mStartPt.y = 0;

	m_leftBtnDown = false;
	// TODO: Add your message handler code here and/or call default
	m_bMouseMove = false;
	m_currentMark = -1;

	CDialog::OnLButtonUp(nFlags, point);
}

void CBezierDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// //距离边界大于4个像素
	// //用tmpPts做处理显示，m_points作为原始数据保持不变
	//vector<Point2f> tmpPts;
	//for (int k = 0; k < m_points.size(); ++k)
	//{
	//	tmpPts.push_back(cv::Point2f(m_points[k].x*m_nowMultiShow, m_points[k].y*m_nowMultiShow));
	//}

	if (m_leftBtnDown)
	{
		mNowPt = point;
		if (m_currentMark >= 0)
		{//移动关键点
			m_points[m_currentMark].x = mNowPt.x/ m_nowMultiShow;
			m_points[m_currentMark].y = mNowPt.y/ m_nowMultiShow;
		}
		RedrawWindow();
	}
	
	if (mBMovingRect)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

	}
	else
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}


	CDialog::OnMouseMove(nFlags, point);
}






/*

打开一个文件，所有的值进行归0

*/
void CBezierDlg::DoGetPtsAndDraw(CString fileName)
{
	mInitPt.x = 0;
	mInitPt.y = 0;

	mStartPt.x = 0;
	mStartPt.y = 0;

	mNowPt.x = 0;
	mNowPt.y = 0;

	m_points[0].x = 10;
	m_points[0].y = 10;
	m_points[1].x = 400;
	m_points[1].y = 10;
	m_points[2].x = 400;
	m_points[2].y = 100;
	m_points[3].x = 10;
	m_points[3].y = 100;


	m_nowMultiShow = 1.0;
	   m_fileName=fileName; //文件名保存在了FilePathName里
	   m_ptsName = m_fileName;

	   m_srcImg = cv::imread(m_fileName.GetBuffer(0));
	   m_nowImg = m_srcImg.clone();
	   Invalidate();
	
		int index = m_ptsName.Replace(".jpg",".pts");
		if(index == 0)
		{
			index = m_ptsName.Replace(".png",".pts");
			if(index == 0)
			{
				index = m_ptsName.Replace(".bmp",".pts");
			}
		}

		ifstream locations(m_ptsName.GetBuffer(0), ios_base::in);
		if(locations.is_open())
		{
			string line;
			// The main file contains the references to other files
			while (!locations.eof())
			{ 
				
				for(int i=0;i<4;i++)
				{
					locations>>m_points[i].x;
					locations>>m_points[i].y;
				}
				getline(locations, line);
			}
		 
			
		}
		

}
void CBezierDlg::OnBnClickedBtnImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
    CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("pic Files (*.jpg)|*.jpg|(*.bmp)|*.bmp|(*.png)|*.png|All Files (*.*)|*.*||"),
        NULL);
    if(dlg.DoModal()==IDOK)
    {
		DoGetPtsAndDraw(dlg.GetPathName());
		
	}
}


void CBezierDlg::OnBnClickedBtnSave()
{

		char szBuf[256];
		sprintf(szBuf, "%s", m_ptsName.GetBuffer(0));
		fstream locations(m_ptsName.GetBuffer(0), ios_base::out);
		for (int i = 0; i < m_points.size(); i++)
		{
			locations << m_points[i].x << " " << m_points[i].y << "\n";
		}
		//locations << "}" << "\n";



}


BOOL CBezierDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!m_leftBtnDown)
	{
		if(m_bKeyOper)
		{
			m_bKeyOper = false;return 1;
		}
		else
		{
			return CDialog::OnEraseBkgnd(pDC);
		}
	}
	else
	{
		return 1;
	}
}



void CBezierDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nID == 61488)
	{
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		MoveWindow(0,0,cx,cy);

		GetDlgItem(IDC_BTN_ORIGIN)->MoveWindow(cx-240,5,110,40);
		GetDlgItem(IDC_BTN_BIG)->MoveWindow(cx-120,5,110,40);

		GetDlgItem(IDC_STATIC_PIC1)->MoveWindow(cx-240,55,240,2);
		GetDlgItem(IDC_BTN_SAVE)->MoveWindow(cx-120,60,110,40);
		GetDlgItem(IDC_STATIC_PIC)->MoveWindow(cx-240,115,240,2);


	
		Invalidate();
	}
	CDialog::OnSysCommand(nID, lParam);
}



BOOL CBezierDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 0x53&& 
			(pMsg->lParam == 0x401f0001
				|| pMsg->lParam == 0x1f0001))
		{
			//AfxMessageBox("ctrl s");
			OnBnClickedBtnSave();
		}
		char szBuf[256];
		sprintf(szBuf, "%x %x\n", pMsg->wParam, pMsg->lParam);
		OutputDebugString(szBuf);
	}
	if(pMsg->message == WM_KEYUP)
	{
			int span = 1;
			switch (pMsg->wParam)
			{
			case VK_UP:
				{
					if(m_currentMark > 0)
					{
						m_points[m_currentMark].y-=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
				}
				break;
			case VK_DOWN:
				if(m_currentMark > 0)
					{
						m_points[m_currentMark].y+=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
				break;
			case VK_RIGHT:
				if(m_currentMark > 0)
					{
						m_points[m_currentMark].x+=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
				break;
			case VK_LEFT:
				if(m_currentMark > 0)
					{
						m_points[m_currentMark].x-=span;
						m_bKeyOper = true;
						RedrawWindow();
					}
			
				break;
			default:
				break;
			}
			

		
		
	}

	if(pMsg->wParam == VK_UP
				|| pMsg->wParam == VK_DOWN
				|| pMsg->wParam == VK_LEFT
				|| pMsg->wParam == VK_RIGHT)
			{
				if(m_currentMark > 0)
				{
					return 1;
				}
			}

	return CDialog::PreTranslateMessage(pMsg);
}






void CBezierDlg::DrawThePicPoint(cv::Mat clmResult, int posX, int posY, int dstW)
{

	//若使用前不想把原来绘制的图片去掉，可以删去上面那三段
	CDC *pDC = GetDC();

	int biaoWidht = dstW;
	cv::Mat dst;
	if (clmResult.cols > dstW)
	{
		float ratio = clmResult.cols*1.0 / biaoWidht;
		int height = clmResult.rows / ratio;

		resize(clmResult, dst, cv::Size(biaoWidht, height));
	}
	else
	{
		dst = clmResult.clone();
	}




	CImage Image;
	Image.Create(dst.cols, dst.rows, 24);
	int i;
	int j;
	for (i = 0; i < Image.GetHeight(); i++)
	{
		int step = dst.step[0] * i;
		int jump = 0;
		for (j = 0; j < Image.GetWidth(); j++)
		{
			byte *pByte = (byte *)Image.GetPixelAddress(j, i);

			if (dst.channels() != 1)
			{
				pByte[0] = (unsigned char)dst.data[step + jump + 0];
				pByte[1] = (unsigned char)dst.data[step + jump + 1];
				pByte[2] = (unsigned char)dst.data[step + jump + 2];

				jump += 3;
			}
			else
			{
				pByte[0] = (unsigned char)dst.data[step + jump + 0];
				pByte[1] = (unsigned char)dst.data[step + jump + 0];
				pByte[2] = (unsigned char)dst.data[step + jump + 0];

				jump += 1;
			}

		}
	}

	Image.Draw(pDC->m_hDC, posX, posY);
	Image.Destroy();

	ReleaseDC(pDC);
}
void CBezierDlg::OnBnClickedButtonIn()
{

	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("pic Files (*.jpg)|*.jpg|(*.bmp)|*.bmp|(*.png)|*.png|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		DoGetPtsAndDraw(dlg.GetPathName());
	
	}
}




void CBezierDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialog::OnMoving(fwSide, pRect);

	CRect rect;
	GetWindowRect(rect);

	
	// TODO: 在此处添加消息处理程序代码
}





int truncateUint8(int value)
{
	if (value < 0) value = 0;
	if (value > 255) value = 255;
	return value;
}

void CBezierDlg::ParseFiles(CString foldername, std::vector<std::string>& outfiles)
{

	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind, foldername);
	strcat(szFind, "\\*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;

	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{

			}
		}
		else
		{
			CString strTmp = foldername + "/";
			strTmp += FindFileData.cFileName;
			if(strTmp.Find(".jpg") != -1)
			outfiles.push_back(strTmp.GetBuffer(0));
		}


		if (!FindNextFile(hFind, &FindFileData))
			break;
		//
	}

	FindClose(hFind);
	
}


void CBezierDlg::OnBnClickedBtnFolder()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);

	//遍历文件夹添加文件
	vector<std::string> files;
	ParseFiles(szBuffer, files);

	mListBox.ResetContent();
	for (int k = 0; k < files.size(); ++k)
	{
		mListBox.AddString(files[k].c_str());
	}
}


void CBezierDlg::OnLbnSelchangeList1()
{
	int iCurSel = mListBox.GetCurSel();
	//得到当前选择索引
	if (LB_ERR != iCurSel)
	{
		CString strTmp;
		mListBox.GetText(iCurSel, strTmp);

		//选中之后一切归0
		DoGetPtsAndDraw(strTmp);

		//得到当前选择的字符串
		SetWindowText(strTmp);
	}
									
	
}


void CBezierDlg::OnBnClickedBtnDel()
{
	int iCurSel = mListBox.GetCurSel();
	//得到当前选择索引
	if (LB_ERR != iCurSel)
	{
		CString strTmp;
		mListBox.GetText(iCurSel, strTmp);
		DeleteFile(strTmp);
		mListBox.DeleteString(iCurSel);
	}
}


BOOL CBezierDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (zDelta > 0)
	{
		m_nowMultiShow += 0.1;
	}
	else
	{
		m_nowMultiShow -= 0.1;
		if (m_nowMultiShow < 1) m_nowMultiShow = 1.0;

	}
	
	RedrawWindow();
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
