#pragma once

BOOL InitThemeMode();
BOOL IsDarkModeEnabled();
DWORD GetSystemBuildNumbers();

bool AllowDarkModeForWindow(HWND hWnd, bool allow);
bool IsHighContrast();
void AllowDarkModeForApp(bool allow);

BOOL SwitchPersonalizeTheme(BOOL bDark);
BOOL IsPersonalizeThemeDark();
BOOL EnablePersonalizeTransparency(BOOL bTransparent);

BOOL SetTaskbarRegistryTransparency(int transparency);

BOOL GetCurrentThemeName(WCHAR *nameBuf, int bufChars);
BOOL GetCurrentThemeFile(LPTSTR nameBuf, int bufChars);


//mode 0: (color + Opacity), mode 1: Blur, mode 2: transparency
BOOL SetWindowBlurring(HWND hWnd, UINT mode, DWORD rgbaColor);


