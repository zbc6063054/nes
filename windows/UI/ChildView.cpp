
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "ZNes.h"
#include "ChildView.h"
#include "../../core/nes.h"
#include "../../core/controller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern Nes nes;

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	mMemBitmap.DeleteObject();
	mMemDc.DeleteDC();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

struct {
	UINT sys_key;
	Controller::Key game_key;
} keyMapper[] = {
	{ VK_UP, Controller::KEY_UP } ,
	{ VK_DOWN, Controller::KEY_DOWN } ,
	{ VK_LEFT, Controller::KEY_LEFT } ,
	{ VK_RIGHT, Controller::KEY_RIGHT } ,
	{ 'Z', Controller::KEY_A } ,
	{ 'X', Controller::KEY_B } ,
	{ 'B', Controller::KEY_START } ,
	{ 'N', Controller::KEY_SELECT } ,
};

// CChildView 

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); 
	CRect winRect;
	this->GetClientRect(&winRect);

	dc.StretchBlt(0, 0, winRect.Width(), winRect.Height(), &mMemDc, 8, 0, 256, 240, SRCCOPY);
}
void RefreshFunc(void *data){
	CChildView* view = ((CChildView *)data);
	view->DrawToMem();
	view->Invalidate(TRUE);
	view->SendMessage(WM_PAINT);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	CDC* pDc = GetDC();
	mMemDc.CreateCompatibleDC(pDc);
	mMemBitmap.CreateCompatibleBitmap(pDc, 256+16, 240);
	mMemDc.SelectObject(mMemBitmap);
	mMemDc.SetBkMode(TRANSPARENT);
	mMemDc.SetTextColor(0xFFFFFF);
	ReleaseDC(pDc);

	nes.setRefreshFunc(RefreshFunc, this);
	return 0;
}

BOOL CChildView::OnEraseBkgnd(CDC* pDc){
	return TRUE;
}

void CChildView::DrawToMem(){
	mMemBitmap.SetBitmapBits((256+16)*240*4, nes.ppu->getScreenData());
	CString strFps;
	strFps.Format(TEXT("%0.2f"), nes.getFps());
	mMemDc.TextOut(20, 10, strFps);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRect wRect, cRect;
	GetParent()->GetWindowRect(&wRect);
	GetClientRect(&cRect);
	wRect.bottom += 240 - cRect.Height();
	wRect.right += 256 - cRect.Width();
	GetParent()->MoveWindow(wRect.left, wRect.top, wRect.Width(), wRect.Height(), FALSE);

	int len = sizeof(keyMapper) / sizeof(keyMapper[0]);
	int i = 0;
	for(i=0; i<len; i++){
		if(nChar == keyMapper[i].sys_key)
			nes.control->setKeyState(keyMapper[i].game_key, true);
	}
	if(i == len){
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}


void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int len = sizeof(keyMapper) / sizeof(keyMapper[0]);
	int i = 0;
	for(i=0; i<len; i++){
		if(nChar == keyMapper[i].sys_key)
			nes.control->setKeyState(keyMapper[i].game_key, false);
	}
	if(nChar == ' '){
		if(nes.isRunning())
			nes.pause();
		else
			nes.resume();
	}else if(i == len){
		CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}
