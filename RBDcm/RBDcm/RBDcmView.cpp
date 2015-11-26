/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  RBDcmView.cpp
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: Software Frame  View Class 
               This class contain main View and image layers manage
  Others:         
***********************************************************************************/
#include "stdafx.h"

#ifndef SHARED_HANDLERS
#include "RBDcm.h"
#endif

#include "RBDcmDoc.h"
#include "RBDcmView.h"
#include "ImgFormat.h"
#include "RBMermoryDC.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRBDcmView
IMPLEMENT_DYNCREATE(CRBDcmView, CView)

BEGIN_MESSAGE_MAP(CRBDcmView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CRBDcmView ����/����
CRBDcmView::CRBDcmView() : m_nType(0)
	, isMax(false)
{
	CLayer* pLayer = NULL;
	m_ls.RemoveAll();								//ɾ������ͼ��
	pLayer = CLayer::Create(IMG_LAYER);				//����ͼ��������ʾ��
	m_ls.Add(pLayer);
	pLayer = CLayer::Create(IMG_INFO_LAYER);		//����ͼ�������Ϣ��ʾ��
	m_ls.Add(pLayer);
}

CRBDcmView::~CRBDcmView()
{
}

BOOL CRBDcmView::PreCreateWindow(CREATESTRUCT& cs)
{
	//CREATESTRUCT cs ���޸Ĵ��������ʽ
	return CView::PreCreateWindow(cs);
}

/***************************************************************************************
Function: �������
Input:    
Output:   NULL
Description: ��ͼ���ϵ���Ϣ���Ƶ���Ļ��
             Ŀǰû��ʵ��˫���壬��ʱ������Ļ��˸������
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnDraw(CDC* pDC)
{
	CRBDcmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rc;
	GetClientRect(&rc);

	CRBMemoryDC MemDC;
	MemDC.Create(rc.Width(), rc.Height(), pDC);
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(0,0,0));
	
    //ͼ�����ͼ���Լ������н��л���
	int i = 0;
	int nSize = pDoc->m_ls.GetSize();
	//����Doc���д��ڵ�ͼ��
	while (i < nSize)
	{
		pDoc->m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	//����View�д��ڵ�ͼ��
	i = 0;
	nSize = m_ls.GetSize();
	while (i < nSize)
	{
		m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
}


// CRBDcmView ���
#ifdef _DEBUG
void CRBDcmView::AssertValid() const
{
	CView::AssertValid();
}

void CRBDcmView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRBDcmDoc* CRBDcmView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRBDcmDoc)));
	return (CRBDcmDoc*)m_pDocument;
}
#endif //_DEBUG


/***************************************************************************************
Function: ��ͼ�����°�����������Ϣ��Ӧ
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRBDcmDoc* pDoc = GetDocument();    //�ļ�ָ��
	if (pDoc->m_ls.GetSize() == 0)		//ͼ���������򷵻�
		return ;
	CImgDataLayer* pImgLayer = (CImgDataLayer*)(pDoc->m_ls[0]);
	CSize imgsize = pImgLayer->GetImgSize();
	CRect SrcRect;
	GetClientRect(SrcRect);
	CRect ImgRect;
	ImgRect.top = (SrcRect.Height() - imgsize.cy) / 2;
	ImgRect.left = (SrcRect.Width() - imgsize.cx) / 2;
	ImgRect.right = ImgRect.left + imgsize.cx;
	ImgRect.bottom = ImgRect.top + imgsize.cy;
	CView::OnLButtonDown(nFlags, point);
}

/***************************************************************************************
Function: ��ͼ������̧����������Ϣ��Ӧ
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	CView::OnLButtonUp(nFlags, point);
}


/***************************************************************************************
Function: ��ͼ��������������Ϣ
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
BOOL CRBDcmView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_ls.GetSize() == 0)								//ͼ���������򷵻�
		return FALSE;
	CImgDataLayer* pImgLayer = (CImgDataLayer*)(m_ls[0]);	//ͼ�����ݲ�
	static int count = 0;									//��¼�����������˲�
	static double action = 0.0;								//
	if (zDelta > 0)											//���������� �Ŵ�ͼ��
	{
		action += 1.1;										//
		//����ͼ��Ŵ�ʱͼ��
		HCURSOR hCursor =AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMIN);
		::SetCursor(hCursor);
	}
	else                                                    //������ڹ��� ��Сͼ��
	{
		action += 0.9;
		//����ͼ����Сʱͼ��
		HCURSOR hCursor =AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMOUT);
		::SetCursor(hCursor);
	}
	count++;
	if (count >= 3)											//�˲�ʹ��
	{
		pImgLayer->SetTimes(action / count);
		count = 0;
		action = 0;
		Invalidate(FALSE);
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


/***************************************************************************************
Function: ���»��ƽ���
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::ReDraw(void)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CRBDcmView *pRDBView = (CRBDcmView *)pMain->GetActiveView();
	CDC *pDC = pRDBView->GetDC();
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 100);
	ReleaseDC(pDC);
}


/***************************************************************************************
Function: ��������С����Ӧ
Input:    
Output:   NULL
Description: NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		((CMainFrame*)AfxGetMainWnd())->m_pOpt->OnBnClickedBtnLast();
		break;
	case VK_DOWN:
		((CMainFrame*)AfxGetMainWnd())->m_pOpt->OnBnClickedBtnNext();
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


/***************************************************************************************
Function: View���ʼ������
Input:    void
Output:   NULL
Description: �ڴ˻�ȡ����������ָ�� XX
rewriter:    ��ֱ���ڴ˻�ȡ���ָ�룬�Ӵ��Ļ�ȡ����ȫ����Frame������ͳһ���й���
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}


/***************************************************************************************
Function: ��ͼ������ʱ��Ϣ��Ӧ
Input:    void
Output:   NULL
Description: �����ڶ��ϴ����Ķ���
             ���ҵ���Doc������ٺ�����Doc���ж��ϴ����Ķ����������
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnDestroy()
{
	CView::OnDestroy();
	CRBDcmDoc *pDoc = GetDocument();
}


/***************************************************************************************
Function: ���˫����Ϣ��Ӧ
Input:    �������˫�������Ϣ
Output:   NULL
Description: ����ڴ�����˫���������Ҫ���ô�������Ϊ��ǰ�Ӵ��ڵ������ʾ
Return:   void
Others:   NULL
***************************************************************************************/
void CRBDcmView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!isMax)				//�������ʱ�Ŵ���������ʱ��С��ԭ����С
	{
		((CMainFrame*)AfxGetMainWnd())->SetMainFrm(0);
		isMax = true;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->ResetFrm();
		isMax = false;
	}
	CView::OnLButtonDblClk(nFlags, point);
}


void CRBDcmView::SetImgData(sOneImg src)
{
	m_sOneImgShow.pixle = src.pixle.clone();
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


void CRBDcmView::OnRButtonDown(UINT nFlags, CPoint point)
{
	/*CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	CMenu* pMenu = menu.GetSubMenu(0);
	ClientToScreen(&point);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	CView::OnRButtonDown(nFlags, point);*/
}

//�Ҽ��˵�
void CRBDcmView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}
