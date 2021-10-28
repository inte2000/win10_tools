#pragma once

BOOL GetCheckBoxValue(SWindow *hostWnd, LPCWSTR name);
void SetCheckBoxValue(SWindow* hostWnd, LPCWSTR name, BOOL bChecked);

BOOL GetRadioBoxValue(SWindow* hostWnd, LPCWSTR name);
void SetRadioBoxValue(SWindow* hostWnd, LPCWSTR name, BOOL bChecked);

SStringT GetEditBoxValue(SWindow* hostWnd, LPCWSTR name, BOOL bRawText = TRUE);
void SetEditBoxValue(SWindow* hostWnd, LPCWSTR name, LPCTSTR value);

SStringT GetControlText(SWindow* hostWnd, LPCWSTR name, BOOL bRawText = TRUE);
void SetControlText(SWindow* hostWnd, LPCWSTR name, LPCTSTR value);

BOOL GetWindowVisible(SWindow* hostWnd, LPCWSTR name, BOOL bCheckParent = FALSE);
void SetWindowVisible(SWindow* hostWnd, LPCWSTR name, BOOL visible, BOOL update = FALSE);

BOOL GetWindowEnabled(SWindow* hostWnd, LPCWSTR name, BOOL bCheckParent = FALSE);
void SetWindowEnabled(SWindow* hostWnd, LPCWSTR name, BOOL enabled);

void SetControlAttr(SWindow* hostWnd, LPCWSTR name, LPCTSTR attrname, LPCTSTR attrvalue);

SStringT GetComboCurselString(SWindow* hostWnd, LPCWSTR name, BOOL bRawText = TRUE);
BOOL MatchComboString(SWindow* hostWnd, LPCWSTR name, LPCTSTR strvalue);

int GetComboCursel(SWindow* hostWnd, LPCWSTR name);
BOOL SetComboCursel(SWindow* hostWnd, LPCWSTR name, int idx);

void SetProgressValue(SWindow* hostWnd, LPCWSTR name, int value);
void SetSliderValue(SWindow* hostWnd, LPCWSTR name, int value);

BOOL SetImageWndImage(SWindow* hostWnd, LPCWSTR name, LPCTSTR strFile, LPCTSTR strTip);
BOOL ResetImageWnd(SWindow* hostWnd, LPCWSTR name, LPCTSTR strTip = NULL);
