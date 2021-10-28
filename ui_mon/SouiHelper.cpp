#include "stdafx.h"
#include "SouiHelper.h"

BOOL GetCheckBoxValue(SWindow* hostWnd, LPCWSTR name)
{
    SCheckBox* pChkBox = hostWnd->FindChildByName2<SCheckBox>(name);
    if (pChkBox != nullptr)
    {
       return pChkBox->IsChecked();
    }
    
    return FALSE;
}

void SetCheckBoxValue(SWindow* hostWnd, LPCWSTR name, BOOL bChecked)
{
    SCheckBox* pChkBox = hostWnd->FindChildByName2<SCheckBox>(name);
    if (pChkBox != nullptr)
    {
        pChkBox->SetCheck(bChecked);
    }
}

BOOL GetRadioBoxValue(SWindow* hostWnd, LPCWSTR name)
{
    SRadioBox* pRadBox = hostWnd->FindChildByName2<SRadioBox>(name);
    if (pRadBox != nullptr)
    {
        return pRadBox->IsChecked();
    }

    return FALSE;
}

void SetRadioBoxValue(SWindow* hostWnd, LPCWSTR name, BOOL bChecked)
{
    SRadioBox* pRadBox = hostWnd->FindChildByName2<SRadioBox>(name);
    if (pRadBox != nullptr)
    {
        pRadBox->SetCheck(bChecked);
    }
}

SStringT GetEditBoxValue(SWindow* hostWnd, LPCWSTR name, BOOL bRawText)
{
    SStringT rtnValue;

    SEdit* pEditBox = hostWnd->FindChildByName2<SEdit>(name);
    if (pEditBox != nullptr)
    {
        rtnValue = pEditBox->GetWindowText(bRawText);
    }

    return rtnValue;
}

void SetEditBoxValue(SWindow* hostWnd, LPCWSTR name, LPCTSTR value)
{
    SEdit* pEditBox = hostWnd->FindChildByName2<SEdit>(name);
    if (pEditBox != nullptr)
    {
        pEditBox->SetWindowText(value);
    }
}

SStringT GetControlText(SWindow* hostWnd, LPCWSTR name, BOOL bRawText)
{
    SStringT rtnValue;

    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        rtnValue = pSWnd->GetWindowText(bRawText);
    }

    return rtnValue;
}

void SetControlText(SWindow* hostWnd, LPCWSTR name, LPCTSTR value)
{
    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        pSWnd->SetWindowText(value);
    }
}

BOOL GetWindowVisible(SWindow* hostWnd, LPCWSTR name, BOOL bCheckParent)
{
    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        return pSWnd->IsVisible(bCheckParent);
    }

    return FALSE;
}

void SetWindowVisible(SWindow* hostWnd, LPCWSTR name, BOOL visible, BOOL update)
{
    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        pSWnd->SetVisible(visible, update);
    }
}

BOOL GetWindowEnabled(SWindow* hostWnd, LPCWSTR name, BOOL bCheckParent)
{
    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        return !pSWnd->IsDisabled(bCheckParent);
    }

    return FALSE;
}

void SetWindowEnabled(SWindow* hostWnd, LPCWSTR name, BOOL enabled)
{
    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        pSWnd->EnableWindow(enabled, TRUE);
    }
}

void SetControlAttr(SWindow* hostWnd, LPCWSTR name, LPCTSTR attrname, LPCTSTR attrvalue)
{
    SWindow* pSWnd = hostWnd->FindChildByName2<SWindow>(name);
    if (pSWnd != nullptr)
    {
        pSWnd->SetAttribute(attrname, attrvalue, TRUE);
    }
}

SStringT GetComboCurselString(SWindow* hostWnd, LPCWSTR name, BOOL bRawText)
{
    SStringT rtnText;
    SComboBox* pCombo = hostWnd->FindChildByName2<SComboBox>(name);
    if (pCombo != nullptr)
    {
        int idx = pCombo->GetCurSel();
        if (idx >= 0)
        {
            rtnText = pCombo->GetLBText(idx, bRawText);
        }
    }

    return rtnText;
}

BOOL MatchComboString(SWindow* hostWnd, LPCWSTR name, LPCTSTR strvalue)
{
    BOOL bFindMatch = FALSE;
    SComboBox* pCombo = hostWnd->FindChildByName2<SComboBox>(name);
    if (pCombo != nullptr)
    {
        for (int i = 0; i < pCombo->GetCount(); i++)
        {
            SStringT lbText = pCombo->GetLBText(i, TRUE);
            if (lbText.Compare(strvalue) == 0)
            {
                if (i != pCombo->GetCurSel())
                {
                    pCombo->SetCurSel(i);
                }
                bFindMatch = TRUE;
                break;
            }
        }
    }

    return bFindMatch;
}


int GetComboCursel(SWindow* hostWnd, LPCWSTR name)
{
    SComboBox* pCombo = hostWnd->FindChildByName2<SComboBox>(name);
    if (pCombo != nullptr)
    {
        return pCombo->GetCurSel();
    }

    return -1;
}

BOOL SetComboCursel(SWindow* hostWnd, LPCWSTR name, int idx)
{
    SComboBox* pCombo = hostWnd->FindChildByName2<SComboBox>(name);
    if (pCombo != nullptr)
    {
        if (pCombo->GetCount() > idx)
        {
            pCombo->SetCurSel(idx);
            return TRUE;
        }
    }

    return FALSE;
}

void SetProgressValue(SWindow* hostWnd, LPCWSTR name, int value)
{
    SProgress* pProgress = hostWnd->FindChildByName2<SProgress>(name);
    if (pProgress != nullptr)
    {
        pProgress->SetValue(value);
    }
}

void SetSliderValue(SWindow* hostWnd, LPCWSTR name, int value)
{
    SSliderBar* pProgress = hostWnd->FindChildByName2<SSliderBar>(name);
    if (pProgress != nullptr)
    {
        pProgress->SetValue(value);
    }
}

BOOL SetImageWndImage(SWindow* hostWnd, LPCWSTR name, LPCTSTR strFile, LPCTSTR strTip)
{
    SImageWnd* pImg = hostWnd->FindChildByName2<SImageWnd>(name);
    if (pImg != nullptr)
    {
        SStringW decname = S_CT2W(strFile);
        IBitmap* image = LOADIMAGE2(L"file:" + decname);
        if (image)
        {
            pImg->SetImage(image);
            if (strTip != nullptr)
            {
                pImg->SetToolTipText(strTip);
            }
            image->Release();

            return TRUE;
        }
    }

    return FALSE;
}

BOOL ResetImageWnd(SWindow* hostWnd, LPCWSTR name, LPCTSTR strTip)
{
    SImageWnd* pImg = hostWnd->FindChildByName2<SImageWnd>(name);
    if (pImg != nullptr)
    {
        pImg->SetImage(NULL);
        if (strTip != nullptr)
        {
            pImg->SetToolTipText(strTip);
        }

        return TRUE;
    }

    return FALSE;
}
