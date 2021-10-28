#include "stdafx.h"
#include "User32Func.h"

LPCTSTR CUser32Func::m_lpszUser32Dll = _T("user32.dll");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser32Func::CUser32Func()
{
	m_bLoad = FALSE;
	m_hModule = NULL;
	m_pfSetWindowCompositionAttribute = NULL;
	m_pfGetWindowCompositionAttribute = NULL;
}

CUser32Func::~CUser32Func()
{
	FreeFunctions();
}

BOOL CUser32Func::LoadFunctions()
{
	m_hModule = ::GetModuleHandle(m_lpszUser32Dll);
	if(m_hModule == NULL)
	{
		m_hModule = ::LoadLibrary(m_lpszUser32Dll);
		m_bLoad = TRUE;
	}

	if(m_hModule == NULL) //still NULL here? Maybe a 9x OS
	{
		m_bLoad = FALSE;
		return FALSE;
	}

	m_pfSetWindowCompositionAttribute = (FP_SETWINDOWCOMPOSITIONATTRIBUTE)::GetProcAddress(m_hModule,"SetWindowCompositionAttribute");
	m_pfGetWindowCompositionAttribute = (FP_GETWINDOWCOMPOSITIONATTRIBUTE)::GetProcAddress(m_hModule,"GetWindowCompositionAttribute");

#ifdef _UNICODE	
#else
#endif

	if((m_pfSetWindowCompositionAttribute == NULL) 
		|| (m_pfGetWindowCompositionAttribute == NULL))
	{
		FreeFunctions();
		return FALSE;
	}

	return TRUE;
}

BOOL CUser32Func::FreeFunctions()
{
	if(m_hModule != NULL)
	{
		if(m_bLoad)
			::FreeLibrary(m_hModule);

		m_hModule = NULL;
	}

	m_pfSetWindowCompositionAttribute = NULL;
	m_pfGetWindowCompositionAttribute = NULL;

	return TRUE;
}

BOOL CUser32Func::SetWindowCompositionAttribute(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA* data)
{
	if(m_pfSetWindowCompositionAttribute != NULL)
		return m_pfSetWindowCompositionAttribute(hWnd, data);
	else
		return FALSE;
}

BOOL CUser32Func::GetWindowCompositionAttribute(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA* data)
{
	if(m_pfGetWindowCompositionAttribute != NULL)
		return m_pfGetWindowCompositionAttribute(hWnd, data);
	else
		return FALSE;
}
