#include "stdafx.h"
#include "NtdllFunc.h"

LPCTSTR CNtdllFunc::m_lpszNtDll = _T("ntdll.dll");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNtdllFunc::CNtdllFunc()
{
	m_bLoad = FALSE;
	m_hModule = NULL;
	m_pfRtlGetNtVersionNumbers = NULL;
}

CNtdllFunc::~CNtdllFunc()
{
	FreeFunctions();
}

BOOL CNtdllFunc::LoadFunctions()
{
	m_hModule = ::GetModuleHandle(m_lpszNtDll);
	if(m_hModule == NULL)
	{
		m_hModule = ::LoadLibrary(m_lpszNtDll);
		m_bLoad = TRUE;
	}

	if(m_hModule == NULL) //still NULL here? Maybe a 9x OS
	{
		m_bLoad = FALSE;
		return FALSE;
	}
	m_pfRtlGetNtVersionNumbers = (FP_RTLGETNTVERSIONNUMBERS)::GetProcAddress(m_hModule,"RtlGetNtVersionNumbers");

#ifdef _UNICODE	
#else
#endif

	if(m_pfRtlGetNtVersionNumbers == NULL)
	{
		FreeFunctions();
		return FALSE;
	}

	return TRUE;
}

BOOL CNtdllFunc::FreeFunctions()
{
	if(m_hModule != NULL)
	{
		if(m_bLoad)
			::FreeLibrary(m_hModule);

		m_hModule = NULL;
	}

	m_pfRtlGetNtVersionNumbers = NULL;

	return TRUE;
}

void CNtdllFunc::RtlGetNtVersionNumbers(LPDWORD major, LPDWORD minor, LPDWORD buildNumber)
{
	if (m_pfRtlGetNtVersionNumbers != NULL)
		m_pfRtlGetNtVersionNumbers(major, minor, buildNumber);
}
