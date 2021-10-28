#include "stdafx.h"
#include "UIFrmWndImp.h"


void CFrmWndImpl::SetWindowTitle(LPCTSTR lpstrTitle)
{
    SStatic* pFtmTitle = FindChildByName2<SStatic>(L"win_title");
    if (pFtmTitle != nullptr)
    {
        pFtmTitle->SetWindowText(lpstrTitle);
    }

    ::SetWindowText(m_hWnd, lpstrTitle);
}

void CFrmWndImpl::SetWindowBkgndMode(BOOL bUsingSolidColor, COLORREF color)
{
    SWindow* pMainFrame = FindChildByName(L"MainFrame");
    if (pMainFrame != nullptr)
    {
        if (bUsingSolidColor)
        {
            pMainFrame->SetBkgndColor(color);
        }

        pMainFrame->SetForceSolidBkgnd(bUsingSolidColor, TRUE);
    }
}

void CFrmWndImpl::SetWindowTransparency(unsigned int alpha)
{
    SWindow* pMainFrame = FindChildByName(L"MainFrame");
    if (pMainFrame != nullptr)
    {
        pMainFrame->GetStyle().SetAlpha((BYTE)alpha);
        pMainFrame->Invalidate();
    }
}

BOOL CFrmWndImpl::OnNcActivate(BOOL bActive)
{
	if(::IsIconic(m_hWnd)) SetMsgHandled(FALSE); 

    SCaption* pCaption = FindChildByName2<SCaption>(L"caption");
    if(pCaption != NULL)
    {
        pCaption->SetActive(bActive, TRUE);
    }

    SetMsgHandled(FALSE);
    return TRUE;
}

void CFrmWndImpl::OnSysMenuCommand(UINT nID)
{
    switch (nID)
    {
    case SYSMENU_RESTORE_ID:
        OnSysBtnRestore();
        break;
    case SYSMENU_MOVE_ID:
        OnSysBtnMove();
        break;
    case SYSMENU_SIZE_ID:
        OnSysBtnSize();
        break;
    case SYSMENU_MINIMIZE_ID:
        OnSysBtnMinimize();
        break;
    case SYSMENU_MAXIMIZE_ID:
        OnSysBtnMaximize();
        break;
    case SYSMENU_CLOSE_ID:
        OnSysBtnClose();
        break;
    }
}

void CFrmWndImpl::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
    BOOL bProcessed = FALSE;
    if (uNotifyCode == 0)
    {
        if ((nID >= SYSMENU_RESTORE_ID) && (nID <= SYSMENU_CLOSE_ID))
        {
            OnSysMenuCommand(nID);
            bProcessed = TRUE;
        }
    }

    SetMsgHandled(bProcessed);
}

void CFrmWndImpl::OnSysCommand(UINT nID, CPoint lParam)
{
	if (nID == SC_CLOSE)
	{
		SetMsgHandled(TRUE); 
		SendMessage(WM_CLOSE);
		return;
	}

    BOOL bZoomed = ::IsZoomed(m_hWnd);
    SHostWnd::DefWindowProc();
    SCaption* pCaption = FindChildByName2<SCaption>(L"caption");
    if (pCaption != NULL)
    {
        pCaption->RelayoutSysButtons(bZoomed);
    }

    SetMsgHandled(TRUE); 
}

