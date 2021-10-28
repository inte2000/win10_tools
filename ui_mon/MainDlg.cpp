// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "UIMessageBox.h"
#include "StaticData.h"
#include "StringFunc.h"
#include "SouiHelper.h"
#include "ThemeMode.h"
#include "Global.h"
#include "AppConfig.h"
#include "SysApiWrapper.h"

#define TIMER_THEME_CHECK  1200

UINT WM_TASKBARCREATED = 0;
UINT MSG_SETTASKBAR_APPEARANCE = 0;

CMainDlg::CMainDlg() : CFrmWndImpl(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
    m_nThemeMode = gThemeModeAuto;
    m_beginH = 7;
    m_beginM = 00;
    m_endH = 17;
    m_endM = 30;
    m_bWallpaperFollow = TRUE;
    m_nWallpaperMode = 0; // 自定义
    m_nLastSettingChangedTicks = 0;
    m_bAutoRun = FALSE;
    m_nUITransparency = 230;
    m_nTaskbarMode = 0;
    m_nAccentOpacity = 240;
    m_nTaskbarTransparency = 0;
    m_bMaximumWndMatch = FALSE;
} 

CMainDlg::~CMainDlg()
{
}

void CMainDlg::OnShowWindow( BOOL bShow, UINT nStatus )
{
    UNREFERENCED_PARAMETER(nStatus);
	if(bShow)
	{
 		AnimateHostWindow(200,AW_CENTER);
	}
}

//appCfg.SetStringAttr("language", lang);
BOOL CMainDlg::OnInitDialog( HWND hWnd, LPARAM lParam )
{
    UNREFERENCED_PARAMETER(hWnd);
    UNREFERENCED_PARAMETER(lParam);

    //ITranslatorMgr *pTransMgr = SApplication::getSingleton().GetTranslator();
    InitDarkModePage();
    InitDesktopPage();
    InitTaskbarPage();
    InitOptionsPage();

    UpdateValueToControl();

    STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_main");
    if (pTabCtrl != nullptr)
        pTabCtrl->SetCurSel(0);
#ifdef _DEBUG
    CSimpleWnd::SetTimer(TIMER_THEME_CHECK, 10 * 1000, NULL);//10 秒钟检查一次
#else
    CSimpleWnd::SetTimer(TIMER_THEME_CHECK, 5 * 60 * 1000, NULL);//5 分钟检查一次
#endif

    AdjustPositionBySystemDpi();
   
    m_bLayoutInited = TRUE;
    return 0;
}

void CMainDlg::OnClose()
{
    CSimpleWnd::KillTimer(TIMER_THEME_CHECK);

    UninstallTaskbarHook();

    UpdateControlToValue();
    CAppConfig& cfg = GetAppConfig();
    cfg.SetThemeMode(m_nThemeMode);
    cfg.SetLightThemeBeginHour(m_beginH);
    cfg.SetLightThemeBeginMinutes(m_beginM);
    cfg.SetLightThemeEndHour(m_endH);
    cfg.SetLightThemeEndMinutes(m_endM);

    cfg.Save();

    AnimateHostWindow(200, AW_CENTER | AW_HIDE);
    DestroyWindow();
}

void CMainDlg::AdjustPositionBySystemDpi()
{
    int nCurScale = ScaleFromSystemDpi(GetSystemDeviceDpi());
    
    int nOriginalScale = 100; //our skin default design as 96dpi
    CRect rcWnd = GetWindowRect();
    int OriWid = rcWnd.Width() * 100 / nOriginalScale;
    int OriHei = rcWnd.Height() * 100 / nOriginalScale;

    int nNewWid = OriWid * nCurScale / 100;
    int nNewHei = OriHei * nCurScale / 100;

    //SDispatchMessage(UM_SETSCALE, nCurScale, 0);

    if (!CSimpleWnd::IsZoomed()) {
        SetWindowPos(NULL, 0, 0, nNewWid, nNewHei, SWP_NOZORDER);
    }
}

void CMainDlg::OnDpiChanged(WORD dpi, const RECT* desRect)
{
    int nScale = ScaleFromSystemDpi(dpi);
    SDispatchMessage(UM_SETSCALE, nScale, 0);

    SetWindowPos(NULL, desRect->left, desRect->top, desRect->right - desRect->left, 
                 desRect->bottom - desRect->top, SWP_NOZORDER | SWP_NOACTIVATE);
}

LRESULT CMainDlg::OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    RefreshTaskbars();
    return 0;
}

void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
    BOOL bCommandHandled = FALSE;
    if (uNotifyCode == 0)
    {
        if (nID == 101)
        {
            PostMessage(WM_CLOSE);
            bCommandHandled = TRUE;
        }
        else if (nID == 107)
        {
            OnRadLightClicked();
            UpdateThemeModeRadio(m_nThemeMode);
            bCommandHandled = TRUE;
        }
        else if (nID == 108)
        {
            OnRadDarkClicked();
            UpdateThemeModeRadio(m_nThemeMode);
            bCommandHandled = TRUE;
        }
        else if (nID == 103)  //options
        {
            STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_main");
            if (pTabCtrl != nullptr)
                pTabCtrl->SetCurSel(3);

            bCommandHandled = TRUE;
        }
        else if (nID == 102)  //about
        {
            STabCtrl* pTabCtrl = FindChildByName2<STabCtrl>(L"tab_main");
            if (pTabCtrl != nullptr)
                pTabCtrl->SetCurSel(4);

            bCommandHandled = TRUE;
        }
    }

    SetMsgHandled(bCommandHandled);
}

