#include <afxwin.h>

//视图窗口类，（所有窗口类都）从CWnd派生
class CMyView :public CView {
	DECLARE_MESSAGE_MAP()
public:
	void OnDraw(CDC* pDC);
	afx_msg void OnPaint();
};
BEGIN_MESSAGE_MAP(CMyView, CView)
	//ON_WM_PAINT()
END_MESSAGE_MAP()

class CMyFrameWnd :public CFrameWnd {
	//验证框架窗口客户区
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT pcs);
};
BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

class CMyWinApp :public CWinApp {
public:
	virtual BOOL InitInstance();
};

BOOL CMyWinApp::InitInstance()
{
	CMyFrameWnd* pFrame = new CMyFrameWnd;
	pFrame->Create(NULL, "mfcView");
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
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
	return CFrameWnd::OnCreate(pcs);
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
