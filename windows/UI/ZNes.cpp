
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ZNes.h"
#include "MainFrm.h"
#include "../../core/nes.h"
#include <io.h>
#include <fcntl.h>

//#ifdef _DEBUG
//#pragma comment( lib, "../debug/core.lib" )
//#else
//#pragma comment( lib, "../Release/core.lib" )
//#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#include <vld.h>
#endif

Nes nes;

// CnesApp

BEGIN_MESSAGE_MAP(CnesApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CnesApp::OnAppAbout)
END_MESSAGE_MAP()

void InitConsole()
{
	int nRet= 0;
	FILE* fp;
	AllocConsole();
	nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nRet, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

CnesApp::CnesApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
#ifdef _DEBUG
	InitConsole();
#endif
	//CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("nes.AppID.NoVersion"));
}

CnesApp theApp;

// CnesApp 

BOOL CnesApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);
	SetRegistryKey(_T("ZNes"));

	nes.init();

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

int CnesApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// AboutDlg 

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void CnesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}