void CMainDlg::OnInitMenuPopup(SMenuEx* menuPopup, UINT nIndex)
{
    SMenuExItem* pSubItem = menuPopup->GetMenuItemByPos(3);
    SSliderBar* pSlider = pSubItem->FindChildByName2<SSliderBar>(L"menu_slider_transparent");
    if (pSlider != NULL)
    {
        if (m_nTaskbarMode == 1)
            pSlider->SetValue(m_nAccentOpacity);
        else if (m_nTaskbarMode == 2)
            pSlider->SetValue(m_nTaskbarTransparency);
        else
            ;
        pSlider->GetEventSet()->subscribeEvent(EVT_SLIDER_POS, Subscriber(&CMainDlg::OnMenuSliderPos, this));
    }
}

void CMainDlg::OnTimer(UINT_PTR idEvent)
{
    SetMsgHandled(FALSE);
    if (idEvent == TIMER_THEME_CHECK)
    {
        if (m_nThemeMode == gThemeModeAuto)
        {
            CheckAndSetTheme();
        }
    }
}

void CMainDlg::InitDarkModePage()
{
    CAppConfig& cfg = GetAppConfig();

    m_nThemeMode = cfg.GetThemeMode();
    m_beginH = cfg.GetLightThemeBeginHour();
    m_beginM = cfg.GetLightThemeBeginMinutes();
    m_endH = cfg.GetLightThemeEndHour();
    m_endM = cfg.GetLightThemeEndMinutes();

    if (m_nThemeMode != gThemeModeAuto) //如果不是 auto 模式，就根据实际情况匹配一下
    {
        m_nThemeMode = IsDarkModeEnabled() ? 1 : 0;
    }

    if (m_beginH > m_endH)
        m_beginH = ((m_endH - 1) > 0) ? m_endH - 1 : m_endH;
    if ((m_beginH == m_endH) && (m_beginM > m_endM))
        m_beginM = m_endM;
}

void CMainDlg::OnRadLightClicked()
{
    m_nThemeMode = gThemeModeLight;
    if (IsDarkModeEnabled())
    {
        SWaitCursor wait;
        SwitchToLightMode();
    }

    CAppConfig& cfg = GetAppConfig();
    cfg.SetThemeMode(m_nThemeMode);
}

void CMainDlg::OnRadDarkClicked()
{
    m_nThemeMode = gThemeModeDark;
    if (!IsDarkModeEnabled())
    {
        SWaitCursor wait;
        SwitchToDarkMode();
    }

    CAppConfig& cfg = GetAppConfig();
    cfg.SetThemeMode(m_nThemeMode);
}

void CMainDlg::OnRadAutoClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_nThemeMode = gThemeModeAuto;
    GetTimeFromControl(m_beginH, m_beginM, m_endH, m_endM);

    if ((m_beginH > m_endH) || ((m_beginH == m_endH) && (m_beginM > m_endM)))
    {
        SStringT msgCaption = TR(GETSTRING(_T("@string/main_wnd_title")), _T(""));
        SStringT msgText = TR(GETSTRING(_T("@string/err_time_range_invalid")), _T("dlg_main"));
        UIMessageBox(m_hWnd, msgText, msgCaption, MB_OK | MB_ICONERROR);

        if (m_beginH > m_endH)
            m_beginH = ((m_endH - 1) > 0) ? m_endH - 1 : m_endH;
        if ((m_beginH == m_endH) && (m_beginM > m_endM))
            m_beginM = m_endM;

        cfg.SetLightThemeBeginHour(m_beginH);
        cfg.SetLightThemeBeginMinutes(m_beginM);
        cfg.SetLightThemeEndHour(m_endH);
        cfg.SetLightThemeEndMinutes(m_endM);
    }

    cfg.SetThemeMode(m_nThemeMode);
}

void CMainDlg::OnApplyButtonClicked()
{
    CAppConfig& cfg = GetAppConfig();

    int beginH, beginM, endH, endM;
    GetTimeFromControl(beginH, beginM, endH, endM);

    if ((beginH > endH) || ((beginH == endH) && (beginM > endM)))
    {
        SStringT msgCaption = TR(GETSTRING(_T("@string/main_wnd_title")), _T(""));
        SStringT msgText = TR(GETSTRING(_T("@string/err_time_range_invalid")), _T("dlg_main"));
        UIMessageBox(m_hWnd, msgText, msgCaption, MB_OK | MB_ICONERROR);
        return;
    }
    m_beginH = beginH;
    m_beginM = beginM;
    m_endH = endH;
    m_endM = endM;
    CheckAndSetTheme();

    cfg.SetLightThemeBeginHour(m_beginH);
    cfg.SetLightThemeBeginMinutes(m_beginM);
    cfg.SetLightThemeEndHour(m_endH);
    cfg.SetLightThemeEndMinutes(m_endM);
}

