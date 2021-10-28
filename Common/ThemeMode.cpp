#include "stdafx.h"
#include "NtdllFunc.h"
#include "UxThemeFunc.h"
#include "User32Func.h"
#include "WzRegKey.h"
#include "ThemeMode.h"

static DWORD s_BuildNumber = 0;
static DWORD s_MajorVersion = 0;
static DWORD s_MinorVersion = 0;
BOOL s_darkModeSupported = FALSE;
BOOL s_darkModeEnabled = FALSE;
CNtdllFunc g_ntdll;
CUxthemeFunc g_themeDll;
CUser32Func g_user32Dll;


BOOL InitThemeMode()
{
    if (!g_ntdll.LoadFunctions())
        return FALSE;

    if (!g_user32Dll.LoadFunctions())
        return FALSE;

    g_ntdll.RtlGetNtVersionNumbers(&s_MajorVersion, &s_MinorVersion, &s_BuildNumber);
    s_BuildNumber &= 0x0FFFFFFF;

    if (!g_themeDll.LoadFunctions())
        return FALSE;

    s_darkModeSupported = TRUE;
    s_darkModeEnabled = g_themeDll.ShouldAppsUseDarkMode() && !IsHighContrast();

    return TRUE;
}

BOOL IsDarkModeEnabled()
{
    return s_darkModeEnabled;
}

DWORD GetSystemBuildNumbers()
{
    return s_BuildNumber;
}

bool AllowDarkModeForWindow(HWND hWnd, bool allow)
{
    if (s_darkModeSupported)
        return g_themeDll.AllowDarkModeForWindow(hWnd, allow);
    return false;
}

bool IsHighContrast()
{
    HIGHCONTRASTW highContrast = { sizeof(highContrast) };
    if (SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof(highContrast), &highContrast, FALSE))
        return highContrast.dwFlags & HCF_HIGHCONTRASTON;
    
    return false;
}

void AllowDarkModeForApp(bool allow)
{
    if (s_BuildNumber < 18362)
        g_themeDll.AllowDarkModeForApp(allow);
    else
        g_themeDll.SetPreferredAppMode(allow ? AllowDark : Default);
}

LPCTSTR lpszPersonalizeKeyName = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize");
BOOL SwitchPersonalizeTheme(BOOL bDark)
{
    CWzRegKey regKey(HKEY_CURRENT_USER, lpszPersonalizeKeyName, KEY_WRITE|KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    DWORD dwValue = bDark ? 0 : 1;
    if (regKey.SetDword(_T("AppsUseLightTheme"), dwValue) != ERROR_SUCCESS)
        return FALSE;
    if (regKey.SetDword(_T("SystemUsesLightTheme"), dwValue) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

BOOL IsPersonalizeThemeDark()
{
    CWzRegKey regKey(HKEY_CURRENT_USER, lpszPersonalizeKeyName, KEY_WRITE | KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    DWORD appTheme, sysTheme;
    regKey.GetDword(_T("AppsUseLightTheme"), appTheme, 21);
    regKey.GetDword(_T("SystemUsesLightTheme"), sysTheme, 21);

    if ((appTheme == 0) || (sysTheme == 0))
        return TRUE;

    return FALSE;
}

BOOL EnablePersonalizeTransparency(BOOL bTransparent)
{
    CWzRegKey regKey(HKEY_CURRENT_USER, lpszPersonalizeKeyName, KEY_WRITE | KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    DWORD dwValue = bTransparent ? 1 : 0;
    if (regKey.SetDword(_T("EnableTransparency"), dwValue) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

LPCTSTR lpszExplorAdv = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced");
BOOL SetTaskbarRegistryTransparency(int transparency)
{
    CWzRegKey regKey(HKEY_LOCAL_MACHINE, lpszExplorAdv, KEY_WRITE | KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    //0 为默认不启用（可直接删除），1 为启用亚克力透明
    if (regKey.SetDword(_T("UseOLEDTaskbarTransparency"), 0) != ERROR_SUCCESS) 
        return FALSE;

    regKey.CloseKey();

    if (regKey.OpenKey(HKEY_CURRENT_USER, lpszExplorAdv, KEY_WRITE | KEY_READ) != ERROR_SUCCESS)
        return FALSE;

    DWORD dwValue = transparency;
    //0为完全透明，10为完全不透明
    if (regKey.SetDword(_T("TaskbarAcrylicOpacity"), dwValue) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

BOOL GetCurrentThemeName(WCHAR *nameBuf, int bufChars)
{
    HRESULT hr = g_themeDll.GetCurrentThemeName(nameBuf, bufChars, NULL, 0, NULL, 0);

    return (hr == S_OK);
}

LPCTSTR lpszCurThemeKeyName = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes");
BOOL GetCurrentThemeFile(LPTSTR nameBuf, int bufChars)
{
    CWzRegKey regKey(HKEY_CURRENT_USER, lpszCurThemeKeyName, KEY_WRITE | KEY_READ);
    if (!regKey.IsValid())
        return FALSE;

    if (regKey.GetString(_T("CurrentTheme"), nameBuf, bufChars, NULL) != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

/*
ACCENT_ENABLE_GRADIENT results in a window that is completely gray, regardless of what is behind it. There is no transparency or glass effect, but the window colour being drawn is being drawn by the DWM, not by the app.
*/
BOOL SetWindowBlurring(HWND hWnd, UINT mode, DWORD rgbaColor)
{
    ACCENT_POLICY_ATTR attrPolicy;
    if (mode == 0)
    {
        attrPolicy = { ACCENT_ENABLE_GRADIENT, 0, rgbaColor, 0 };
    }
    else if (mode == 1)
    {
        //attrPolicy.accentState = ACCENT_ENABLE_BLURBEHIND; //ACCENT_ENABLE_ACRYLICBLURBEHIND;//
        attrPolicy = { ACCENT_ENABLE_BLURBEHIND, 2, rgbaColor, 0};
    }
    else
    {
        attrPolicy = { ACCENT_ENABLE_TRANSPARENTGRADIENT, 2, rgbaColor, 0 };
    }

    WINDOWCOMPOSITIONATTRIBDATA attrData = { WCA_ACCENT_POLICY, &attrPolicy, sizeof(ACCENT_POLICY_ATTR)};
    return g_user32Dll.SetWindowCompositionAttribute(hWnd, &attrData);
}
