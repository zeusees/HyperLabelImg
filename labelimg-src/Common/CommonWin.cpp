
#include "CommonWin.h"
#include <string>
#include <strsafe.h>
#include <tlhelp32.h> 
#include <ShlGuid.h>

#pragma comment(lib,"iphlpapi.lib")

bool gbCompleteWorkFemale = false;//自动下载
bool gbCompleteWorkMale = false;//自动下载
bool gbCompleteUpdate = false;//自动更新

CString ShortName(CString name)
{
	return "";
}
CString GetExePath()
{
	//获得执行文件的路径
	char szCurPath[MAX_PATH] = {0};
	char *pFind = NULL;
	GetModuleFileName(NULL, szCurPath, sizeof(szCurPath));
	pFind = strrchr(szCurPath, '\\');
	if(pFind == NULL)
	{
		pFind = strrchr(szCurPath, '/');
	}
	if(pFind)
	{
		memset((void *)pFind, 0x00, strlen(pFind));
	}
	

	return szCurPath;
}

void DrawMyTextType(CString type,CString text,
					int posX,int posY,CWnd * pWnd,int FontSize)
{
	CDC * pDc = pWnd->GetDC();
	if(pDc)
	{
			CFont font;
		font.CreateFont(
		FontSize, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T(type.GetBuffer(0)) // nPitchAndFamily Arial
		); 
		CFont * oldFont = pDc->SelectObject(&font);

		pDc->SetTextColor(RGB(0,0,0));

		pDc->SetBkColor(RGB(240,240,240));

		pDc->DrawText(text,CRect(posX,posY,posX+1000,posY+FontSize+20),DT_SINGLELINE|DT_LEFT|DT_TOP); //显示文本
		pDc->SelectObject(oldFont);
		pWnd->ReleaseDC(pDc);
	}

}

void DrawMyText(CString text,int posX,int posY,CDC* pDc,int FontSize,COLORREF textColor)
{

	CFont font;
	font.CreateFont(
	FontSize, // nHeight
	0, // nWidth
	0, // nEscapement
	0, // nOrientation
	FW_NORMAL, // nWeight
	FALSE, // bItalic
	FALSE, // bUnderline
	0, // cStrikeOut
	ANSI_CHARSET, // nCharSet
	OUT_DEFAULT_PRECIS, // nOutPrecision
	CLIP_DEFAULT_PRECIS, // nClipPrecision
	DEFAULT_QUALITY, // nQuality
	DEFAULT_PITCH | FF_SWISS,
	_T("微软雅黑") // nPitchAndFamily Arial
	); 
	CFont * oldFont = pDc->SelectObject(&font);

	pDc->SetTextColor(textColor);
	pDc->SetBkColor(RGB(240,240,240));
	pDc->SetBkMode(TRANSPARENT);

	pDc->DrawText(text,CRect(posX,posY,posX+1000,posY+60),DT_SINGLELINE|DT_LEFT|DT_TOP); //显示文本
	pDc->SelectObject(oldFont);
}

void DrawMyTextBK(CString text,int posX,int posY,CWnd * pWnd,COLORREF bkColor,int FontSize,COLORREF textColor)
{
	CDC * pDc = pWnd->GetDC();
	if(pDc)
	{
		CFont font;
		font.CreateFont(
		FontSize, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("微软雅黑") // nPitchAndFamily Arial
		); 
		CFont * oldFont = pDc->SelectObject(&font);

		CRect shortrect;
		shortrect.left = posX;
		shortrect.top = posY;

		shortrect.right = shortrect.left + text.GetLength()*FontSize;
		shortrect.bottom = shortrect.top + FontSize+10;

		pDc->SetTextColor(textColor);
		pDc->FillSolidRect(shortrect,bkColor);
		//pDc->SetBkColor(RGB(240,240,240));
		//pDc->SetBkMode(TRANSPARENT);

		pDc->DrawText(text,CRect(posX,posY,posX+1000,posY+60),DT_SINGLELINE|DT_LEFT|DT_TOP); //显示文本
		pDc->SelectObject(oldFont);

		pWnd->ReleaseDC(pDc);
	}

}
void DrawMyText(CString text,int posX,int posY,CWnd * pWnd,int FontSize,COLORREF textColor)
{
	CDC * pDc = pWnd->GetDC();
	if(pDc)
	{
		CFont font;
		font.CreateFont(
		FontSize, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("微软雅黑") // nPitchAndFamily Arial
		); 
		CFont * oldFont = pDc->SelectObject(&font);

		pDc->SetTextColor(textColor);
		pDc->SetBkColor(RGB(240,240,240));
		pDc->SetBkMode(TRANSPARENT);

		pDc->DrawText(text,CRect(posX,posY,posX+1000,posY+60),DT_SINGLELINE|DT_LEFT|DT_TOP); //显示文本
		pDc->SelectObject(oldFont);

		pWnd->ReleaseDC(pDc);
	}

}
	