void CMainDlg::InitDesktopPage()
{
    CAppConfig& cfg = GetAppConfig();
    m_bWallpaperFollow = cfg.GetWallpaperFollow();
    m_nWallpaperMode = cfg.GetWallpaperMode(); // 自定义
    m_strLightWp = cfg.GetLightThemeWallpaper().c_str();
    m_strDarkWp = cfg.GetDarkThemeWallpaper().c_str();

    if (m_strDarkWp.IsEmpty() || m_strLightWp.IsEmpty())
    {
        SStringT strCurWallpaper = WzGetDesktopWallpaper();
        if (IsDarkModeEnabled() && m_strDarkWp.IsEmpty())
            m_strDarkWp = strCurWallpaper;
        if (!IsDarkModeEnabled() && m_strLightWp.IsEmpty())
            m_strLightWp = strCurWallpaper;

        cfg.SetLightThemeWallpaper(m_strLightWp);
        cfg.SetDarkThemeWallpaper(m_strDarkWp);
    }
    
    InitThemeComboData();

    EnableCustomDesktopControls(m_nWallpaperMode == 0);
    EnableThemeDesktopControls(m_nWallpaperMode == 1);

    //WCHAR curThemeName[128] = { 0 };
    //GetCurrentThemeName(curThemeName, 128);  // it always get aero.msstyle
    //SStringT tname = S_CW2T(curThemeName);
    m_strLightThemeFile = cfg.GetLightThemeFile().c_str();
    m_strDarkThemeFile = cfg.GetDarkThemeFile().c_str();
    if (m_strDarkThemeFile.IsEmpty() || m_strLightThemeFile.IsEmpty())
    {
        SStringT curThemeFile;
        GetCurrentThemeFile(curThemeFile.GetBuffer(MAX_PATH), MAX_PATH);
        curThemeFile.ReleaseBuffer();

        if (IsDarkModeEnabled() && m_strDarkThemeFile.IsEmpty())
            m_strDarkThemeFile = curThemeFile;
        if (!IsDarkModeEnabled() && m_strLightThemeFile.IsEmpty())
            m_strLightThemeFile = curThemeFile;

        cfg.SetLightThemeFile(m_strLightThemeFile);
        cfg.SetDarkThemeFile(m_strDarkThemeFile);
    }

    SStringT lightbaseName = GetFileBaseName(m_strLightThemeFile);
    SStringT darkbaseName = GetFileBaseName(m_strDarkThemeFile);
    MatchComboString(this, L"cbx_light_theme", lightbaseName);
    MatchComboString(this, L"cbx_dark_theme", darkbaseName);
}

void CMainDlg::OnRadCustomDesktopClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_nWallpaperMode = 0;
    EnableCustomDesktopControls(m_nWallpaperMode == 0);
    EnableThemeDesktopControls(m_nWallpaperMode == 1);
    cfg.SetWallpaperMode(m_nWallpaperMode);
}

void CMainDlg::OnRadSysThemeClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_nWallpaperMode = 1;
    EnableCustomDesktopControls(m_nWallpaperMode == 0);
    EnableThemeDesktopControls(m_nWallpaperMode == 1);
    cfg.SetWallpaperMode(m_nWallpaperMode);
}

void CMainDlg::OnLightWpDeleteButtonClicked()
{
    CAppConfig& cfg = GetAppConfig();
    ResetImageWnd(this, L"light_wallpaper", NULL);
    cfg.SetLightThemeWallpaper(_T(""));
}

void CMainDlg::OnLightWpChooseButtonClicked()
{
    CAppConfig& cfg = GetAppConfig();
    TCHAR szCurDir[MAX_PATH] = { 0 };
    ::GetCurrentDirectory(MAX_PATH, szCurDir);
    TString filename = WzGetOpenFileName(m_hWnd, szCurDir, _T("All\0*.*\0"));
    if (SetImageWndImage(this, L"light_wallpaper", filename.c_str(), filename.c_str()))
    {
        m_strLightWp = filename.c_str();
        if (!m_bWallpaperFollow || !IsDarkModeEnabled())
            WzSetDesktopWallpaper(m_strLightWp);

        cfg.SetLightThemeWallpaper(m_strLightWp);
    }
}

void CMainDlg::OnDarkWpDeleteButtonClicked()
{
    CAppConfig& cfg = GetAppConfig();
    ResetImageWnd(this, L"dark_wallpaper", NULL);
    cfg.SetDarkThemeWallpaper(_T(""));
}

void CMainDlg::OnDarkWpChooseButtonClicked()
{
    CAppConfig& cfg = GetAppConfig();
    TCHAR szCurDir[MAX_PATH] = { 0 };
    ::GetCurrentDirectory(MAX_PATH, szCurDir);
    TString filename = WzGetOpenFileName(m_hWnd, szCurDir, _T("All\0*.*\0"));
    if (SetImageWndImage(this, L"dark_wallpaper", filename.c_str(), filename.c_str()))
    {
        m_strDarkWp = filename.c_str();
        if (m_bWallpaperFollow && IsDarkModeEnabled())
            WzSetDesktopWallpaper(m_strDarkWp);
        cfg.SetDarkThemeWallpaper(m_strDarkWp);
    }
}

