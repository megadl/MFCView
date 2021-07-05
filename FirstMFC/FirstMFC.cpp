#include <afxwin.h>

class CMyFrameWnd :public CFrameWnd {

};

class CMyWinApp :public CWinApp {
public:
	CMyWinApp()
	{

	}
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