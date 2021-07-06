#include <afxwin.h>
#include <afxext.h>
#include "resource.h"

class CMyDoc :public CDocument {
	 DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSave();

	CString str;//模拟文档类管理的数据
};
BEGIN_MESSAGE_MAP(CMyDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, OnSave)
END_MESSAGE_MAP()
void CMyDoc::OnSave()
{
	str = "hello world";
	//UpdateAllViews(NULL);//刷新和这个文档类对象（this）关联的所有视图窗口
	POSITION pos = this->GetFirstViewPosition();//GetFirstxxxPosition()获取链表的迭代器，返回位置信息，指向链表头节点的钱一个位置dummy（你懂的）
	CView* pView = this->GetNextView(pos);//dummy.GetNextView()获取真的头节点
	UpdateAllViews(pView);
}
class CMyView :public CView {
	DECLARE_DYNCREATE(CMyView)//动态创建机制
	DECLARE_MESSAGE_MAP()//自己写WM_CREATE消息处理函数，不使用父类的
public:
	virtual void OnDraw(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT pcs);
	afx_msg void OnSave();
};
IMPLEMENT_DYNCREATE(CMyView, CView)

BEGIN_MESSAGE_MAP(CMyView, CView)
	ON_WM_CREATE()
	//ON_COMMAND(ID_FILE_SAVE,OnSave)
END_MESSAGE_MAP()
void CMyView::OnDraw(CDC* pDC) {
	CMyDoc* pDoc = (CMyDoc*)this->m_pDocument;//通过视图类对象的成员变量获取相关联的文档类
	pDC->TextOutA(0, 0, pDoc->str);//把文档中管理的数据刷新到视图窗口
}
int CMyView::OnCreate(LPCREATESTRUCT pcs)
{
	return CView::OnCreate(pcs);//CMyView调用父类的WM_CREATE消息处理函数，建立文档类和视图类之间的联系
}
void CMyView::OnSave()
{
	AfxMessageBox("在视图类中处理了ON_COMMAND消息");
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
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL InitInstance();
	afx_msg void OnSave();
};
BEGIN_MESSAGE_MAP(CMyWinApp, CWinApp)
	ON_COMMAND(ID_FILE_SAVE,OnSave)
END_MESSAGE_MAP()

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
void CMyWinApp::OnSave()
{
	AfxMessageBox("应用程序类处理了ON_COMMAND");
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
	//选取默认激活窗口为左侧视图窗口
	m_pViewActive = (CView*)split.GetPane(0,0);//倒下来的日字视图框架，(0,0)代表左边的视图,注意类型转换
	return TRUE;
}

CMyWinApp theApp;