void CMainDlg::OnComboLightThemeChange(EventArgs* e)
{
    CAppConfig& cfg = GetAppConfig();
    EventCBSelChange* ed = sobj_cast<EventCBSelChange>(e);
    if (ed->nCurSel >= 0)
    {
        SComboBox* pCombo = dynamic_cast<SComboBox*>(e->sender);
        SStringT theme = pCombo->GetLBText(ed->nCurSel, TRUE);
        int nMode = (int)pCombo->GetItemData(ed->nCurSel);
        SStringT fullName;
        if (nMode == 0)
        {
            fullName = GetWindowsFolder() + _T("\\Resources\\Themes\\");
        }
        else
        {
            fullName = GetAppLocalFolder() + _T("\\Microsoft\\Windows\\Themes\\");
        }
        fullName += (theme + _T(".theme"));
        if (m_strLightThemeFile.CompareNoCase(fullName) != 0)
        {
            m_strLightThemeFile = fullName;
            if (m_bWallpaperFollow && !IsDarkModeEnabled())
                SwitchSystemTheme(m_hWnd, fullName);

            cfg.SetLightThemeFile(m_strLightThemeFile);
        }
    }
}

void CMainDlg::OnComboDarkThemeChange(EventArgs* e)
{
    CAppConfig& cfg = GetAppConfig();
    EventCBSelChange* ed = sobj_cast<EventCBSelChange>(e);
    if (ed->nCurSel >= 0)
    {
        SComboBox* pCombo = dynamic_cast<SComboBox*>(e->sender);
        SStringT theme = pCombo->GetLBText(ed->nCurSel, TRUE);
        int nMode = (int)pCombo->GetItemData(ed->nCurSel);
        SStringT fullName;
        if (nMode == 0)
        {
            fullName = GetWindowsFolder() + _T("\\Resources\\Themes\\");
        }
        else
        {
            fullName = GetAppLocalFolder() + _T("\\Microsoft\\Windows\\Themes\\");
        }
        fullName += (theme + _T(".theme"));
        if (m_strDarkThemeFile.CompareNoCase(fullName) != 0)
        {
            m_strDarkThemeFile = fullName;
            if (m_bWallpaperFollow && IsDarkModeEnabled())
                SwitchSystemTheme(m_hWnd, fullName);

            cfg.SetDarkThemeFile(m_strDarkThemeFile);
        }
    }
}

void CMainDlg::OnComboLanguageChange(EventArgs* e)
{
    CAppConfig& appCfg = GetAppConfig();
    EventCBSelChange* ed = sobj_cast<EventCBSelChange>(e);
    if (ed->nCurSel >= 0)
    {
        SComboBox* pCombo = dynamic_cast<SComboBox *>(e->sender);
        SStringT curLanname = (LPCTSTR)pCombo->GetItemData(ed->nCurSel);

        SStringT tlangname = appCfg.GetLanguage().c_str();
        if (curLanname != tlangname)
        {
            ITranslatorMgr *pTransMgr = SApplication::getSingleton().GetTranslator();
            pugi::xml_document xmlLang;
            if (SApplication::getSingleton().LoadXmlDocment(xmlLang, curLanname, _T("LANG")))
            {
                CAutoRefPtr<ITranslator> lang;
                pTransMgr->CreateTranslator(&lang);
                lang->Load(&xmlLang.child(L"language"), 1);//1=LD_XML
                wchar_t szName[64];
                lang->GetName(szName);
                pTransMgr->SetLanguage(szName);
                pTransMgr->InstallTranslator(lang);

                SFontPool::getSingleton().UpdateFonts();//update fonts that defined by translator
                SDispatchMessage(UM_SETLANGUAGE, 0, 0);

                appCfg.SetLanguage(curLanname);
            }
            else
            {
                SStringT msgCaption = TR(GETSTRING(_T("@string/main_wnd_title")), _T(""));
                SStringT msgText = TR(GETSTRING(_T("@string/err_load_language")), _T(""));
                UIMessageBox(m_hWnd, msgText, msgCaption, MB_OK|MB_ICONERROR);
            }
        }
    }
}

void CMainDlg::OnTimesEditNotify(EventArgs* e)
{
    EventRENotify* pEvt = sobj_cast<EventRENotify>(e);
    if ((pEvt->iNotify == EN_CHANGE) && m_bLayoutInited)
    {
        SEdit* pEdit = sobj_cast<SEdit>(pEvt->sender);
        SASSERT(pEdit);
        SStringT value = pEdit->GetWindowText();
        if (value.GetLength() < 2)
            return;

        int beginH, beginM, endH, endM;
        GetTimeFromControl(beginH, beginM, endH, endM);
        if ((beginH > endH) || ((beginH == endH) && (beginM > endM)))
        {
            SStringT msgCaption = TR(GETSTRING(_T("@string/main_wnd_title")), _T(""));
            SStringT msgText = TR(GETSTRING(_T("@string/err_time_range_invalid")), _T("dlg_main"));
            UIMessageBox(m_hWnd, msgText, msgCaption, MB_OK | MB_ICONERROR);
        }
        else
        {
            m_beginH = beginH;
            m_beginM = beginM;
            m_endH = endH;
            m_endM = endM;
        }
    }
}

void CMainDlg::OnToggleFollowChanged(EventArgs* e)
{
    CAppConfig& cfg = GetAppConfig();
    EventTGSelChanged* pEvt = sobj_cast<EventTGSelChanged>(e);
    m_bWallpaperFollow = pEvt->bNew;
    cfg.SetWallpaperFollow(m_bWallpaperFollow);
}

