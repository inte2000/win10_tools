#pragma once


//InstallHook
typedef	BOOL (WINAPI* FP_INSTALLHOOK)(); 
typedef	void (WINAPI* FP_UNINSTALLHOOK)();
typedef	void (WINAPI* FP_SETTASKBARMODE)(int mode, int transopca);
typedef	void (WINAPI* FP_SETOPTIONS)(BOOL bMaximumWndMatch);

class CTaskbarFunc
{
public:
	CTaskbarFunc();
	virtual ~CTaskbarFunc();
	
	BOOL LoadFunctions();
	BOOL FreeFunctions();

	BOOL InstallHook();
	void UninstallHook();
	void SetTaskbarMode(int mode, int transopca);
	void SetOptions(BOOL bMaximumWndMatch);

public:
	static LPCTSTR m_lpszTaskbarDll;

protected:
	FP_INSTALLHOOK m_pfInstallHook;
	FP_UNINSTALLHOOK m_pfUninstallHook;
	FP_SETTASKBARMODE m_pfSetTaskbarMode;
	FP_SETOPTIONS m_pfSetOptions;

	BOOL m_bLoad;
	HMODULE m_hModule;
};

