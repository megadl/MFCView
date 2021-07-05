#include <afxwin.h>
#include <afxext.h>
#include "resource.h"

class CMyDoc :public CDocument {
	 
};

class CMyView :public CView {
	DECLARE_DYNCREATE(CMyView)//动态创建机制
	DECLARE_MESSAGE_MAP()//自己写WM_CREATE消息处理函数，不使用父类的
public:
	virtual void OnDraw(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT pcs);
};
IMPLEMENT_DYNCREATE(CMyView, CView)

BEGIN_MESSAGE_MAP(CMyView, CView)
	ON_WM_CREATE()
END_MESSAGE_MAP()
void CMyView::OnDraw(CDC* pDC) {
	pDC->TextOutA(0, 0, "I'm CView");
}
int CMyView::OnCreate(LPCREATESTRUCT pcs)
{
	return CView::OnCreate(pcs);//CMyView调用父类的WM_CREATE消息处理函数，建立文档类和视图类之间的联系
}
class CMyFrameWnd :public CFrameWnd {
	//若不是公有CFrameWnd，就不能new CMyFrameWnd
	DECLARE_MESSAGE_MAP()
public:
	CSplitterWnd split;//注意不规则窗口对象的生命周期，若在类外定义，会导致不规则窗口无法实现。
public:
	afx_msg int OnCreate(LPCREATESTRUCT psc);
	afx_msg void OnPaint();
	virtual BOOL OnCreateClient(LPCREATESTRUCT pcs, CCreateContext* pContext);
};
BEGIN_MESSAGE_MAP(CMyFrameWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

class CMyWinApp :public CWinApp {

public:
	virtual BOOL InitInstance();
};

BOOL CMyWinApp::InitInstance() {
	CMyFrameWnd* pFrame = new CMyFrameWnd;
	CMyDoc* pDoc = new CMyDoc;
	CCreateContext cct;// Creation information structure
	cct.m_pCurrentDoc = pDoc;//文档类对象地址
	cct.m_pNewViewClass = RUNTIME_CLASS(CMyView);//(&class_name::classCMyView)，返回的就是CMyVIew静态变量classCMyView的地址
	pFrame->LoadFrame(IDR_MENU1, WS_OVERLAPPEDWINDOW, NULL, &cct);//一直以来都是Create但是从现在开始用LoadFrame，LoadFrame里调用了Create函数
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}
//自己写框架窗口的ON_WM_CREATE消息处理函数
int CMyFrameWnd::OnCreate(LPCREATESTRUCT pcs) {
	//return 0;//不仅返回0，还把父类的WM_CREATE消息处理函数给屏蔽了，这样一来就无法显示视图窗口
	return CFrameWnd::OnCreate(pcs);//使用父类的OnCreate，动态创建视图类对象，并创建视图窗口。pcs可以得到CREATESTRUCT cs的12个成员变量
}

void CMyFrameWnd::OnPaint() {
	PAINTSTRUCT ps = { 0 };
	HDC hdc = ::BeginPaint(this->m_hWnd, &ps);
	::TextOut(hdc, 200, 200, "我是框架窗口客户区域", 20);
	::EndPaint(m_hWnd, &ps);
}

BOOL CMyFrameWnd::OnCreateClient(LPCREATESTRUCT pcs, CCreateContext* pContext)
{	//创建多个视图窗口
	split.CreateStatic(this, 1, 2);//创建不规则框架窗口
	split.CreateView(0, 0, RUNTIME_CLASS(CMyView), CSize(100, 100), pContext);//创建视图窗口来填充框架窗口的客户区域
	split.CreateView(0, 1, pContext->m_pNewViewClass, CSize(100, 100), pContext);//创建视图窗口来填充框架窗口的客户区域
	return TRUE;
}

CMyWinApp theApp;