void ContructBih(int nWidth,int nHeight,BITMAPINFOHEADER& bih,int bit)
{
	bih.biSize=40; 						// header size
	bih.biWidth=nWidth;
	bih.biHeight=nHeight;
	bih.biPlanes=1;
	bih.biBitCount=bit;					// RGB encoded, 24 bit
	bih.biCompression=BI_RGB;			// no compression
	bih.biSizeImage=nWidth*nHeight*(bit/8);
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
	bih.biClrUsed=0;
	bih.biClrImportant=0;
}

void DrawBmpBuf(BITMAPINFOHEADER& bih,unsigned char* pDataBuf,HWND hShowWnd,CWnd * pWnd,char * szText,int posX,int posY)
{
	RECT rc;
	::GetWindowRect( hShowWnd, &rc );
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	BITMAPINFO bi;
	memset(&bi,0,sizeof(bi));
	memcpy( &(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER) );
	int iWidth=bih.biWidth;
	int iHeight=bih.biHeight;

	// display bitmap
	HDC hdcStill = ::GetDC( hShowWnd );
	PAINTSTRUCT ps;
	::BeginPaint( hShowWnd, &ps );

	::SetStretchBltMode( hdcStill, COLORONCOLOR );


	::StretchDIBits( hdcStill, 0, 0, nWidth, nHeight,
	0, 0, iWidth, iHeight, pDataBuf, &bi,
	DIB_RGB_COLORS,	SRCCOPY );

	//DrawText(hdcStill,szText,strlen(szText),CRect(CPoint(0,0),CSize(200,40)),0);

	DrawMyText(szText,posX,posY,pWnd,60,RGB(255,0,0));

	::EndPaint( hShowWnd, &ps );
	::ReleaseDC( hShowWnd, hdcStill );
}




void DrawBmpBuf(BITMAPINFOHEADER& bih,unsigned char* pDataBuf,HWND hShowWnd,int posX,int posY)
{
	RECT rc;
	::GetWindowRect( hShowWnd, &rc );
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	BITMAPINFO bi;
	memset(&bi,0,sizeof(bi));
	memcpy( &(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER) );
	int iWidth=bih.biWidth;
	int iHeight=bih.biHeight;

	// display bitmap
	HDC hdcStill = ::GetDC( hShowWnd );
	PAINTSTRUCT ps;
	::BeginPaint( hShowWnd, &ps );

	::SetStretchBltMode( hdcStill, COLORONCOLOR );


	::StretchDIBits( hdcStill, posX, posY, nWidth, nHeight,
	0, 0, iWidth, iHeight, pDataBuf, &bi,
	DIB_RGB_COLORS,	SRCCOPY );


	::EndPaint( hShowWnd, &ps );
	::ReleaseDC( hShowWnd, hdcStill );
}


