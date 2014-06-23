
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "ZNes.h"
#include "../../core/nes.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern Nes nes;

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame DEBUG

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnFileOpen()
{
	TCHAR szFilter[] = TEXT("Nes Files (*.nes)|*.nes|");

	nes.pause();
	CFileDialog dialog(true, NULL, NULL, 6UL, szFilter, this);
	if (dialog.DoModal() == IDCANCEL){
		nes.resume();
		return;
	}
	nes.stop();
	USES_CONVERSION;
	int ret = nes.loadFile(W2CA(dialog.m_ofn.lpstrFile));
	if (ret != NES_ERROR_OK){
		AfxMessageBox(CA2W(GetErrorString(ret)), MB_OK | MB_ICONINFORMATION);
		return;
	}
	nes.reset();
	nes.start();
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC){
	return TRUE;
}

void CMainFrame::OnClose()
{
	nes.stop();
	CFrameWnd::OnClose();
}
