// Bezier.h : main header file for the BEZIER application
//

#if !defined(AFX_BEZIER_H__204A6E7B_7E65_43F2_8E44_1035D440BD3E__INCLUDED_)
#define AFX_BEZIER_H__204A6E7B_7E65_43F2_8E44_1035D440BD3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBezierApp:
// See Bezier.cpp for the implementation of this class
//

class CBezierApp : public CWinApp
{
public:
	CBezierApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBezierApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBezierApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEZIER_H__204A6E7B_7E65_43F2_8E44_1035D440BD3E__INCLUDED_)
