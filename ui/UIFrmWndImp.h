#ifndef UI_FRAME_WINDOW_HPP
#define UI_FRAME_WINDOW_HPP

const UINT SYSMENU_RESTORE_ID = 10001;
const UINT SYSMENU_MOVE_ID = 10002;
const UINT SYSMENU_SIZE_ID = 10003;
const UINT SYSMENU_MINIMIZE_ID = 10004;
const UINT SYSMENU_MAXIMIZE_ID = 10005;
const UINT SYSMENU_CLOSE_ID = 10006;

class CFrmWndImpl: public SHostWnd
{
    SOUI_CLASS_NAME(CFrmWndImpl, L"FrmWndImpl")
public:
    CFrmWndImpl(LPCTSTR pszResName = NULL) : SHostWnd(pszResName)
    {}
	virtual ~CFrmWndImpl() {};

    void SetWindowTitle(LPCTSTR lpstrTitle);
    void SetWindowBkgndMode(BOOL bUsingSolidColor, COLORREF color);
    void SetWindowTransparency(unsigned int alpha);
protected:
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnSysBtnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnSysBtnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnSysBtnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnSysBtnRestore)
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CFrmWndImpl)
	    MSG_WM_COMMAND(OnCommand)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_NCACTIVATE(OnNcActivate)
		CHAIN_MSG_MAP(SHostWnd)
		//REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

    void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
    void OnSysCommand(UINT nID, CPoint lParam);
    BOOL OnNcActivate(BOOL bActive);

    void OnSysMenuCommand(UINT nID);

    void OnSysBtnClose()  { PostMessage(WM_CLOSE); }
    void OnSysBtnMaximize()  { PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);  }
    void OnSysBtnMinimize()  { PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);  }
    void OnSysBtnRestore()   { PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);   }
    void OnSysBtnMove()  { PostMessage(WM_SYSCOMMAND, SC_MOVE, 0); }
    void OnSysBtnSize()  { PostMessage(WM_SYSCOMMAND, SC_SIZE, 0); }
};

#endif // UI_FRAME_WINDOW_HPP
