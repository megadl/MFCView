#include <afxwin.h>
#include "resource.h"
//视图窗口类，（所有窗口类都）从CWnd派生
class CMyView :public CView {
	DECLARE_MESSAGE_MAP()
public:
	void OnDraw(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNew();
};
BEGIN_MESSAGE_MAP(CMyView, CView)
	//ON_WM_PAINT()
	//处理菜单栏中灰色的按钮New
	ON_COMMAND(ID_FILE_NEW,OnNew)
END_MESSAGE_MAP()

class CMyFrameWnd :public CFrameWnd {
	//验证框架窗口客户区
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT pcs);
	afx_msg void OnNew();
};
BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_NEW, OnNew)
END_MESSAGE_MAP()

class CMyWinApp :public CWinApp {
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL InitInstance();
	afx_msg void OnNew();

};
BEGIN_MESSAGE_MAP(CMyWinApp, CWinApp)
	ON_COMMAND(ID_FILE_NEW, OnNew)
END_MESSAGE_MAP()
BOOL CMyWinApp::InitInstance()
{
	CMyFrameWnd* pFrame = new CMyFrameWnd;
	pFrame->Create(NULL, "mfcView",WS_OVERLAPPEDWINDOW, CFrameWnd::rectDefault,NULL, (CHAR*)IDR_MENU1);
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

void CMyWinApp::OnNew()
{
	AfxMessageBox("应用程序类CMyWinApp处理了ON_COMMAND消息");
}

CMyWinApp theApp;

void CMyFrameWnd::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(m_hWnd, &ps);
	::TextOut(hdc, 0, 0, "Hello, Windows! This is FrameWnd from OnPaint.", 46);
	::EndPaint(m_hWnd, &ps);
}

int CMyFrameWnd::OnCreate(LPCREATESTRUCT pcs)
{
	CMyView* pView = new CMyView;//CMyView是抽象类（拥有纯虚函数的类），不能定义对象。若要定义对象，必须重写override纯虚函数
	pView->Create(NULL, "pView", WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 200, 200), this, AFX_IDW_PANE_FIRST);//子窗口必备属性WS_CHILD | WS_VISIBLE，视图窗口是子窗口,AFX_IDW_PANE_FIRST视图窗口平铺
	m_pViewActive = pView;//创建完成视图窗口后，就自动激活，否则需要鼠标先点击视图窗口，之后才能激活视图窗口以及其组件（如菜单）
	return CFrameWnd::OnCreate(pcs);
}

void CMyFrameWnd::OnNew()
{
	AfxMessageBox("在框架窗口中创建File_New");
}

//这个是父类帮我们的派生类处理绘图消息时调用的函数，若派生类中已经使用宏写了OnPaint函数，就不会通过调用父类的OnPaint调用OnDraw
void CMyView::OnDraw(CDC* pDC)
{
	pDC->TextOut(110, 111, "CMyview::OnDraw");
}

void CMyView::OnPaint()
{
	PAINTSTRUCT ps = { 0 };
	HDC hdc = ::BeginPaint(m_hWnd, &ps);
	::TextOut(hdc, 0, 0, "hello world!", 12);
	::EndPaint(m_hWnd, &ps);
}

void CMyView::OnNew()
{
	AfxMessageBox("视图类CView处理了ON_COMMAND");
}