/////////////////////////////////////////////////////////////////////////////
HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color,CWnd * pWnd)
{
	if (!hBmp) return NULL;

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm );	// get bitmap attributes

	CDC dcBmp;
	CDC * pDC = pWnd->GetDC();
	if (!pDC) return NULL;
	dcBmp.CreateCompatibleDC(pDC);	//Creates a memory device context for the bitmap
	pWnd->ReleaseDC(pDC);
	HBITMAP hOldBitmap = (HBITMAP)dcBmp.SelectObject(hBmp);	//selects the bitmap in the device context

	const DWORD RDHDR = sizeof(RGNDATAHEADER);
	const DWORD MAXBUF = 40;		// size of one block in RECTs
									// (i.e. MAXBUF*sizeof(RECT) in bytes)
	LPRECT	pRects;								
	DWORD	cBlocks = 0;			// number of allocated blocks

	INT		i, j;					// current position in mask image
	INT		first = 0;				// left position of current scan line
									// where mask was found
	bool	wasfirst = false;		// set when if mask was found in current scan line
	bool	ismask;					// set when current color is mask color

	// allocate memory for region data
	RGNDATAHEADER* pRgnData = (RGNDATAHEADER*)new unsigned char[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
	memset( pRgnData, 0, RDHDR + cBlocks * MAXBUF * sizeof(RECT) );
	// fill it by default
	pRgnData->dwSize	= RDHDR;
	pRgnData->iType		= RDH_RECTANGLES;
	pRgnData->nCount	= 0;
	for ( i = 0; i < bm.bmHeight; i++ )
	for ( j = 0; j < bm.bmWidth; j++ ){
		// get color
		ismask=(dcBmp.GetPixel(j,bm.bmHeight-i-1)!=color);
		// place part of scan line as RECT region if transparent color found after mask color or
		// mask color found at the end of mask image
		if (wasfirst && ((ismask && (j==(bm.bmWidth-1)))||(ismask ^ (j<bm.bmWidth)))){
			// get offset to RECT array if RGNDATA buffer
			pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
			// save current RECT
			pRects[ pRgnData->nCount++ ] = CRect( first, bm.bmHeight - i - 1, j+(j==(bm.bmWidth-1)), bm.bmHeight - i );
			// if buffer full reallocate it
			if ( pRgnData->nCount >= cBlocks * MAXBUF ){
				LPBYTE pRgnDataNew = new unsigned char[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
				memcpy( pRgnDataNew, pRgnData, RDHDR + (cBlocks - 1) * MAXBUF * sizeof(RECT) );
				delete pRgnData;
				pRgnData = (RGNDATAHEADER*)pRgnDataNew;
			}
			wasfirst = false;
		} else if ( !wasfirst && ismask ){		// set wasfirst when mask is found
			first = j;
			wasfirst = true;
		}
	}
	dcBmp.SelectObject( hOldBitmap );
	dcBmp.DeleteDC();	//release the bitmap
	// create region
/*  Under WinNT the ExtCreateRegion returns NULL (by Fable@aramszu.net) */
//	HRGN hRgn = ExtCreateRegion( NULL, RDHDR + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
/* ExtCreateRegion replacement { */
	HRGN hRgn=CreateRectRgn(0, 0, 0, 0);
	ASSERT( hRgn!=NULL );
	pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
	for(i=0;i<(int)pRgnData->nCount;i++)
	{
		HRGN hr=CreateRectRgn(pRects[i].left, pRects[i].top, pRects[i].right, pRects[i].bottom);
		VERIFY(CombineRgn(hRgn, hRgn, hr, RGN_OR)!=ERROR);
		if (hr) DeleteObject(hr);
	}
	ASSERT( hRgn!=NULL );
/* } ExtCreateRegion replacement */

	delete pRgnData;
	return hRgn;
}

void TraverseDir(CString dir, std::vector<std::string>& vec)

{
    CFileFind finder;
	CString strFindFolder = dir + "/*.*";
	bool bWorking = finder.FindFile(strFindFolder,0);
	int iItem = 0;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;		// Not a file
		
		CString strFoundFile = finder.GetFilePath();
		vec.push_back(strFoundFile.GetBuffer(0));
	}
}
void TraverseDirShortName(CString dir, std::vector<std::string>& vec)

{
    CFileFind finder;
	CString strFindFolder = dir + "/*.*";
	bool bWorking = finder.FindFile(strFindFolder,0);
	int iItem = 0;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;		// Not a file
		
		CString strFoundFile = finder.GetFileName();
		vec.push_back(strFoundFile.GetBuffer(0));
	}
}
void GetMacString(std::string& strMac,std::string& strIP)
{
	CString strGateWay,strSubnetMask;
	 u_char pMac[6];
	 PIP_ADAPTER_INFO adp = NULL;
	 ULONG uLong=0;
	 //为适配器申请内存
	::GetAdaptersInfo(adp,&uLong);
	 adp = (PIP_ADAPTER_INFO)new unsigned char[uLong];

	 //取得本地适配器结构信息
	if(::GetAdaptersInfo(adp,&uLong) == ERROR_SUCCESS)
	{
		
		int index = 0;
		 while(adp != NULL)
		 {
			 bool bFindWireLess = false;
			 CString des = adp->Description;

			 des.MakeLower();

			 if( des.Find("wireless") != -1
				 || des.Find("wifi") != -1
				 || des.Find("无线") != -1)
			 {
				 bFindWireLess = true;
			 }
			//bFindWireLess |||| adp->Type == 71
			 if( index == 0 )
			 {
				 //strMacAdd.Format("%s",adp->Address);
				 memcpy(pMac,adp->Address,6);
				 char szBuf[512];
				 sprintf_s(szBuf,"%02X-%02X-%02X-%02X-%02X-%02X",pMac[0],pMac[1],pMac[2],pMac[3],pMac[4],pMac[5]);
				 strMac = szBuf;

				 strGateWay.Format("%s",adp->GatewayList.IpAddress.String);// 网关

				 sprintf_s(szBuf,"%s",adp->IpAddressList.IpAddress.String);//IP
				 strIP = szBuf;

				 strSubnetMask.Format("%s",adp->IpAddressList.IpMask.String);//子网掩码
				 break;
			 }
			
			

			
			 adp = adp->Next;
			 index++;
		 }
	  }
	 delete adp;

}

