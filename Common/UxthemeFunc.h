#pragma once

#include <Uxtheme.h>

enum IMMERSIVE_HC_CACHE_MODE
{
	IHCM_USE_CACHED_VALUE,
	IHCM_REFRESH
};

// 1903 18362
enum PreferredAppMode
{
	Default,
	AllowDark,
	ForceDark,
	ForceLight,
	Max
};


//GetCurrentThemeName
typedef HRESULT(WINAPI* FP_GETCURRENTTHEMENAME)(LPWSTR pszThemeFileName, int cchMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);

// 1809 17763
//ShouldAppsUseDarkMode
typedef	bool (WINAPI* FP_SHOULDAPPSUSEDARKMODE)();   // ordinal 132
//AllowDarkModeForWindow
typedef	bool (WINAPI* FP_ALLOWDARKMODEFORWINDOW)(HWND hWnd, bool allow); // ordinal 133
//AllowDarkModeForApp
typedef	bool (WINAPI* FP_ALLOWDARKMODEFORAPP)(bool allow); // ordinal 135, in 1809
//FlushMenuThemes
typedef	void (WINAPI* FP_FLUSHMENUTHEMES)(); // ordinal 136
//RefreshImmersiveColorPolicyState
typedef	void (WINAPI* FP_REFRESHIMMERSIVECOLORPOLICYSTATE)(); // ordinal 104
//IsDarkModeAllowedForWindow
typedef	bool (WINAPI* FP_ISDARKMODEALLOWEDFORWINDOW)(HWND hWnd); // ordinal 137
//GetIsImmersiveColorUsingHighContrast
typedef	bool (WINAPI* FP_GETISIMMERSIVECOLORUSINGHIGHCONTRAST)(IMMERSIVE_HC_CACHE_MODE mode); // ordinal 106
//OpenNcThemeData
typedef	HTHEME (WINAPI* FP_OPENNCTHEMEDATA)(HWND hWnd, LPCWSTR pszClassList); // ordinal 49

// 1903 18362
//ShouldSystemUseDarkMode
typedef	bool (WINAPI* FP_SHOULDSYSTEMUSEDARKMODE)();   // ordinal 138
//SetPreferredAppMode
typedef	PreferredAppMode (WINAPI* FP_SETPREFERREDAPPMODE)(PreferredAppMode appMode); // ordinal 135, in 1903
//IsDarkModeAllowedForApp
typedef	bool (WINAPI* FP_ISDARKMODEALLOWEDFORAPP)();  // ordinal 139


typedef union
{
	FP_ALLOWDARKMODEFORAPP pfAllowDarkModeForApp;
	FP_SETPREFERREDAPPMODE pfSetPreferredAppMode;
	PVOID p;
}UxThemeOrdinal135;

class CUxthemeFunc
{
public:
	CUxthemeFunc();
	virtual ~CUxthemeFunc();
	
	BOOL LoadFunctions();
	BOOL FreeFunctions();

	HRESULT GetCurrentThemeName(LPWSTR pszThemeFileName, int cchMaxNameChars, 
		                       LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);

	bool ShouldAppsUseDarkMode();
	bool AllowDarkModeForWindow(HWND hWnd, bool allow);
	bool AllowDarkModeForApp(bool allow);
	PreferredAppMode SetPreferredAppMode(PreferredAppMode appMode);
	void FlushMenuThemes();
	void RefreshImmersiveColorPolicyState();
	bool IsDarkModeAllowedForWindow(HWND hWnd);
	bool GetIsImmersiveColorUsingHighContrast(IMMERSIVE_HC_CACHE_MODE mode);
	HTHEME OpenNcThemeData(HWND hWnd, LPCWSTR pszClassList);
	bool ShouldSystemUseDarkMode();
	bool IsDarkModeAllowedForApp();

public:
	static LPCTSTR m_lpszuxthemeDll;

protected:
	FP_GETCURRENTTHEMENAME m_pfGetCurrentThemeName;

	FP_SHOULDAPPSUSEDARKMODE m_pfShouldAppsUseDarkMode;
	FP_ALLOWDARKMODEFORWINDOW m_pfAllowDarkModeForWindow;
	UxThemeOrdinal135 m_Ordinal135;
	FP_FLUSHMENUTHEMES m_pfFlushMenuThemes;
	FP_REFRESHIMMERSIVECOLORPOLICYSTATE m_pfRefreshImmersiveColorPolicyState;
	FP_ISDARKMODEALLOWEDFORWINDOW m_pfIsDarkModeAllowedForWindow;
	FP_GETISIMMERSIVECOLORUSINGHIGHCONTRAST m_pfGetIsImmersiveColorUsingHighContrast;
	FP_OPENNCTHEMEDATA m_pfOpenNcThemeData;
	FP_SHOULDSYSTEMUSEDARKMODE m_pfShouldSystemUseDarkMode;
	FP_ISDARKMODEALLOWEDFORAPP m_pfIsDarkModeAllowedForApp;
	 

	BOOL m_bLoad;
	HMODULE m_hModule;
};

