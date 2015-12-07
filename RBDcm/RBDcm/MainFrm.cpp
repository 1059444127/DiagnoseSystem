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
	ON_COMMAND(ID_BTN_RESET, &CMainFrame::OnBtnReset)
	ON_COMMAND(ID_BTN_ZOOM, &CMainFrame::OnBtnZoom)
	ON_COMMAND(ID_BTN_SRCIMG, &CMainFrame::OnBtnSrcimg)
	ON_COMMAND(ID_BTN_PLAYIMGS, &CMainFrame::OnBtnPlayimgs)
	ON_COMMAND(ID_BTN_NEXTIMG, &CMainFrame::OnBtnNextimg)
	ON_COMMAND(ID_BTN_LOADXML, &CMainFrame::OnBtnLoadxml)
	ON_COMMAND(ID_BTN_LASTIMG, &CMainFrame::OnBtnLastimg)
	ON_COMMAND(ID_BTN_HISTIMG, &CMainFrame::OnBtnHistimg)
	ON_COMMAND(ID_BTN_CAMERA, &CMainFrame::OnBtnCamera)
	ON_COMMAND(ID_SERILE_OPEN, &CMainFrame::OnSerileOpen)
	ON_COMMAND(ID_FILE_OPEN_ONE, &CMainFrame::OnFileOpenOne)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,				// ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����
CMainFrame::CMainFrame(): m_isLeftPaneMin(FALSE)
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
	//����״̬��
	if (!m_wndStatusBar.Create(this))						
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_CAPS, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_NUM, SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_SCRL, SBPS_NORMAL, 100);
	//����������
	if (!m_toolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_LEFT
		|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_toolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		return -1;
	}
	m_toolBar.EnableDocking(CBRS_ALIGN_ANY);		//����������ͣ��
	EnableDocking(CBRS_ALIGN_ANY);					//����֧��ͣ��
	DockControlBar(&m_toolBar);						//ͣ��������

	PostMessage(UM_PROSSESS);						//������Ϣ����״̬���ϴ���������
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
	int nRcWidth = rct.Width();
	int nRcHeight = rct.Height();
	nRcHeight -= 60;							//���⣬״̬��Ԥ��

	//�Ҳ����ʾ����
	if (!m_splt2.CreateStatic(&m_splt, 2, 2, WS_CHILD|WS_VISIBLE, m_splt.IdFromRowCol(0, 0)))
	{
		AfxMessageBox("���ڷָ�ʧ�ܣ�");
		return FALSE;
	}
	nRcWidth = (nRcWidth-200)/2;
	nRcHeight = nRcHeight/2;
	m_splt2.CreateView(0, 0, RUNTIME_CLASS(CRBDcmView), CSize(nRcWidth, nRcHeight), pContext);
	m_pRBView = (CRBDcmView*)m_splt2.GetPane(0,0);
	m_splt2.CreateView(0, 1, RUNTIME_CLASS(CSegView), CSize(nRcWidth, nRcHeight), pContext);
	m_pSegView = (CSegView*)m_splt2.GetPane(0,1);
	m_splt2.CreateView(1, 0, RUNTIME_CLASS(CClassifierView), CSize(nRcWidth, nRcHeight), pContext);
	m_pClassier = (CClassifierView*)m_splt2.GetPane(1,0);
	m_splt2.CreateView(1, 1, RUNTIME_CLASS(CInfoView), CSize(nRcWidth, nRcHeight), pContext);
	m_pInfoView = (CInfoView*)m_splt2.GetPane(1,1);
	
	m_splt.CreateView(0, 1, RUNTIME_CLASS(COprtView), CSize(200, nRcHeight), pContext);
	m_pOpt = (COprtView*)m_splt.GetPane(0,1);
	
	m_splt.SetColumnInfo(0, rct.Width()-210, 500);
	m_splt.SetColumnInfo(1,  200, 100);
	
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
	if (m_isLeftPaneMin)
		nRcWidth = nRcWidth;
	else
		nRcWidth = (nRcWidth-200);
	switch (num)
	{
	case 0:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 1:
		m_splt2.SetColumnInfo(1,nRcWidth,10);
		m_splt2.SetRowInfo(0,nRcHeight,10);
		break;
	case 2:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	case 3:
		m_splt2.SetColumnInfo(0,nRcWidth,10);
		m_splt2.SetRowInfo(1,nRcHeight,10);
		break;
	default:
		break;
	}
	m_splt2.RecalcLayout();				//�޸��Ӵ���Ҫˢ��
	m_pRBView->Invalidate(FALSE);
	m_pSegView->Invalidate(FALSE);
	m_pClassier->Invalidate(FALSE);
	m_pInfoView->Invalidate(FALSE);
}


/***************************************************************************************
Function:    ��ʾ�Ӵ��ص�ԭ��λ��
Input:		 void
Output:   
Description: 
Return:		 void
Others:		 NULL
***************************************************************************************/
int CMainFrame::ResetFrm()
{
	CRect rct;
	GetClientRect(&rct);						//��ȡ�ͻ����������С
	int nRcWidth = rct.Width();			
	int nRcHeight = rct.Height();
	nRcHeight -= 60;							//ȥ�����⣬�˵���״̬��
	if (m_isLeftPaneMin == TRUE)				//����ʾ������		
		nRcWidth = nRcWidth/2;					//
	else										//��ʾ������
		nRcWidth = (nRcWidth-200)/2;			//
	m_splt2.SetColumnInfo(0, nRcWidth, 10);		//
	m_splt2.SetRowInfo(0, nRcHeight / 2, 10);	//
	m_splt2.RecalcLayout();
	m_pRBView->Invalidate(FALSE);
	m_pSegView->Invalidate(FALSE);
	m_pClassier->Invalidate(FALSE);
	m_pInfoView->Invalidate(FALSE);
	return 0;
}


BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnBtnReset()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnBtnZoom()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnBtnSrcimg()
{
	CSrcShowDlg dlg;
	dlg.SetImgData(m_ImgSerial.GetCurrentMatImg().pixle.clone());
	dlg.DoModal();
	dlg.CenterWindow();
}


void CMainFrame::OnBtnPlayimgs()
{
	if (m_PlayDlg != NULL)			//�������Ѿ������ڴ��У�����ʾ�Ϳ����ˣ����ⴴ���������
	{
		m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
		m_PlayDlg->ShowWindow(SW_NORMAL);
		return;
	}
	m_PlayDlg = new CPlaySeriesDlg();
	CRBDcmView *pView = ((CMainFrame*)AfxGetMainWnd())->m_pRBView;
	m_PlayDlg->Create(MAKEINTRESOURCE(IDD_PLAY_SERI), pView);
	m_PlayDlg->SetImgSerial(m_ImgSerial.GetBegin(), m_ImgSerial.GetImageNum());
	m_PlayDlg->ShowWindow(SW_NORMAL);
}


void CMainFrame::OnBtnNextimg()
{
	sOneImg img = m_ImgSerial.NextMatImg();
	m_pRBView->SetImgData(img);
	m_pInfoView->SetImgData(img);
	//SetCurrent(m_ImgSerial.GetCurrentNum());
}

void CMainFrame::OnBtnLoadxml()
{
	CFileDialog dlg(TRUE);								//�ļ��򿪶Ի���
	dlg.m_ofn.lpstrTitle = "��Xml�ļ�";				//ָ�����ļ��Ի�������
	//�ļ�������
	dlg.m_ofn.lpstrFilter = "xml Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0\0";  
	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	m_ImgSerial.LoadXml(strFileName);					//����xml�ļ�
	sOneImg info = m_ImgSerial.GetCurrentMatImg();
	m_pInfoView->SetImgData(info);						//����Ϣ��ʾ���ڴ����ڵ���Ϣ
}


void CMainFrame::OnBtnLastimg()
{
	sOneImg img = m_ImgSerial.LastMatImg();
	m_pRBView->SetImgData(img);
	m_pInfoView->SetImgData(img);
	//SetCurrent(m_ImgSerial.GetCurrentNum());
}


void CMainFrame::OnBtnHistimg()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnBtnCamera()
{
	// TODO: �ڴ���������������
}





void CMainFrame::OnSerileOpen()
{
	AfxMessageBox("��ͼ������");
	CFileDialog dlg(TRUE);								//�ļ��򿪶Ի���
	CFileFind ff;										//�ļ�������
	CStringArray dcmPathstr;							//�ļ�·��+�ļ����б� dcm�ļ�
	dlg.m_ofn.lpstrTitle = "��ͼ������";				//ָ�����ļ��Ի�������
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";  //�ļ�������

	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;

	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	int end = strFileName.ReverseFind('\\');			//�������
	CString strFilePath = strFileName.Left(end);		//ȥ�������ļ����õ��ļ�Ŀ¼

	if(!ff.FindFile(strFilePath + "\\*.dcm"))			//����Ŀ¼���Ƿ����dcm�ļ������������˳�
		return;

	while(ff.FindNextFile())							//����dcm�ļ��������ļ�����·������dcmPathstr
		dcmPathstr.Add(strFilePath + "\\" + ff.GetFileName());

	m_ImgSerial.Clear();								//��������ǰ�����
	CString strXml = NULL;								//�ļ�·��
	for (int i=0; i<dcmPathstr.GetSize(); ++i)		
	{
		strFileName = dcmPathstr.GetAt(i);				//��������ȡ���ļ�·��
		m_ImgSerial.LoadDcm(strFileName);				//����Dcm�ļ�
	}
	//SetSum(m_ImgSerial.GetImageNum());					//��ʾ��ǰͼ������
	//SetCurrent(m_ImgSerial.GetCurrentNum());			//��ʾ��ǰͼ�����
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//ȡ�õ�ǰͼ��
	m_pRBView->SetImgData(info);			//��ͼ�����õ�CRBDcm����
}


void CMainFrame::OnFileOpenOne()
{
	CFileDialog dlg(TRUE);								//�ļ��򿪶Ի���
	dlg.m_ofn.lpstrTitle = "�򿪵���ͼ��";				//ָ�����ļ��Ի�������
	//�ļ�������
	dlg.m_ofn.lpstrFilter = "Dcm Files(*.dcm)\0*.dcm\0All Files(*.*)\0*.*\0\0";		
	if (IDCANCEL == dlg.DoModal())						//�����ļ��򿪶Ի���ѡ��ȡ����ֱ�ӷ���
		return;
	CString strFileName = dlg.GetPathName();			//��ȡ�ļ�·��+�ļ���
	m_ImgSerial.Clear();								//���ԭ������
	m_ImgSerial.LoadDcm(strFileName);					//��������
	//SetSum(m_ImgSerial.GetImageNum());				//��ʾ��ǰͼ������
	//SetCurrent(m_ImgSerial.GetCurrentNum());			//��ʾ��ǰͼ�����
	sOneImg info = m_ImgSerial.GetCurrentMatImg();		//�����е�ǰͼ��
	m_pRBView->SetImgData(info);			//���õ�ǰͼ��
}
