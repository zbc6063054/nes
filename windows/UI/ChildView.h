
// ChildView.h : CChildView ��Ľӿ�
//
#pragma once
// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

private:
	CDC mMemDc;
	CBitmap mMemBitmap;
// ����
public:

// ����
public:
	void DrawToMem();
// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnEraseBkgnd(CDC*);
// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

