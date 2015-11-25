/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MainFrm.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: application framework class 
               MFC structure
  Others:         
***********************************************************************************/
#include "stdafx.h"
#include "RBDcm.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(UM_PROSSESS, OnProgress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,				// ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����
CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

/***************************************************************************************
Function: �����ܴ�����Ϣ��Ӧ
Input:		void
Output:   
Description: 
Return:		void
Others:		NULL
***************************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_CAPS, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_NUM, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_SCRL, SBPS_NORMAL, 100);
	PostMessage(UM_PROSSESS);
	return 0;
}


/***************************************************************************************
Function: �����ܴ���ǰ��Ϣ��Ӧ
Input:		void
Output:   
Description: �ڴ������޸ĳ�������ʽ
Return:		void
Others:		NULL
***************************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_SYSMENU;
	cs.lpszName = "RBDcm������������ϵͳ";
	cs.style &= ~FWS_ADDTOTITLE;				//ȥ��������ʾ���ļ�����

	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);	//��ȡ������Ļ�Ĵ�С
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
	cs.y = 0;									//����������������Ļ��С
	cs.x = 0;
	return TRUE;
}

// CMainFrame ���
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/***************************************************************************************
Function:    �������д����ͻ��� ��Ϣ��Ӧ
Input:		 void
Output:   
Description: �ڴ����зָ��
Return:		 void
Others:		 NULL
***************************************************************************************/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if( !m_splt.CreateStatic(this, 1, 2) )		 //�ָ�Ϊ1��2��
	{
		AfxMessageBox("���ڷָ�ʧ�ܣ�");
		return FALSE;
	}
	CRect rct;
	GetWindowRect(&rct);
	//GetClientRect(&rct);
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	//���󶨲������
	m_splt.CreateView(0, 0, RUNTIME_CLASS(COprtView), CSize(nRcWidth / 7, nRcHeight), pContext);
	m_pOpt = (COprtView*)m_splt.GetPane(0,0);
	//�Ҳ����ʾ����
	if (!m_splt2.CreateStatic(&m_splt, 2,2,WS_CHILD|WS_VISIBLE,m_splt.IdFromRowCol(0, 1)))
	{
		AfxMessageBox("���ڷָ�ʧ�ܣ�");
		return FALSE;
	}
	m_splt2.CreateView(0, 0, RUNTIME_CLASS(CRBDcmView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pRBView = (CRBDcmView*)m_splt2.GetPane(0,0);
	m_splt2.CreateView(0, 1, RUNTIME_CLASS(CSegView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pSegView = (CSegView*)m_splt2.GetPane(0,1);
	m_splt2.CreateView(1, 0, RUNTIME_CLASS(CClassifierView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pClassier = (CClassifierView*)m_splt2.GetPane(1,0);
	m_splt2.CreateView(1, 1, RUNTIME_CLASS(CInfoView), CSize(nRcWidth /7 * 3, nRcHeight/2), pContext);
	m_pInfoView = (CInfoView*)m_splt2.GetPane(1,1);
	return TRUE;
}


//���ô��ڴ�С�䶯
//tips: ���͸������Ӵ��ڵ���Ϣ�����ڴ�·�ɣ�
// ����Ҫ�жϾ����������Ӵ����϶����Ӱ��.
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	//�ػ�����ڱ�����˫����Ϣ
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	if(pMsg->message == WM_NCLBUTTONDBLCLK && pMsg->wParam == HTCAPTION && pMsg->hwnd == hWnd)
		return TRUE;
	//�ػ�����ڱ������϶���Ϣ
	if (pMsg->message == WM_NCLBUTTONDOWN  && pMsg->wParam == HTCAPTION && pMsg->hwnd == hWnd)
		return TRUE;
	return CFrameWnd::PreTranslateMessage(pMsg);
}


/***************************************************************************************
Function:    �Զ�����Ϣ������
Input:		 void
Output:   
Description: �ڴ�������״̬����ӻ���
Return:		 void
Others:		 NULL
***************************************************************************************/
LRESULT CMainFrame::OnProgress(WPARAM, LPARAM)
{
	CRect rect;
	m_wndStatusBar.GetItemRect(3, &rect);  //��ȡҪ��ʾ������Ĵ�С
	//�ʵ���С���壬���ڱ�Ե�����ڸ�
	rect.top += 2;
	rect.bottom -= 2;
	rect.left += 2;
	rect.right -= 2;
	//��״̬������һ������ؼ�
	if (!m_slider.Create(TBS_HORZ|WS_CHILD|WS_VISIBLE, rect, &m_wndStatusBar, 123))
	{
		AfxMessageBox("���鴴��ʧ��");
		return -1;
	}
	return 0;
}


/***************************************************************************************
Function:    ����ĳһ���Ӵ�Ϊ��ǰ����Ӵ��������Ӵ�������
Input:		 void
Output:   
Description: 
Return:		 void
Others:		 NULL
***************************************************************************************/
void CMainFrame::SetMainFrm(int num)
{
	CRect rct;
	GetClientRect(&rct);
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	switch (num)
	{
	case 0:
		m_splt2.SetColumnInfo(0,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 1:
		m_splt2.SetColumnInfo(1,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 2:
		m_splt2.SetColumnInfo(0,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	case 3:
		m_splt2.SetColumnInfo(0,nRcWidth/7 * 6,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	default:
		break;
	}
	m_splt2.RecalcLayout();				//�޸��Ӵ���Ҫˢ��
}


/***************************************************************************************
Function:    ��ʾ�Ӵ��ص�ԭ��λ��
Input:		 void
Output:   
Description: �ڴ�������״̬����ӻ���
Return:		 void
Others:		 NULL
***************************************************************************************/
int CMainFrame::ResetFrm(void)
{
	CRect rct;
	GetClientRect(&rct);
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	m_splt2.SetColumnInfo(0, nRcWidth / 7 * 3, 10);
	m_splt2.SetRowInfo(0, nRcHeight / 2, 10);
	m_splt2.RecalcLayout();
	return 0;
}