bool DeleteDirectory(LPCTSTR DirName)
{
	CFileFind tempFind; //声明一个CFileFind类变量，以用来搜索
	char tempFileFind[200]; //用于定义搜索格式
	sprintf_s(tempFileFind,"%s\\*.*",DirName);
	//匹配格式为*.*,即该目录下的所有文件 

	bool IsFinded=(bool)tempFind.FindFile(tempFileFind);
	//查找第一个文件
	while(IsFinded)
	{
		IsFinded=(bool)tempFind.FindNextFile(); //递归搜索其他的文件
		if(!tempFind.IsDots()) //如果不是"."目录
		{
			char foundFileName[200];
			strcpy_s(foundFileName,tempFind.GetFileName().GetBuffer(200));
			if(tempFind.IsDirectory()) //如果是目录，则递归地调用
			{ 
				//DeleteDirectory
				char tempDir[200];
				sprintf_s(tempDir,"%s\\%s",DirName,foundFileName);
				DeleteDirectory(tempDir);
			}
			else
			{ 
				//如果是文件则直接删除之
				char tempFileName[200];
				sprintf_s(tempFileName,"%s\\%s",DirName,foundFileName);
				DeleteFile(tempFileName);
			}
		}
	}
	tempFind.Close();
	if(!RemoveDirectory(DirName)) //删除目录
	{
		return FALSE;
	}
	return TRUE;
} 



bool CreateMultipleDirectory(const CString& szPath)
{
	char splitCode = '\\';
	if(szPath.Find(splitCode) == -1)
	{
		splitCode = '/';
	}


	CString strDir(szPath);//存放要创建的目录字符串
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=splitCode)
	{
		strDir.AppendChar(splitCode);
	}
	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	bool bSuccess = false;//成功标志
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != splitCode) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(splitCode);
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}
	return bSuccess;
}




time_t FileTimeToTime_t(const FILETIME &ft)
{
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	return ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
}

int GetDiffSeconds(const SYSTEMTIME &t1, const SYSTEMTIME &t2)
{
	//FILETIME fTime1 = { 0, 0 };
	//FILETIME fTime2 = { 0, 0 };
	//SystemTimeToFileTime(&t1, &fTime1);
	//SystemTimeToFileTime(&t2, &fTime2);

	//time_t tt1 = FileTimeToTime_t(fTime1);
	//time_t tt2 = FileTimeToTime_t(fTime2);

	return (t2.wMinute*60*1000-t1.wMinute*60*1000+t2.wSecond*1000 - t1.wSecond*1000+t2.wMilliseconds-t1.wMilliseconds);
}

int GetDiffDays(const SYSTEMTIME &t1, const SYSTEMTIME &t2)
{
	int diffSeconds = GetDiffSeconds(t1, t2);
	return diffSeconds / (24 * 3600);
}



int InitIEOption()
{
	HKEY hKey;     
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges"; 
	//DWORD state,dwtype,sizeBuff;
	char reBuff[10] = {0};
	CString str;
	long lRet=0;


	/*读取键值*************************************************************************/
	//开机启动
	//---------------找到系统的启动项 
	lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\3"; 
	lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_READ|KEY_WRITE, &hKey);
	//-----------打开启动项Key  
	if(lRet == ERROR_SUCCESS) 
	{ 
		DWORD dwData=0;
		lRet = RegSetValueEx(hKey, _T("160A"),0,REG_DWORD,(unsigned char *)&dwData,sizeof(dwData));
		if(lRet!=ERROR_SUCCESS)
		{
			//HKEY hSub;
			//lRet=RegCreateKey(hKey,"Range2",&hSub);
			//DEBUG_TRACE("RegCreateKey lRet=%d",lRet);

			//lRet = RegSetValueEx(hSub, _T(":Range"),0,REG_SZ,(unsigned char *)"192.168.18.101",strlen("192.168.18.101")*2);
			//DEBUG_TRACE("RegSetValueEx lRet=%d",lRet);

			//DWORD dwData=2;
			//lRet = RegSetValueEx(hSub, _T("http"),0,REG_DWORD,(unsigned char *)&dwData,sizeof(dwData));
			//DEBUG_TRACE("RegSetValueEx lRet=%d",lRet);
		}
		RegCloseKey(hKey);
		//----------------关闭注册表
	}
	else
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
			FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,NULL);
		//DEBUG_TRACE((char*)lpMsgBuf);
	}


	return 0;
}

