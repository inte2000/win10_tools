#include "stdafx.h"
#include "TaskbarFunc.h"

LPCTSTR CTaskbarFunc::m_lpszTaskbarDll = _T("taskbar.dll");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskbarFunc::CTaskbarFunc()
{
	m_bLoad = FALSE;
	m_hModule = NULL;

	m_pfInstallHook = NULL;
	m_pfUninstallHook = NULL;
	m_pfSetTaskbarMode = NULL;
	m_pfSetOptions = NULL;
}

CTaskbarFunc::~CTaskbarFunc()
{
	FreeFunctions();
}

BOOL CTaskbarFunc::LoadFunctions()
{
	m_hModule = ::LoadLibrary(m_lpszTaskbarDll);
	if(m_hModule == NULL) //still NULL here? Maybe a 9x OS
	{
		m_bLoad = FALSE;
		return FALSE;
	}
	m_bLoad = TRUE;
	m_pfInstallHook = (FP_INSTALLHOOK)GetProcAddress(m_hModule, "InstallHook");
	m_pfUninstallHook = (FP_UNINSTALLHOOK)GetProcAddress(m_hModule, "UninstallHook");
	m_pfSetTaskbarMode = (FP_SETTASKBARMODE)GetProcAddress(m_hModule, "SetTaskbarMode");
	m_pfSetOptions = (FP_SETOPTIONS)GetProcAddress(m_hModule, "SetOptions");
#ifdef _UNICODE	
#else
#endif
	if((m_pfInstallHook == NULL)
		|| (m_pfUninstallHook == NULL)
		|| (m_pfSetTaskbarMode == NULL)
		|| (m_pfSetOptions == NULL))
	{
		FreeFunctions();
		return FALSE;
	}

	return TRUE;
}

BOOL CTaskbarFunc::FreeFunctions()
{
	if(m_hModule != NULL)
	{
		if(m_bLoad)
			::FreeLibrary(m_hModule);

		m_hModule = NULL;
	}

	m_pfInstallHook = NULL;
	m_pfUninstallHook = NULL;
	m_pfSetTaskbarMode = NULL;
	m_pfSetOptions = NULL;

	return TRUE;
}

BOOL CTaskbarFunc::InstallHook()
{
	if (m_pfInstallHook != NULL)
		return m_pfInstallHook();
	return
		FALSE;
}

void CTaskbarFunc::UninstallHook()
{
	if (m_pfUninstallHook != NULL)
		m_pfUninstallHook();
}

void CTaskbarFunc::SetTaskbarMode(int mode, int transopca)
{
	if (m_pfSetTaskbarMode != NULL)
		m_pfSetTaskbarMode(mode, transopca);
}

void CTaskbarFunc::SetOptions(BOOL bMaximumWndMatch)
{
	if (m_pfSetOptions != NULL)
		m_pfSetOptions(bMaximumWndMatch);
}

