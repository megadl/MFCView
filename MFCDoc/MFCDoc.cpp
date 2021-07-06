#include <afxwin.h>
#include <afxext.h>
#include "resource.h"

class CMyDoc :public CDocument {
	 DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSave();

	CString str;//ģ���ĵ�����������
};
BEGIN_MESSAGE_MAP(CMyDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, OnSave)
END_MESSAGE_MAP()
void CMyDoc::OnSave()
{
	str = "hello world";
	//UpdateAllViews(NULL);//ˢ�º�����ĵ������this��������������ͼ����
	POSITION pos = this->GetFirstViewPosition();//GetFirstxxxPosition()��ȡ����ĵ�����������λ����Ϣ��ָ������ͷ�ڵ��Ǯһ��λ��dummy���㶮�ģ�
	CView* pView = this->GetNextView(pos);//dummy.GetNextView()��ȡ���ͷ�ڵ�
	UpdateAllViews(pView);
}
class CMyView :public CView {
	DECLARE_DYNCREATE(CMyView)//��̬��������
	DECLARE_MESSAGE_MAP()//�Լ�дWM_CREATE��Ϣ����������ʹ�ø����
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
	CMyDoc* pDoc = (CMyDoc*)this->m_pDocument;//ͨ����ͼ�����ĳ�Ա������ȡ��������ĵ���
	pDC->TextOutA(0, 0, pDoc->str);//���ĵ��й��������ˢ�µ���ͼ����
}
int CMyView::OnCreate(LPCREATESTRUCT pcs)
{
	return CView::OnCreate(pcs);//CMyView���ø����WM_CREATE��Ϣ�������������ĵ������ͼ��֮�����ϵ
}
void CMyView::OnSave()
{
	AfxMessageBox("����ͼ���д�����ON_COMMAND��Ϣ");
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
	cct.m_pCurrentDoc = pDoc;//�ĵ�������ַ
	cct.m_pNewViewClass = RUNTIME_CLASS(CMyView);//(&class_name::classCMyView)�����صľ���CMyVIew��̬����classCMyView�ĵ�ַ
	pFrame->LoadFrame(IDR_MENU1, WS_OVERLAPPEDWINDOW, NULL, &cct);//һֱ��������Create���Ǵ����ڿ�ʼ��LoadFrame��LoadFrame�������Create����
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}
void CMyWinApp::OnSave()
{
	AfxMessageBox("Ӧ�ó����ദ����ON_COMMAND");
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
	//ѡȡĬ�ϼ����Ϊ�����ͼ����
	m_pViewActive = (CView*)split.GetPane(0,0);//��������������ͼ��ܣ�(0,0)������ߵ���ͼ,ע������ת��
	return TRUE;
}

CMyWinApp theApp;