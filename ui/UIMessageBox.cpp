#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

#include "UIMessageBox.h"


const WCHAR* const kMessageboxTitleName = L"msgtitle";
const WCHAR* const kMessageboxTextName = L"msgtext";
const WCHAR* const kMessageboxIconName = L"msgicon";
const WCHAR* const kMessageboxBtnSwtichName = L"btnSwitch";
const WCHAR* const kMessageboxBtn1Name = L"button1st";
const WCHAR* const kMessageboxBtn2Name = L"button2nd";
const WCHAR* const kMessageboxBtn3Name = L"button3rd";


struct MSGBTN_TEXT
{
    int   nBtns;    //按钮数,<=3
    struct
    {
        UINT uBtnID;//按钮ID
        WCHAR szText[32]; //按钮字符    
    }btnInfo[3];
}g_msgBtnText[] =
{
    { 1, { {IDOK,    L"ok"},     {0,        L""},       {0,        L""}        } },  //MB_OK
    { 2, { {IDOK,    L"ok"},     {IDCANCEL, L"cancel"}, {0,        L""}        } },  //MB_OKCANCEL
    { 3, { {IDABORT, L"abort"},  {IDRETRY,  L"retry"},  {IDIGNORE, L"ignore"}  } },  //MB_ABORTRETRYIGNORE
    { 3, { {IDYES,   L"yes"},    {IDNO,     L"no"},     {IDCANCEL, L"cancel"}  } },  //MB_YESNOCANCEL
    { 2, { {IDYES,   L"yes"},    {IDNO,     L"no"},     {0,        L""}        } },  //MB_YESNO
    { 2, { {IDRETRY, L"retry"},  {IDCANCEL, L"cancel"}, {0,        L""}        } },   //MB_RETRYCANCEL
};

const WCHAR* g_wcsNameOfBtns[] =
{
    kMessageboxBtn1Name,
    kMessageboxBtn2Name,
    kMessageboxBtn3Name
};

typedef struct tagMB_ICON_DEF
{
    UINT id;
    int iconIdx;
}MB_ICON_DEF;

static MB_ICON_DEF g_iconDef[] = 
{
    { MB_ICONEXCLAMATION,    0 },
    { MB_ICONWARNING,        1 },
    { MB_ICONINFORMATION,    2 },
    { MB_ICONASTERISK,       3 },
    { MB_ICONQUESTION,       4 },
    { MB_ICONSTOP,           5 },
    { MB_ICONERROR,          6 },
    { MB_ICONHAND,           7 }
};

//用户自定义图标，以后支持
//#define MB_USERICON                 0x00000080L

CMessageBox::CMessageBox() : SHostDialog(_T("COMMON_UI:uimessagebox"))
{
    m_bLayoutInited = FALSE;
}

CMessageBox::CMessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) : SHostDialog(_T("COMMON_UI:uimessagebox"))
{
    m_strCaption = lpCaption;
    m_strText = lpText;
    m_Style = uType;
    m_bLayoutInited = FALSE;
}

void CMessageBox::SetInformation(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    m_strCaption = lpCaption;
    m_strText = lpText;
    m_Style = uType;
}

BOOL CMessageBox::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(hWnd);
    UNREFERENCED_PARAMETER(lParam);

    SStatic* pTitle = FindChildByName2<SStatic>(kMessageboxTitleName);
    if (pTitle != nullptr)
        pTitle->SetWindowText(m_strCaption);

    SStatic* pMsgText = FindChildByName2<SStatic>(kMessageboxTextName);
    if (pMsgText != nullptr)
        pMsgText->SetWindowText(m_strText);

    PrepareIconShow();
    PrepareButtonShow();
    PrepareWindowSize();

    m_bLayoutInited = TRUE;

    return 0;
}

//TODO:消息映射
void CMessageBox::OnClose()
{
    EndDialog(IDCANCEL);
}

int CMessageBox::GetIconIndex(UINT style)
{
    UINT uIconType = style & 0x000000F0;

    for(int i = 0; i < sizeof(g_iconDef) / sizeof(g_iconDef[0]); i++)
    {
        if(g_iconDef[i].id == uIconType)
        {
            return g_iconDef[i].iconIdx;
        }
    }

    return -1;
}

void CMessageBox::PrepareIconShow()
{
    SImageWnd* pImgWnd = FindChildByName2<SImageWnd>(kMessageboxIconName);
    if (pImgWnd != nullptr)
    {
        int iconidx = GetIconIndex(m_Style);
        if (iconidx == -1)
        {
            pImgWnd->SetVisible(false, TRUE);
        }
        else
        {
            pImgWnd->SetIcon(iconidx);
        }
    }
}

BOOL CMessageBox::PrepareButtonShow()
{
    STabCtrl* pBtnSwitch = FindChildByName2<STabCtrl>(kMessageboxBtnSwtichName);
    if (pBtnSwitch == nullptr)
        return FALSE;
    
    UINT uBtnType = m_Style & 0x0000000F;
    if (uBtnType > MB_CANCELTRYCONTINUE)
        return FALSE;

    pBtnSwitch->SetCurSel(g_msgBtnText[uBtnType].nBtns - 1);
    SWindow* pBtnPanel = pBtnSwitch->GetItem(g_msgBtnText[uBtnType].nBtns - 1);
    if (pBtnPanel == nullptr)
        return FALSE;

    for (int i = 0; i < g_msgBtnText[uBtnType].nBtns; i++)
    {
        SWindow* pBtn = pBtnPanel->FindChildByName(g_wcsNameOfBtns[i]);
        pBtn->SetID(g_msgBtnText[uBtnType].btnInfo[i].uBtnID);
        SStringT transText = TR(g_msgBtnText[uBtnType].btnInfo[i].szText, L"messagebox");
        pBtn->SetWindowText(transText);
    }

    return TRUE;
}

void CMessageBox::PrepareWindowSize()
{
    SHostWndAttr& hostAttr = GetHostAttr();
    CSize minSize = hostAttr.GetMinSize(GetScale());
    
    UpdateLayout();

    CRect rcWnd = GetWindowRect();
    CSize szWnd = rcWnd.Size();
    if (szWnd.cx < minSize.cx)
        szWnd.cx = minSize.cx;
    if (szWnd.cy < minSize.cy)
        szWnd.cy = minSize.cy;

    SetWindowPos(0, 0, 0, szWnd.cx, szWnd.cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

    CenterWindow();
}

INT_PTR UIMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
    CMessageBox msgBox;

    msgBox.SetInformation(lpText, lpCaption, uType);
    return msgBox.DoModal(hWnd);
}