void CMainDlg::OnMainTabChanged(EventArgs* e)
{
    EventTabSelChanged* pEvt = sobj_cast<EventTabSelChanged>(e);
    STabCtrl* pTabCtrl = dynamic_cast<STabCtrl*>(e->sender);
    if (pTabCtrl != nullptr)
    {
        STabPage* pPage = dynamic_cast<STabPage*>(pTabCtrl->GetPage(pEvt->uNewSel));
        if (pPage != nullptr)
        {
            SStringT pagetitle = pPage->GetTitle();
            SStringT wndtitle = TR(GETSTRING(_T("@string/main_wnd_title")), _T(""));
            SetWindowTitle(wndtitle + _T(" - ") + pagetitle);
        }
    }
}

void CMainDlg::InitLanguageComboData()
{
    CAppConfig& appCfg = GetAppConfig();
    SStringT tlangname = appCfg.GetLanguage().c_str();

    SComboBox* pCombo = FindChildByName2<SComboBox>(L"cbx_language");
    if (pCombo != nullptr)
    {
        int cursel = -1;
        for (int i = 0; i < GetLanguagesCount(); i++)
        {
            pCombo->InsertItem(i, g_appLanguages[i].langDisplayName, -1, (LPARAM)g_appLanguages[i].langname);
            if (tlangname == g_appLanguages[i].langname)
                cursel = i;
        }

        if (cursel >= 0)
        {
            pCombo->SetCurSel(cursel);
        }
    }
}

void CMainDlg::InitThemeComboData()
{
    SComboBox* pLightCombo = FindChildByName2<SComboBox>(L"cbx_light_theme");
    SComboBox* pDarkCombo = FindChildByName2<SComboBox>(L"cbx_dark_theme");
    if ((pLightCombo == nullptr) || (pDarkCombo == nullptr))
        return;

    std::vector<std::pair<std::string, int>> themes;
    EnumSystemInstalledThemes(themes);

    int index = 0;
    for (auto& x : themes)
    {
        SStringT name = S_CA2T(x.first.c_str());
        pLightCombo->InsertItem(index, name, -1, (LPARAM)x.second);
        pDarkCombo->InsertItem(index, name, -1, (LPARAM)x.second);
        index++;
    }
}

BOOL CMainDlg::UpdateControlToValue()
{
    SStringT timeStr = GetControlText(this, L"edit_beginh", TRUE);
    m_beginH = _ttoi(timeStr);
    timeStr = GetControlText(this, L"edit_beginm", TRUE);
    m_beginM = _ttoi(timeStr);
    timeStr = GetControlText(this, L"edit_endh", TRUE);
    m_endH = _ttoi(timeStr);
    timeStr = GetControlText(this, L"edit_endm", TRUE);
    m_endM = _ttoi(timeStr);

    //Desktop
    SToggleEx* pToggleFollow = FindChildByName2<SToggleEx>(L"toggle_follow");
    m_bWallpaperFollow = pToggleFollow->GetToggle();

    return TRUE;
}

BOOL CMainDlg::UpdateValueToControl()
{
    UpdateThemeModeRadio(m_nThemeMode);

    SStringT timeStr = SStringT().Format(_T("%02d"), m_beginH);
    SetControlText(this, L"edit_beginh", timeStr);
    timeStr.Format(_T("%02d"), m_beginM);
    SetControlText(this, L"edit_beginm", timeStr);
    timeStr.Format(_T("%02d"), m_endH);
    SetControlText(this, L"edit_endh", timeStr);
    timeStr.Format(_T("%02d"), m_endM);
    SetControlText(this, L"edit_endm", timeStr);

    //desktop
    SetRadioBoxValue(this, L"radio_custom_desktop", (m_nWallpaperMode == 0));
    SetRadioBoxValue(this, L"radio_sys_theme", (m_nWallpaperMode == 1));
    
    SToggleEx* pToggleFollow = FindChildByName2<SToggleEx>(L"toggle_follow");
    pToggleFollow->SetToggle(m_bWallpaperFollow, TRUE);

    SetImageWndImage(this, L"light_wallpaper", m_strLightWp, m_strLightWp);
    SetImageWndImage(this, L"dark_wallpaper", m_strDarkWp, m_strDarkWp);

    //option page
    SetCheckBoxValue(this, L"chk_auto_run", m_bAutoRun);
    SetSliderValue(this, L"slider_uitrans", m_nUITransparency);

    //taskbar page
    SetRadioBoxValue(this, L"radio_normal", (m_nTaskbarMode == 0));
    SetRadioBoxValue(this, L"radio_blur", (m_nTaskbarMode == 1));
    SetRadioBoxValue(this, L"radio_transparent", (m_nTaskbarMode == 2));
    UpdateSliderInfor(m_nTaskbarMode);
    SetCheckBoxValue(this, L"chk_maxwnd", m_bMaximumWndMatch);

    return TRUE;
}

void CMainDlg::UpdateThemeModeRadio(int nThemeMode)
{
    SetRadioBoxValue(this, L"radio_light", (nThemeMode == gThemeModeLight));
    SetRadioBoxValue(this, L"radio_dark", (nThemeMode == gThemeModeDark));
    SetRadioBoxValue(this, L"radio_auto", (nThemeMode == gThemeModeAuto));
}

