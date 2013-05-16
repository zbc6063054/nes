
// ChildView.h : CChildView 类的接口
//
#pragma once
// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

private:
	CDC mMemDc;
	CBitmap mMemBitmap;
// 特性
public:

// 操作
public:
	void DrawToMem();
// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnEraseBkgnd(CDC*);
// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

