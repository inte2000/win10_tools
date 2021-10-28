// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "MainDlg.h"
#include "SShellNotifyIcon.h"
#include "Global.h"


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
    HRESULT hRes = OleInitialize(NULL);
    SASSERT(SUCCEEDED(hRes));

    if (!InitGlobalData())
    {
        ::MessageBox(::GetDesktopWindow(), _T("This tool need windows 10 1809 or higher version!"), _T("Win10 tools"), MB_OK | MB_ICONERROR);
        OleUninitialize();
        return -1;
    }
    int nRet = 0;
    SComMgr *pComMgr = new SComMgr;

    {
        BOOL bLoaded = FALSE;
        CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
        CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
        CAutoRefPtr<SOUI::ITranslatorMgr> pTransMgr;    //多语言翻译模块，由translator.dll提供

        //bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
        bLoaded = pComMgr->CreateRender_Skia((IObjRef**)&pRenderFactory);
        SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
        bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
        SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));
        bLoaded = pComMgr->CreateTranslator((IObjRef**)&pTransMgr);
        SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("translator"));

        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

        SApplication *theApp=new SApplication(pRenderFactory,hInstance);
        pComMgr->SetDllPath(theApp->GetAppDir());
        ::SetCurrentDirectory(theApp->GetAppDir());

        SetAppConfigInfo(theApp->GetAppDir() + _T("\\config.xml"));
        CAppConfig& appCfg = GetAppConfig();
        SStringT tThemeName = appCfg.GetUIThemeName().c_str();
        if (tThemeName.IsEmpty())
        {
            tThemeName = _T("blue");
            appCfg.SetUIThemeName(tThemeName);
        }

        CAutoRefPtr<IResProvider>   pResProvider;

#ifdef _DEBUG
        SStringT strSkinLoc = theApp->GetAppDir() + _T("\\skin\\") + tThemeName;
        CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
        if (!pResProvider->Init((LPARAM)(LPCTSTR)strSkinLoc, 0))
        {
            SASSERT(0);
            return 1;
        }
#else
        tThemeName += _T(".zip");
        SStringT strSkinLoc = theApp->GetAppDir() + _T("\\skin\\") + tThemeName;
        //pComMgr->CreateResProvider_7ZIP((IObjRef**)&pResProvider);
        bLoaded = pComMgr->CreateResProvider_ZIP((IObjRef**)&pResProvider);
        if (bLoaded)
        {
            ZIPRES_PARAM param;
            param.ZipFile(pRenderFactory, strSkinLoc, "souizip");
            bLoaded = pResProvider->Init((WPARAM)&param, 0);
        }
#endif

        theApp->AddResProvider(pResProvider);
        theApp->RegisterWindowClass<SShellNotifyIcon>();

        SStringT tlangname = appCfg.GetLanguage().c_str();
        if (tlangname.IsEmpty())
        {
            LANGID lid = GetSystemDefaultLangID();
            if (lid == 0x0804)
                tlangname = _T("chs");
            else
                tlangname = _T("eng");

            appCfg.SetLanguage(tlangname);
        }

        //语言包加载
        theApp->SetTranslator(pTransMgr);
        pugi::xml_document xmlLang;
        if (theApp->LoadXmlDocment(xmlLang, tlangname, _T("LANG")))
        {
            CAutoRefPtr<ITranslator> langCN;
            pTransMgr->CreateTranslator(&langCN);
            langCN->Load(&xmlLang.child(L"language"), 1);//1=LD_XML
            pTransMgr->InstallTranslator(langCN);
        }

        SNotifyCenter* pNotifyCenter = new SNotifyCenter;
        // BLOCK: Run application
        {
            CMainDlg dlgMain;  
            dlgMain.Create(GetActiveWindow(),0,0,0,0);
            dlgMain.SendMessage(WM_INITDIALOG);
            dlgMain.CenterWindow(dlgMain.m_hWnd);
            dlgMain.ShowWindow(SW_SHOWNORMAL);
            nRet=theApp->Run(dlgMain.m_hWnd);
        }
        delete pNotifyCenter;

        delete theApp;
    }
    
    delete pComMgr;
    OleUninitialize();
    return nRet;
}