void CMainDlg::GetTimeFromControl(int& beginH, int& beginM, int& endH, int& endM)
{
    SStringT timeStr = GetControlText(this, L"edit_beginh", TRUE);
    beginH = _ttoi(timeStr);
    timeStr = GetControlText(this, L"edit_beginm", TRUE);
    beginM = _ttoi(timeStr);
    timeStr = GetControlText(this, L"edit_endh", TRUE);
    endH = _ttoi(timeStr);
    timeStr = GetControlText(this, L"edit_endm", TRUE);
    endM = _ttoi(timeStr);
}

bool CMainDlg::IsCurrentInLightRange()
{
    struct tm curTime = { 0 };
    std::time_t time = std::time(nullptr);
    localtime_s(&curTime, &time);

    if ((m_beginH < curTime.tm_hour) && (curTime.tm_hour < m_endH))
        return true;
    if (m_beginH == curTime.tm_hour)
    {
        if ((m_beginH != m_endH) && (curTime.tm_min >= m_beginM))
            return true;
        if ((m_beginH == m_endH) && ((curTime.tm_min >= m_beginM) && (curTime.tm_min <= m_endM)))
            return true;
    }
    if (m_endH == curTime.tm_hour)
    {
        if ((m_beginH != m_endH) && (curTime.tm_min < m_endM))
            return true;
        if ((m_beginH == m_endH) && ((curTime.tm_min >= m_beginM) && (curTime.tm_min <= m_endM)))
            return true;
    }

    return false;
}

BOOL CMainDlg::SwitchToLightMode()
{
    SwitchSystemToLightMode();
    if (m_bWallpaperFollow && !m_strLightWp.IsEmpty())
    {
        WzSetDesktopWallpaper(m_strLightWp);
    }

    return TRUE;
}

BOOL CMainDlg::SwitchToDarkMode()
{
    SwitchSystemToDarkMode();
    if (m_bWallpaperFollow && !m_strDarkWp.IsEmpty())
    {
        WzSetDesktopWallpaper(m_strDarkWp);
    }

    return TRUE;
}

void CMainDlg::CheckAndSetTheme()
{
    if (IsCurrentInLightRange())
    {
        if (IsDarkModeEnabled())
        {
            SwitchToLightMode();
        }
    }
    else
    {
        if (!IsDarkModeEnabled())
        {
            SwitchToDarkMode();
        }
    }
}

void EnumThemeFiles(const std::string& themeFolder, std::vector<std::string>& themenames)
{
    stdfs::path base_folder(themeFolder);
    stdfs::directory_iterator ffitems(base_folder);
    for (const auto& di : ffitems)
    {
        if (!di.is_directory() && (di.path().extension().compare(".theme") == 0))
        {
            std::string themename = di.path().stem().string();
            themenames.push_back(themename);
        }
    }
}

void CMainDlg::EnumSystemInstalledThemes(std::vector<std::pair<std::string, int>>& themes)
{
    SStringT path = GetWindowsFolder();
    path += _T("\\Resources\\Themes");
    std::string systhemepath = S_CT2A(path);
    std::vector<std::string> systhemenames;
    EnumThemeFiles(systhemepath, systhemenames);
    for (auto& x : systhemenames)
        themes.push_back({x, 0});

    path = GetAppLocalFolder();
    path += _T("\\Microsoft\\Windows\\Themes");
    std::string userbasepath = S_CT2A(path);
    std::vector<std::string> userthemenames;
    EnumThemeFiles(userbasepath, userthemenames);
    for (auto& x : userthemenames)
        themes.push_back({ x, 1 });
}

void CMainDlg::EnableCustomDesktopControls(BOOL bEnable)
{
    SetWindowVisible(this, L"light_wallpaper", bEnable, TRUE);
    SetWindowVisible(this, L"dark_wallpaper", bEnable, TRUE);
    SetWindowEnabled(this, L"light_wp_delete", bEnable);
    SetWindowEnabled(this, L"light_wp_choose", bEnable);
    SetWindowEnabled(this, L"dark_wp_delete", bEnable);
    SetWindowEnabled(this, L"dark_wp_choose", bEnable);
}

void CMainDlg::EnableThemeDesktopControls(BOOL bEnable)
{
    SetWindowEnabled(this, L"cbx_light_theme", bEnable);
    SetWindowEnabled(this, L"cbx_dark_theme", bEnable);
}

void CMainDlg::SetTaskbarBluring(int nTaskbarMode)
{
    DWORD rgbaColor = 0x383838;
    if (!IsDarkModeEnabled())
        rgbaColor = 0xFFFFFF;

    if (nTaskbarMode == 0)
    {
        rgbaColor = 0;
        for (const auto& x : m_taskbars)
            ::SendMessage(x, MSG_SETTASKBAR_APPEARANCE, (WPARAM)nTaskbarMode, (LPARAM)rgbaColor);
    }
    else if (nTaskbarMode == 2)
    {
        rgbaColor = 0x0000FFFF;
        DWORD Transparency = m_nTaskbarTransparency;
        Transparency <<= 24;
        rgbaColor |= Transparency;
        for (const auto& x : m_taskbars)
            ::SendMessage(x, MSG_SETTASKBAR_APPEARANCE, (WPARAM)nTaskbarMode, (LPARAM)rgbaColor);
    }
    else
    {
        DWORD Opacity = m_nAccentOpacity;
        Opacity <<= 24;
        rgbaColor &= 0x00FFFF00;
        rgbaColor |= Opacity;
        //rgbaColor = 0x66000000;
        for (const auto& x : m_taskbars)
            ::SendMessage(x, MSG_SETTASKBAR_APPEARANCE, (WPARAM)nTaskbarMode, (LPARAM)rgbaColor);
    }
}

void CMainDlg::OnMaxWndCheckboxClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_bMaximumWndMatch = !m_bMaximumWndMatch;

    SetharedTaskbarOptions(m_bMaximumWndMatch);
    cfg.SetMaximumWndMatch(m_bMaximumWndMatch);
}

void CMainDlg::InitTaskbarPage()
{
    CAppConfig& cfg = GetAppConfig();

    m_nTaskbarMode = cfg.GetTaskbarAppearance();
    m_nAccentOpacity = cfg.GetAccentOpacity();
    m_nTaskbarTransparency = cfg.GetTransparency();
    m_bMaximumWndMatch = cfg.GetMaximumWndMatch();
    WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
    MSG_SETTASKBAR_APPEARANCE = ::RegisterWindowMessage(_T("UIMON_SETTASKBAR"));

    InstallTaskbarHook();
    RefreshTaskbars();

    if (m_nTaskbarMode == 0)
        InitSharedTaskbarMode(m_nTaskbarMode, 0);
    else if (m_nTaskbarMode == 1)
        InitSharedTaskbarMode(m_nTaskbarMode, m_nAccentOpacity);
    else
        InitSharedTaskbarMode(m_nTaskbarMode, m_nTaskbarTransparency);

    SetharedTaskbarOptions(m_bMaximumWndMatch);
}

void CMainDlg::OnRadTaskbarNotmalClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_nTaskbarMode = 0;
    UpdateSliderInfor(m_nTaskbarMode);
    SetTaskbarBluring(m_nTaskbarMode);

    cfg.SetTaskbarAppearance(m_nTaskbarMode);
}

void CMainDlg::OnRadTaskbarBlurClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_nTaskbarMode = 1;
    UpdateSliderInfor(m_nTaskbarMode);
    SetTaskbarBluring(m_nTaskbarMode);

    cfg.SetTaskbarAppearance(m_nTaskbarMode);
}

void CMainDlg::OnRadTaskbarTransparentClicked()
{
    CAppConfig& cfg = GetAppConfig();
    m_nTaskbarMode = 2;
    UpdateSliderInfor(m_nTaskbarMode);
    SetTaskbarBluring(m_nTaskbarMode);

    cfg.SetTaskbarAppearance(m_nTaskbarMode);
}

void CMainDlg::RefreshTaskbars()
{
    m_taskbars.clear();

    HWND hTaskbar = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    if(hTaskbar != NULL)
        m_taskbars.push_back(hTaskbar);
    HWND secondtaskbar = NULL;
    while (secondtaskbar = ::FindWindowEx(0, secondtaskbar, _T("Shell_SecondaryTrayWnd"), NULL))
    {
        m_taskbars.push_back(secondtaskbar);
    }
}

bool CMainDlg::OnMenuSliderPos(EventArgs* pEvt)
{
    EventSliderPos* pEvt2 = sobj_cast<EventSliderPos>(pEvt);
    SSliderBar* pMenuSlider = sobj_cast<SSliderBar>(pEvt->sender);
    SSliderBar* pTabSlider = FindChildByName2<SSliderBar>(L"slider_transparent");

    CAppConfig& cfg = GetAppConfig();
    if (m_nTaskbarMode == 1) //Blur
    {
        m_nAccentOpacity = pMenuSlider->GetValue();
        pTabSlider->SetValue(m_nAccentOpacity);
        cfg.SetAccentOpacity(m_nAccentOpacity);
        SetTaskbarBluring(m_nTaskbarMode);
    }
    else if (m_nTaskbarMode == 2)
    {
        m_nTaskbarTransparency = pMenuSlider->GetValue();
        pTabSlider->SetValue(m_nTaskbarTransparency);
        cfg.SetTransparency(m_nTaskbarTransparency);
        SetTaskbarBluring(m_nTaskbarMode);
    }

    return true;
}

bool CMainDlg::OnTaskbarPageSliderPos(EventArgs* pEvt)
{
    EventSliderPos* pEvt2 = sobj_cast<EventSliderPos>(pEvt);
    SSliderBar* pSlider = sobj_cast<SSliderBar>(pEvt->sender);

    CAppConfig& cfg = GetAppConfig();
    if (m_nTaskbarMode == 1) //Blur
    {
        m_nAccentOpacity = pSlider->GetValue();
        cfg.SetAccentOpacity(m_nAccentOpacity);
        SetTaskbarBluring(m_nTaskbarMode);
    }
    else if (m_nTaskbarMode == 2)
    {
        m_nTaskbarTransparency = pSlider->GetValue();
        cfg.SetTransparency(m_nTaskbarTransparency);
        SetTaskbarBluring(m_nTaskbarMode);
    }

    return true;
}

