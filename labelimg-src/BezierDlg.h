// BezierDlg.h : header file
//

#if !defined(AFX_BEZIERDLG_H__78532247_8543_44EB_AE32_706C9CD0D646__INCLUDED_)
#define AFX_BEZIERDLG_H__78532247_8543_44EB_AE32_706C9CD0D646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/*
      本界面类是主类 主要提供显示68个展示功能，微调功能(按上下左右键进行微调)；
	  自动保存pts文件在当前路径下；
	  可以打开文件夹，批处理图片

	  代码写的比较凌乱，还望海涵，但是我尽量删除多余重复代码
	  
*/
// CBezierDlg dialog
#include <opencv2\core.hpp>
#include "afxwin.h"


using namespace std;

class CBezierDlg : public CDialog
{
// Construction
public:

	CBezierDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBezierDlg)
	enum { IDD = IDD_BEZIER_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBezierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:

	CPaintDC * pDC;
	int  m_currentMark;
	vector<cv::Point2f> m_points;
	CString m_fileName;
	CString m_ptsName;
	bool m_bDeleteFile;
	bool m_bMouseMove;
	bool m_leftBtnDown;
	bool m_bKeyOper;
	int m_nowMulti;

	cv::Mat m_srcImg;
	cv::Mat m_nowImg;

	CPoint mInitPt;

	CPoint mStartPt;
	CPoint mNowPt;
	BOOL mBMovingRect;
private:
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedBtnSave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnBig();
	afx_msg void OnBnClickedBtnOrigin();

	
	void DrawPoint(int start,int end,CDC * pDc);

	void DoGetPtsAndDraw(CString fileName);

	//对文件夹，解析出里面的文件
	void ParseFiles(CString foldername, std::vector<std::string>& outfiles);

	// Generated message map functions
	//{{AFX_MSG(CBezierDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void DrawThePicPoint(cv::Mat clmResult, int posX, int posY, int dstW);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnBnClickedButtonIn();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnBnClickedBtnFolder();
	CListBox mListBox;
	afx_msg void OnLbnSelchangeList1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEZIERDLG_H__78532247_8543_44EB_AE32_706C9CD0D646__INCLUDED_)
