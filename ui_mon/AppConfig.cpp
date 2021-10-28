#include "stdafx.h"
#include "StringFunc.h"
#include "AppConfig.h"

static SStringT s_AppPosition;

CAppConfig::CAppConfig(LPCTSTR position)
{
    m_Initialized = false;
    m_position = AnsiStringFromTString(TString(position));
}

BOOL CAppConfig::Load()
{    
    LoadConfig<XmlStorage>(m_position, m_cfg);
    m_Initialized = true;
    return m_Initialized ? TRUE : FALSE;
}

BOOL CAppConfig::Save()
{
    bool rtn = SaveConfig<XmlStorage>(m_position, m_cfg);

    return rtn ? TRUE : FALSE;
}

TString CAppConfig::GetLanguage() const
{
    std::string lang = m_cfg.GetStringAttr("Global", "language");
    TString mtlan = TStringFromChars(lang.c_str());

    return mtlan;
}

void CAppConfig::SetLanguage(LPCTSTR lang)
{
    std::string language = AnsiStringFromTString(TString(lang));
    m_cfg.SetStringAttr("Global", "language", language);
}

TString CAppConfig::GetUIThemeName() const
{
    std::string theme = m_cfg.GetStringAttr("Global", "theme");
    TString mtheme = TStringFromChars(theme.c_str());

    return mtheme;
}

void CAppConfig::SetUIThemeName(LPCTSTR theme)
{
    std::string stheme = AnsiStringFromTString(TString(theme));
    m_cfg.SetStringAttr("Global", "theme", stheme);
}

TString CAppConfig::GetLightThemeWallpaper() const
{
    std::string wp = m_cfg.GetStringAttr("Desktop", "LightThemeWallpaper");
    TString twp = TStringFromChars(wp.c_str());

    return std::move(twp);
}

void CAppConfig::SetLightThemeWallpaper(LPCTSTR wallpaper)
{
    std::string wp = AnsiStringFromTString(TString(wallpaper));
    m_cfg.SetStringAttr("Desktop", "LightThemeWallpaper", wp);
}

TString CAppConfig::GetDarkThemeWallpaper() const
{
    std::string wp = m_cfg.GetStringAttr("Desktop", "DarkThemeWallpaper");
    TString twp = TStringFromChars(wp.c_str());

    return std::move(twp);
}

void CAppConfig::SetDarkThemeWallpaper(LPCTSTR wallpaper)
{
    std::string wp = AnsiStringFromTString(TString(wallpaper));
    m_cfg.SetStringAttr("Desktop", "DarkThemeWallpaper", wp);
}

TString CAppConfig::GetLightThemeFile() const
{
    std::string tf = m_cfg.GetStringAttr("Desktop", "LightThemeFile");
    TString ttf = TStringFromChars(tf.c_str());

    return std::move(ttf);
}

void CAppConfig::SetLightThemeFile(LPCTSTR filename)
{
    std::string tf = AnsiStringFromTString(TString(filename));
    m_cfg.SetStringAttr("Desktop", "LightThemeFile", tf);
}

TString CAppConfig::GetDarkThemeFile() const
{
    std::string tf = m_cfg.GetStringAttr("Desktop", "DarkThemeFile");
    TString ttf = TStringFromChars(tf.c_str());

    return std::move(ttf);
}

void CAppConfig::SetDarkThemeFile(LPCTSTR filename)
{
    std::string tf = AnsiStringFromTString(TString(filename));
    m_cfg.SetStringAttr("Desktop", "DarkThemeFile", tf);
}



void SetAppConfigInfo(LPCTSTR position)
{
    s_AppPosition = position;
}

CAppConfig& GetAppConfig()
{
    static CAppConfig s_cfg(s_AppPosition);
    if (!s_cfg.IsInitialized())
    {
        s_cfg.Load();
    }
    
    return s_cfg;
}
