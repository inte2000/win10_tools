#pragma once

#include "std_inc.h"
#include "ConfigMgmt.h"

class CAppConfig final
{
    friend CAppConfig& GetAppConfig();

public:
    ~CAppConfig() {}
    bool IsInitialized() const { return m_Initialized; }
    BOOL Load();
    BOOL Save();

    TString GetLanguage() const;
    void SetLanguage(LPCTSTR lang);
    TString GetUIThemeName() const;
    void SetUIThemeName(LPCTSTR theme);
    int GetUITransparency() const { return m_cfg.GetIntegerAttr("Global", "UITransparency", 230); }
    void SetUITransparency(int transparency) { m_cfg.SetIntegerAttr("Global", "UITransparency", transparency); }
    BOOL GetAutoRun() const { return m_cfg.GetBoolAttr("Global", "AutoRun", FALSE); }
    void SetAutoRun(BOOL bAutoRun) { m_cfg.SetBoolAttr("Global", "AutoRun", bAutoRun); }
    int GetThemeMode() const { return m_cfg.GetIntegerAttr("DarkMode", "ThemeMode", 0); }
    void SetThemeMode(int mode) { m_cfg.SetIntegerAttr("DarkMode", "ThemeMode", mode); }
    BOOL GetWallpaperFollow() const { return m_cfg.GetBoolAttr("Desktop", "WallpaperFollow", TRUE); }
    void SetWallpaperFollow(BOOL bFollow) { m_cfg.SetBoolAttr("Desktop", "WallpaperFollow", bFollow); }
    int GetWallpaperMode() const { return m_cfg.GetIntegerAttr("Desktop", "WallpaperMode", 0); }
    void SetWallpaperMode(int mode) { m_cfg.SetIntegerAttr("Desktop", "WallpaperMode", mode); }
    int GetLightThemeBeginHour() const { return m_cfg.GetIntegerAttr("DarkMode", "LightThemeBeginHour", 0); }
    void SetLightThemeBeginHour(int hour) { m_cfg.SetIntegerAttr("DarkMode", "LightThemeBeginHour", hour); }
    int GetLightThemeBeginMinutes() const { return m_cfg.GetIntegerAttr("DarkMode", "LightThemeBeginMinutes", 0); }
    void SetLightThemeBeginMinutes(int Minutes) { m_cfg.SetIntegerAttr("DarkMode", "LightThemeBeginMinutes", Minutes); }
    int GetLightThemeEndHour() const { return m_cfg.GetIntegerAttr("DarkMode", "LightThemeEndHour", 0); }
    void SetLightThemeEndHour(int hour) { m_cfg.SetIntegerAttr("DarkMode", "LightThemeEndHour", hour); }
    int GetLightThemeEndMinutes() const { return m_cfg.GetIntegerAttr("DarkMode", "LightThemeEndMinutes", 0); }
    void SetLightThemeEndMinutes(int Minutes) { m_cfg.SetIntegerAttr("DarkMode", "LightThemeEndMinutes", Minutes); }
    TString GetLightThemeWallpaper() const;
    void SetLightThemeWallpaper(LPCTSTR wallpaper);
    TString GetDarkThemeWallpaper() const;
    void SetDarkThemeWallpaper(LPCTSTR wallpaper);
    TString GetLightThemeFile() const;
    void SetLightThemeFile(LPCTSTR filename);
    TString GetDarkThemeFile() const;
    void SetDarkThemeFile(LPCTSTR filename);
    int GetTaskbarAppearance() const { return m_cfg.GetIntegerAttr("Taskbar", "TaskbarAppearance", 0); }
    void SetTaskbarAppearance(int mode) { m_cfg.SetIntegerAttr("Taskbar", "TaskbarAppearance", mode); }
    int GetAccentOpacity() const { return m_cfg.GetIntegerAttr("Taskbar", "AccentOpacity", 0); }
    void SetAccentOpacity(int opacity) { m_cfg.SetIntegerAttr("Taskbar", "AccentOpacity", opacity); }
    int GetTransparency() const { return m_cfg.GetIntegerAttr("Taskbar", "Transparency", 0); }
    void SetTransparency(int trans) { m_cfg.SetIntegerAttr("Taskbar", "Transparency", trans); }
    BOOL GetMaximumWndMatch() const { return m_cfg.GetBoolAttr("Taskbar", "MaximumWndMatch", FALSE); }
    void SetMaximumWndMatch(BOOL bFollow) { m_cfg.SetBoolAttr("Taskbar", "MaximumWndMatch", bFollow); }


protected:
    CAppConfig(LPCTSTR position);

protected:
    bool m_Initialized;
    std::string m_position;
    XConfig m_cfg;
};

CAppConfig& GetAppConfig();
void SetAppConfigInfo(LPCTSTR position);