void CMainDlg::UpdateSliderInfor(int nTaskbarMode)
{
    if (nTaskbarMode == 0)
    {
        SetControlText(this, L"text_title", _T(""));
        SetSliderValue(this, L"slider_transparent", 0);
        SetWindowVisible(this, L"slider_transparent", FALSE);
    }
    else if (nTaskbarMode == 2)
    {
        SetControlText(this, L"text_title", _T("transparency: "));
        SetSliderValue(this, L"slider_transparent", m_nTaskbarTransparency);
        SetWindowVisible(this, L"slider_transparent", TRUE);
    }
    else
    {
        SetControlText(this, L"text_title", _T("Blur: "));
        SetSliderValue(this, L"slider_transparent", m_nAccentOpacity);
        SetWindowVisible(this, L"slider_transparent", TRUE);
    }
}

void CMainDlg::InitOptionsPage()
{
    CAppConfig& cfg = GetAppConfig();
    m_bAutoRun = cfg.GetAutoRun();
    m_nUITransparency = cfg.GetUITransparency();

    InitLanguageComboData();
    InitUIThemeComboData();
    SetWindowTransparency(m_nUITransparency);
}

void CMainDlg::OnAutoRunCheckboxClicked()
{
    m_bAutoRun = !m_bAutoRun;
    if (m_bAutoRun)
    {
        TCHAR exepath[MAX_PATH];
        ::GetModuleFileName(::GetModuleHandle(NULL), exepath, MAX_PATH);
        TString progPsth = exepath;

        AddRegistryAutorunEntry(_T("win10tools"), progPsth.c_str(), NULL);
    }
    else
    {
        RemoveRegistryAutorunEntry(_T("win10tools"));
    }
    CAppConfig& cfg = GetAppConfig();
    cfg.SetAutoRun(m_bAutoRun);
}

void CMainDlg::OnComboUIThemeChange(EventArgs* e)
{
    CAppConfig& appCfg = GetAppConfig();
    EventCBSelChange* ed = sobj_cast<EventCBSelChange>(e);
    if (ed->nCurSel >= 0)
    {
        SComboBox* pCombo = dynamic_cast<SComboBox*>(e->sender);
        SStringT curThemeName = (LPCTSTR)pCombo->GetLBText(ed->nCurSel);

        SStringT ttCfgName = appCfg.GetUIThemeName().c_str();
        if (curThemeName != ttCfgName)
        {
            appCfg.SetUIThemeName(curThemeName);
            SStringT msgCaption = TR(GETSTRING(_T("@string/main_wnd_title")), _T(""));
            SStringT msgText = TR(GETSTRING(_T("@string/restart_program")), _T(""));
            UIMessageBox(m_hWnd, msgText, msgCaption, MB_OK | MB_ICONINFORMATION);
        }
    }
}

bool IsUiSkinZipFile(const stdfs::directory_entry& di)
{
    std::string extname = di.path().extension().string();
    if (boost::iequals(extname, ".zip"))
        return true;

    return false;
}

void CMainDlg::InitUIThemeComboData()
{
    SComboBox* pCombo = FindChildByName2<SComboBox>(L"cbx_theme");
    if (pCombo == nullptr)
        return;

    CAppConfig& appCfg = GetAppConfig();

    SStringT strSkinPath = SApplication::getSingleton().GetAppDir() + _T("\\skin\\");
    std::string ansiSkinPath = AnsiStringFromTString(TString(strSkinPath));
    stdfs::directory_entry skin_entry(ansiSkinPath);

    int cursel = -1;
    int index = 0;
    stdfs::directory_iterator ffitems(skin_entry);
    for (const auto& di : ffitems)
    {
        if (!di.is_directory() && IsUiSkinZipFile(di))
        {
            std::string name = di.path().stem().string();
            TString tName = TStringFromAnsiString(name);
            pCombo->InsertItem(index, tName.c_str(), -1, (LPARAM)NULL);
            if (tName == appCfg.GetUIThemeName())
                cursel = index;

            index++;
        }
    }
    if (cursel >= 0)
    {
        pCombo->SetCurSel(cursel);
    }
}

void CMainDlg::OnColorClicked(EventArgs* e)
{
    SWindow* sender = (SWindow*)e->sender;
    SetWindowBkgndMode(TRUE, sender->GetStyle().m_crBg);
}

bool CMainDlg::OnOptionPageUITransSliderPos(EventArgs* pEvt)
{
    EventSliderPos* pEvt2 = sobj_cast<EventSliderPos>(pEvt);
    SSliderBar* pSlider = sobj_cast<SSliderBar>(pEvt->sender);

    m_nUITransparency = pSlider->GetValue();
    SetWindowTransparency(m_nUITransparency);
    CAppConfig& cfg = GetAppConfig();
    cfg.SetUITransparency(m_nUITransparency);

    return true;
}


/*

经过一番挖掘，我找到了这两种方法。两者都没有记录，可能会更改，恕不另行通知。

1

SetWindowTheme(hwnd, L"DarkMode_Explorer", NULL);

2

using TYPE_AllowDarkModeForWindow = bool (WINAPI *)(HWND a_HWND, bool a_Allow);
static const TYPE_AllowDarkModeForWindow AllowDarkModeForWindow = (TYPE_AllowDarkModeForWindow)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133));
AllowDarkModeForWindow(a_HWND, true);
SetWindowTheme(hwnd, L"Explorer", NULL);

*/