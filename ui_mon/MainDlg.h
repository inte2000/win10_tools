// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "UIFrmWndImp.h"

const int gThemeModeLight = 0;
const int gThemeModeDark = 1;
const int gThemeModeAuto = 2;

extern UINT WM_TASKBARCREATED;

class CMainDlg : public CFrmWndImpl, public TAutoEventMapReg<CMainDlg>//通知中心自动注册
{
public:
	CMainDlg();
	~CMainDlg();

protected:
	void OnShowWindow(BOOL bShow, UINT nStatus);
    BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnClose();

	void OnDpiChanged(WORD dpi, const RECT* desRect);
	LRESULT OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//DUI菜单响应函数
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnInitMenuPopup(SMenuEx* menuPopup, UINT nIndex);
	void OnTimer(UINT_PTR idEvent);

	//Dark mode page
	void InitDarkModePage();
	void OnRadLightClicked();
	void OnRadDarkClicked();
	void OnRadAutoClicked();
	void OnApplyButtonClicked();

	//Desktop page
	void InitDesktopPage();
	void InitThemeComboData();
	void OnRadCustomDesktopClicked();
	void OnRadSysThemeClicked();
	void OnLightWpDeleteButtonClicked();
	void OnLightWpChooseButtonClicked();
	void OnDarkWpDeleteButtonClicked();
	void OnDarkWpChooseButtonClicked();
	void OnComboLightThemeChange(EventArgs* e);
	void OnComboDarkThemeChange(EventArgs* e);

	//taskbar page
	void InitTaskbarPage();
	void OnRadTaskbarNotmalClicked();
	void OnRadTaskbarBlurClicked();
	void OnRadTaskbarTransparentClicked();
	void RefreshTaskbars();
	bool OnMenuSliderPos(EventArgs* pEvt);
	bool OnTaskbarPageSliderPos(EventArgs* pEvt);
	void SetTaskbarBluring(int nTaskbarMode);
	void OnMaxWndCheckboxClicked();

	//Options page
	void InitOptionsPage();
	void OnComboLanguageChange(EventArgs* e);
	void InitLanguageComboData();
	void OnAutoRunCheckboxClicked();
	void OnComboUIThemeChange(EventArgs* e);
	void InitUIThemeComboData();
	void OnColorClicked(EventArgs* e);
	bool OnOptionPageUITransSliderPos(EventArgs* pEvt);

	void OnTimesEditNotify(EventArgs* e);
	void OnToggleFollowChanged(EventArgs* e);
	void OnMainTabChanged(EventArgs* e);

protected:
	
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"radio_light", OnRadLightClicked)
		EVENT_NAME_COMMAND(L"radio_dark", OnRadDarkClicked)
		EVENT_NAME_COMMAND(L"radio_auto", OnRadAutoClicked)
		EVENT_NAME_COMMAND(L"btn_apply", OnApplyButtonClicked)
		EVENT_NAME_HANDLER(L"cbx_language", EventCBSelChange::EventID, OnComboLanguageChange)
		EVENT_NAME_HANDLER(L"cbx_theme", EventCBSelChange::EventID, OnComboUIThemeChange)
		EVENT_NAME_HANDLER(L"cbx_light_theme", EventCBSelChange::EventID, OnComboLightThemeChange)
		EVENT_NAME_HANDLER(L"cbx_dark_theme", EventCBSelChange::EventID, OnComboDarkThemeChange)
		EVENT_NAME_HANDLER(L"edit_beginh", EVT_RE_NOTIFY, OnTimesEditNotify)
		EVENT_NAME_HANDLER(L"edit_beginm", EVT_RE_NOTIFY, OnTimesEditNotify)
		EVENT_NAME_HANDLER(L"edit_endh", EVT_RE_NOTIFY, OnTimesEditNotify)
		EVENT_NAME_HANDLER(L"edit_endm", EVT_RE_NOTIFY, OnTimesEditNotify)
		EVENT_NAME_HANDLER(L"toggle_follow", EventTGSelChanged::EventID, OnToggleFollowChanged)
		EVENT_NAME_HANDLER(L"tab_main", EventTabSelChanged::EventID, OnMainTabChanged)
		EVENT_NAME_COMMAND(L"radio_custom_desktop", OnRadCustomDesktopClicked)
		EVENT_NAME_COMMAND(L"radio_sys_theme", OnRadSysThemeClicked)
		EVENT_NAME_COMMAND(L"light_wp_delete", OnLightWpDeleteButtonClicked)
		EVENT_NAME_COMMAND(L"light_wp_choose", OnLightWpChooseButtonClicked)
		EVENT_NAME_COMMAND(L"dark_wp_delete", OnDarkWpDeleteButtonClicked)
		EVENT_NAME_COMMAND(L"dark_wp_choose", OnDarkWpChooseButtonClicked)
		EVENT_NAME_COMMAND(L"chk_auto_run", OnAutoRunCheckboxClicked)
		EVENT_NAME_COMMAND(L"radio_normal", OnRadTaskbarNotmalClicked)
		EVENT_NAME_COMMAND(L"radio_blur", OnRadTaskbarBlurClicked)
		EVENT_NAME_COMMAND(L"radio_transparent", OnRadTaskbarTransparentClicked)
		EVENT_NAME_HANDLER(L"slider_transparent", EventSliderPos::EventID, OnTaskbarPageSliderPos)
		EVENT_NAME_COMMAND(L"chk_maxwnd", OnMaxWndCheckboxClicked)
		EVENT_ID_RANGE_HANDLER(30, 46, EVT_CMD, OnColorClicked)
		EVENT_NAME_HANDLER(L"slider_uitrans", EventSliderPos::EventID, OnOptionPageUITransSliderPos)
		//<--通知中心事件
		//-->
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_COMMAND(OnCommand)
		MSG_WM_INITMENUPOPUP_EX(OnInitMenuPopup)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_SHOWWINDOW(OnShowWindow)
	    MSG_WM_DPICHANGED(OnDpiChanged)
		MESSAGE_HANDLER_EX(WM_TASKBARCREATED, OnTaskbarCreated);
		CHAIN_MSG_MAP(CFrmWndImpl)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	void AdjustPositionBySystemDpi();
	BOOL UpdateControlToValue();
	BOOL UpdateValueToControl();
	void UpdateThemeModeRadio(int nThemeMode);
	void GetTimeFromControl(int &beginH, int &beginM, int &endH, int& endM);
	bool IsCurrentInLightRange();
	BOOL SwitchToLightMode();
	BOOL SwitchToDarkMode();
	void CheckAndSetTheme();
	void EnumSystemInstalledThemes(std::vector<std::pair<std::string, int>>& themes);
	void EnableCustomDesktopControls(BOOL bEnable);
	void EnableThemeDesktopControls(BOOL bEnable);
	void UpdateSliderInfor(int nTaskbarMode);
private:
	BOOL m_bLayoutInited;
	std::vector<HWND> m_taskbars;
	int m_nThemeMode;
	int m_beginH, m_beginM, m_endH, m_endM;
	BOOL m_bWallpaperFollow;
	int m_nWallpaperMode;
	SStringT m_strLightWp, m_strDarkWp;
	DWORD m_nLastSettingChangedTicks;
	SStringT m_strLightThemeFile, m_strDarkThemeFile;
	BOOL m_bAutoRun;
	int m_nUITransparency;
	int m_nTaskbarMode;
	int m_nAccentOpacity;
	int m_nTaskbarTransparency;
	BOOL m_bMaximumWndMatch;
};
