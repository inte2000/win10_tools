#pragma once

typedef	void	(WINAPI * FP_RTLGETNTVERSIONNUMBERS)(LPDWORD major, LPDWORD minor, LPDWORD buildNumber);


class CNtdllFunc  
{
public:
	CNtdllFunc();
	virtual ~CNtdllFunc();
	
	BOOL LoadFunctions();
	BOOL FreeFunctions();
	void RtlGetNtVersionNumbers(LPDWORD major, LPDWORD minor, LPDWORD buildNumber);

public:
	static LPCTSTR m_lpszNtDll;
protected:
	FP_RTLGETNTVERSIONNUMBERS m_pfRtlGetNtVersionNumbers;

	BOOL m_bLoad;
	HMODULE m_hModule;
};
