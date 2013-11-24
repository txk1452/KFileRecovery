#include "StdAfx.h"
#include "PubFun.h"


CPubFun::CPubFun(void)
{
}


CPubFun::~CPubFun(void)
{
}

CString CPubFun::GetLastErrorMessage()
{
	DWORD dwError = ::GetLastError();

	CString strErrorMsg;
	HLOCAL hlocal = NULL;   // Buffer that gets the error message string

	// Get the error code's textual description
	BOOL bOk = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		NULL, dwError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
		(PTSTR) &hlocal, 0, NULL);

	if (!bOk) 
	{
		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES); // Is it a network-related error?

		if (hDll != NULL) 
		{
			FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
				hDll, dwError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
				(PTSTR) &hlocal, 0, NULL);
			FreeLibrary(hDll);
		}
	}

	if (hlocal != NULL) 
	{
		strErrorMsg = (LPTSTR)LocalLock(hlocal);
		LocalFree(hlocal);
	} 

	return strErrorMsg;
}
