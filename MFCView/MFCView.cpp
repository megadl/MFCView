#include <afxwin.h>
#include "resource.h"
//��ͼ�����࣬�����д����඼����CWnd����
class CMyView :public CView {
	DECLARE_MESSAGE_MAP()
public:
	void OnDraw(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNew();
};
BEGIN_MESSAGE_MAP(CMyView, CView)
	//ON_WM_PAINT()
	//����˵����л�ɫ�İ�ťNew
	ON_COMMAND(ID_FILE_NEW,OnNew)
END_MESSAGE_MAP()

class CMyFrameWnd :public CFrameWnd {
	//��֤��ܴ��ڿͻ���
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
	AfxMessageBox("Ӧ�ó�����CMyWinApp������ON_COMMAND��Ϣ");
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
	CMyView* pView = new CMyView;//CMyView�ǳ����ࣨӵ�д��麯�����ࣩ�����ܶ��������Ҫ������󣬱�����дoverride���麯��
	pView->Create(NULL, "pView", WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 200, 200), this, AFX_IDW_PANE_FIRST);//�Ӵ��ڱر�����WS_CHILD | WS_VISIBLE����ͼ�������Ӵ���,AFX_IDW_PANE_FIRST��ͼ����ƽ��
	m_pViewActive = pView;//���������ͼ���ں󣬾��Զ����������Ҫ����ȵ����ͼ���ڣ�֮����ܼ�����ͼ�����Լ����������˵���
	return CFrameWnd::OnCreate(pcs);
}

void CMyFrameWnd::OnNew()
{
	AfxMessageBox("�ڿ�ܴ����д���File_New");
}

//����Ǹ�������ǵ������ദ���ͼ��Ϣʱ���õĺ����������������Ѿ�ʹ�ú�д��OnPaint�������Ͳ���ͨ�����ø����OnPaint����OnDraw
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
	AfxMessageBox("��ͼ��CView������ON_COMMAND");
}
