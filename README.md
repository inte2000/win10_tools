# win10_tools

  Windows 10 interface adjustment tool supports automatic switching of light and dark modes, automatic switching of themes and transparent setting of taskbar

## theme mode setting

![](https://github.com/inte2000/win10_tools/blob/main/doc/darkmode.png)

## taskbar setting



## desktop theme setting

![](https://github.com/inte2000/win10_tools/blob/main/doc/desktop.png)

## options

# complie

## Third party library dependency

win10_ Tools uses the **boost** library as an extension of the standard library, **libiconv** for string encoding, **xconfig2** as the configuration persistence storage scheme, **soui2** as the user interface solution, and **WTL** as a small number of window message crack.

Boost, libiconv, WTL and soui2 are widely known and widely used open source libraries. While Xconfig2 is not, but it is not far away, just here:  https://github.com/inte2000/xconfig2

boost     1.70
libIconv  1.16
soui2     
WTL       10.0.xxxx
xconfig2  0.1.7


  modify utilities.dll, add

    int UTILITIES_API ScaleFromSystemDpi(UINT dpi);
    UINT UTILITIES_API GetSystemDeviceDpi();


?    
    int ScaleFromSystemDpi(UINT dpi)
    {
        int nScale = dpi * 100 / 96;
        return nScale;
    }
    
    UINT GetSystemDeviceDpi()
    {
        HDC hDCScreen = GetDC(NULL);
        UINT dpiX = GetDeviceCaps(hDCScreen, LOGPIXELSX);
        UINT dpiY = GetDeviceCaps(hDCScreen, LOGPIXELSY);
        ReleaseDC(NULL, hDCScreen);
    
        return dpiY;
    }

SWindow
    增加变量 m_bUsingSolidBkgnd
    
	void SWindow::SetBkgndColor(COLORREF color, BOOL bUpdate)
	{
		SStringW strColor;
		strColor.Format(L"rgb(%u, %u, %u)", GetRValue(color), GetGValue(color), GetBValue(color));
		SetAttribute(L"colorBkgnd", strColor, bUpdate);
	}
	
	COLORREF SWindow::GetBkgndColor() const
	{
		return GetStyle().GetBkgndColor();
	}
	
	void SWindow::SetForceSolidBkgnd(BOOL bUsingSolidBkgnd, BOOL bUpdate)
	{
		m_bUsingSolidBkgnd = bUsingSolidBkgnd;
		if (bUpdate)
			Invalidate();
	}


?    
	BOOL SWindow::OnEraseBkgnd(IRenderTarget *pRT)
	{
		CRect rcClient=GetClientRect();
		if (!m_pBgSkin || m_bUsingSolidBkgnd)
		{
			COLORREF crBg = m_style.m_crBg;



