#pragma once

#include <winuser.h>


class CMessageBox : public SHostDialog
{
public:
    CMessageBox();
    CMessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
	virtual ~CMessageBox() {};

    void SetInformation(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

protected:	
    BOOL OnInitDialog(HWND hWnd, LPARAM lParam);
    void OnClose();
    
    void OnButtonClick(int uID)
    {
        EndDialog(uID);
    }

    EVENT_MAP_BEGIN()
        EVENT_ID_COMMAND_RANGE(IDOK, IDNO, OnButtonClick)
    EVENT_MAP_END()

        //HostWnd真实窗口消息处理
    BEGIN_MSG_MAP_EX(CMessageBox)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        CHAIN_MSG_MAP(SHostWnd)
        REFLECT_NOTIFICATIONS_EX()
    END_MSG_MAP()

    int GetIconIndex(UINT style);
    void PrepareIconShow();
    BOOL PrepareButtonShow();
    void PrepareWindowSize();
protected:
    SStringT m_strCaption;
    SStringT m_strText;
    UINT m_Style;
    BOOL m_bLayoutInited;
};


INT_PTR UIMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