//
//判断字符串是否全是数字
//
bool IsNumber( LPCTSTR pszText )
{
	for( int i = 0; i < lstrlen( pszText ); i++ )
		if( !_istdigit( pszText[ i ] ) )
			return FALSE;

	return TRUE;
}

//
//判断字符串是否全是数字
//
bool IsDigital(LPCTSTR lpszSrc)
{
	CString Src = lpszSrc;
	return (Src ==  Src.SpanIncluding( _T("0123456789" ) ));
}

void DEBUG_STRING(const char *fmt,...)
{
	int nBuf;
	char szBuffer[1024];
	va_list args;

	va_start(args, fmt);
	nBuf = vsprintf_s(szBuffer, fmt, args) ;
	ASSERT(nBuf >= 0);
	OutputDebugString(szBuffer);
	va_end(args);
}


unsigned long GetFileSize(const char *filename)
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	DWORD fileSize=0;
	hFind = FindFirstFile(filename ,&fileInfo);
	if(hFind != INVALID_HANDLE_VALUE)
		fileSize = fileInfo.nFileSizeLow;
	FindClose(hFind);
	return fileSize;
}






CString GetFileTitleFromFileName(CString FileName,CString& ExtendName) //获取文件名和扩展名
{
	int Where;
	Where=FileName.ReverseFind('\\');
	if(Where==-1)
	{
		Where=FileName.ReverseFind('/');
	}
	CString FileTitle=FileName.Right(FileName.GetLength()-1-Where);
	int Which=FileTitle.ReverseFind('.');
	ExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);
	if (Which!=-1)
	{
		FileTitle=FileTitle.Left(Which);
	}
	return FileTitle;
}


#define BUFSIZE 256

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef bool (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);




//
//设置字符串值函数
//
int SetValue_S(HKEY ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,LPBYTE ReSetContent_S)
{
	int i=0;	//操作结果：0==succeed
	HKEY hKey; 
	LONG lRet=0;

	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_SZ,ReSetContent_S,CString(ReSetContent_S).GetLength())!=ERROR_SUCCESS)
		{
			// 			AfxMessageBox("错误：无法设置有关的注册表信息");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		// 		AfxMessageBox("错误：无法查询有关的注册表信息");
		i=1;
	}
	return i;
}

//
//设置DWORD值函数
//
int SetValue_D(HKEY ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName,int ReSetContent_D[256])
{
	int i=0;	//操作结果：0==succeed
	HKEY hKey; 
	LONG lRet=0;
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,ReValueName,NULL,REG_DWORD,(const unsigned char *)ReSetContent_D,4)!=ERROR_SUCCESS)
		{
			// 			AfxMessageBox("错误：无法设置有关的注册表信息");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		// 		AfxMessageBox("错误：无法查询有关的注册表信息");
		i=1;
	}
	return i;
}

int DeleteKey(HKEY ReRootKey,TCHAR *ReSubKey,TCHAR *ReKeyName)
{
	int i=0;	//操作结果：0==succeed
	HKEY hKey; 
	LONG lRet=0;
	if((RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey))==ERROR_SUCCESS)
	{
		lRet = RegDeleteKey(hKey,ReKeyName);
		if(lRet!=ERROR_SUCCESS)
		{
			//AfxMessageBox("清除指定项失败！");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		//AfxMessageBox("错误：无法打开有关的hKEY");
		i=1;
	}
	return i;
}

int DeleteValue(HKEY ReRootKey,TCHAR *ReSubKey,TCHAR *ReValueName)
{
	int i=0;	//操作结果：0==succeed
	HKEY hKey; 
	LONG lRet=0;
	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegDeleteValue(hKey,ReValueName)!=ERROR_SUCCESS)
		{
			//AfxMessageBox("清除指定值失败！");
			i=1;
		}
		RegCloseKey(hKey);
	}
	else
	{
		//AfxMessageBox("错误：无法打开有关的hKEY");
		i=1;
	}
	return i;
}

