#include "globalInfo.h"
#include <vector>

#ifdef WIN32
char g_rootpath[512] = "./";
#else 
#ifdef ANDROID
std::string  g_rootpath = "./sdcard/ShowO/data/modle/CLM/";
#else
std::string g_rootpath="./"
#endif
#endif

bool  ISFileExist(char * szFileName)
{
	FILE * pfile = fopen(szFileName,"r");
	if(pfile != NULL)
	{
		fclose(pfile);
		return true;
	}
	return false;
}


