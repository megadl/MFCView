#include <afxwin.h>

//��ͼ�����࣬�����д����඼����CWnd����
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
	//��֤��ܴ��ڿͻ���
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
	CMyView* pView = new CMyView;//CMyView�ǳ����ࣨӵ�д��麯�����ࣩ�����ܶ��������Ҫ������󣬱�����дoverride���麯��
	pView->Create(NULL, "pView", WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 200, 200), this, AFX_IDW_PANE_FIRST);//�Ӵ��ڱر�����WS_CHILD | WS_VISIBLE����ͼ�������Ӵ���,AFX_IDW_PANE_FIRST��ͼ����ƽ��
	return CFrameWnd::OnCreate(pcs);
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