bool RaisePrivileges()
{
	HANDLE TokenHandle;
	TOKEN_PRIVILEGES t_privileges = {0};

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &TokenHandle))
	{
		return FALSE;
	}

	if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &t_privileges.Privileges[0].Luid))
	{
		return TRUE;
	}

	t_privileges.PrivilegeCount = 1;
	t_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	if(!AdjustTokenPrivileges(TokenHandle, FALSE, &t_privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		CloseHandle(TokenHandle);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


void SetIECoreVersion()
{
	char* path = "SOFTWARE\\Microsoft\\Internet Explorer\\MAIN\\FeatureControl\\FEATURE_BROWSER_EMULATION";
	char* valueName = "星品秀发.exe";
	long version=11000;
	char err[1024];
	HKEY hKey;
	DWORD dwDisposition;
	long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0,REG_LEGAL_OPTION, &hKey);
	if( ret != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ret, NULL,err,sizeof(err),NULL);
		ret = RegCreateKeyEx(HKEY_LOCAL_MACHINE, path,0, NULL, REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL, &hKey, &dwDisposition);
		if( ret != ERROR_SUCCESS)
		{
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ret, NULL,err,sizeof(err),NULL);
			return;
		}
	}

	ret = RegSetValueEx( hKey, valueName, NULL, REG_DWORD, (unsigned char*)&version, sizeof(version));
	if( ret != ERROR_SUCCESS)
		return;
}

bool CreateShortCut( CString FilePath, CString lnk)
{
	if(PathFileExists(lnk))
	{
		return true;
	}
	HRESULT hr = CoInitialize(NULL); 
	if (SUCCEEDED(hr)) 
	{ 
		IShellLink *pisl;  
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkA, (void**)&pisl);  
		if (SUCCEEDED(hr))  
		{   
			IPersistFile* pIPF;  
			char path[MAX_PATH];  
			GetModuleFileName(GetModuleHandle(NULL), path, MAX_PATH);  
			pisl->SetPath(path);  
			char *pFind = NULL;
			pFind = strrchr(path, '\\');
			memset((void *)pFind, 0x00, strlen(pFind));
			DWORD reserved = 1;
			pisl->SetRelativePath(path,reserved);
			//pisl->SetIconLocation(path);
			//pisl->SetArguments(path);
			pisl->SetWorkingDirectory(path);
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);      
			if (SUCCEEDED(hr))  
			{    
			  
			   WCHAR wpath[MAX_PATH] = { 0 };    
			   //strcat(lnk, ".lnk");    
			   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lnk.GetBuffer(0), -1, wpath, MAX_PATH);  
			   pIPF->Save(wpath, FALSE);   
			   
			   pIPF->Release();  
			}  
			pisl->Release();  
		}  
		//CoUninitialize();
		return true;
	}
	return false;
} 

void PostMsgByCapname(CString capname,UINT msg,WPARAM param,LPARAM lparam)
{
	CWnd *pWnd=CWnd::FindWindow(NULL,capname);//目标进程的窗口的captain 
	if(pWnd)
	{
		pWnd->PostMessage(msg,param,lparam);
	}
}

HANDLE GetProcessHandle(int nID)//通过进程ID获取进程句柄
{
return OpenProcess(PROCESS_ALL_ACCESS, FALSE, nID);
}
HANDLE GetHandleByProcessName(char * name,DWORD ProcessID)
{
	ProcessID = -1;
	HANDLE pHandler = NULL;
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//找第一个进程
	PROCESSENTRY32  processListStr;

	bool f = Process32First(Snapshot, &processListStr);
	while(f)
	{
		CString exeName = processListStr.szExeFile;


		int index = exeName.Find(name);
		if (index != -1 && (strlen(name)+4)>= exeName.GetLength())
		{
			pHandler  =GetProcessHandle(processListStr.th32ProcessID);
			ProcessID = processListStr.th32ProcessID;
			break;
		}
		f = Process32Next(Snapshot, &processListStr); //继续查找
	}

	/*bool status = Thread32First(Snapshot, &threadinfo);
	while (status && pHandler)
	{
		if (threadinfo.th32OwnerProcessID == ProcessID)
		{
			ProcessID = threadinfo.th32ThreadID;
			break;
		}
		status = Thread32Next(Snapshot, &threadinfo);
	}*/



	CloseHandle(Snapshot);

	return pHandler;
}
