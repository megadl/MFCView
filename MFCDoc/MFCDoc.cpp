#include <afxwin.h>
#include <afxext.h>
#include "resource.h"

class CMyDoc :public CDocument {
	 
};

class CMyView :public CView {
	DECLARE_DYNCREATE(CMyView)//��̬��������
	DECLARE_MESSAGE_MAP()//�Լ�дWM_CREATE��Ϣ����������ʹ�ø����
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
	return CView::OnCreate(pcs);//CMyView���ø����WM_CREATE��Ϣ�������������ĵ������ͼ��֮�����ϵ
}
class CMyFrameWnd :public CFrameWnd {
	//�����ǹ���CFrameWnd���Ͳ���new CMyFrameWnd
	DECLARE_MESSAGE_MAP()
public:
	CSplitterWnd split;//ע�ⲻ���򴰿ڶ�����������ڣ��������ⶨ�壬�ᵼ�²����򴰿��޷�ʵ�֡�
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
	cct.m_pCurrentDoc = pDoc;//�ĵ�������ַ
	cct.m_pNewViewClass = RUNTIME_CLASS(CMyView);//(&class_name::classCMyView)�����صľ���CMyVIew��̬����classCMyView�ĵ�ַ
	pFrame->LoadFrame(IDR_MENU1, WS_OVERLAPPEDWINDOW, NULL, &cct);//һֱ��������Create���Ǵ����ڿ�ʼ��LoadFrame��LoadFrame�������Create����
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}
//�Լ�д��ܴ��ڵ�ON_WM_CREATE��Ϣ������
int CMyFrameWnd::OnCreate(LPCREATESTRUCT pcs) {
	//return 0;//��������0�����Ѹ����WM_CREATE��Ϣ�������������ˣ�����һ�����޷���ʾ��ͼ����
	return CFrameWnd::OnCreate(pcs);//ʹ�ø����OnCreate����̬������ͼ����󣬲�������ͼ���ڡ�pcs���Եõ�CREATESTRUCT cs��12����Ա����
}

void CMyFrameWnd::OnPaint() {
	PAINTSTRUCT ps = { 0 };
	HDC hdc = ::BeginPaint(this->m_hWnd, &ps);
	::TextOut(hdc, 200, 200, "���ǿ�ܴ��ڿͻ�����", 20);
	::EndPaint(m_hWnd, &ps);
}

BOOL CMyFrameWnd::OnCreateClient(LPCREATESTRUCT pcs, CCreateContext* pContext)
{	//���������ͼ����
	split.CreateStatic(this, 1, 2);//�����������ܴ���
	split.CreateView(0, 0, RUNTIME_CLASS(CMyView), CSize(100, 100), pContext);//������ͼ����������ܴ��ڵĿͻ�����
	split.CreateView(0, 1, pContext->m_pNewViewClass, CSize(100, 100), pContext);//������ͼ����������ܴ��ڵĿͻ�����
	return TRUE;
}

CMyWinApp theApp;