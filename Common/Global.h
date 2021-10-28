#pragma once

BOOL InitGlobalData();
BOOL IsSystemMatchRequest();

BOOL SwitchSystemToLightMode();
BOOL SwitchSystemToDarkMode();

BOOL SwitchSystemTheme(HWND hWnd, LPCTSTR strThemeFile);

BOOL AddRegistryAutorunEntry(LPCTSTR entryname, LPCTSTR exepath, LPCTSTR param);
BOOL RemoveRegistryAutorunEntry(LPCTSTR entryname);

BOOL InstallTaskbarHook();
void InitSharedTaskbarMode(int nTaskbarMode, int nTransopca);
void SetharedTaskbarOptions(BOOL bMaximumWndMatch);
void UninstallTaskbarHook();

void TestEnumImmersiveColor();

