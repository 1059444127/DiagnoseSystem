// ClassifierView.cpp : ʵ���ļ�
#include "stdafx.h"
#include "RBDcm.h"
#include "ClassifierView.h"


// CClassifierView
IMPLEMENT_DYNCREATE(CClassifierView, CView)


/***************************************************************************************
Function: CClassifierView���캯��
Input:    NULL
Output:   BOOL
Description: ��ͼ�๹��ʱ��������ͼ��
Return:   NULL
Others:   NULL
***************************************************************************************/
CClassifierView::CClassifierView()
{
	CLayer* pLayer = NULL;						//Create layers
	m_ls.RemoveAll();							//delete all layers
	pLayer = CLayer::Create(IMG_LAYER);			//create images layer
	m_ls.Add(pLayer);							//add to the layer array
	pLayer = CLayer::Create(IMG_INFO_LAYER);	//create info layer 
	m_ls.Add(pLayer);
}


CClassifierView::~CClassifierView()
{
}


BEGIN_MESSAGE_MAP(CClassifierView, CView)
END_MESSAGE_MAP()


/***************************************************************************************
Function: CClassifierView��OnDraw��Ϣ��Ӧ
Input:    NULL
Output:   pDC:����DC 
Description: �����Լ���ͼ����л��ƣ�ʹ��˫���壩
			ͼ����������ͼ�๹��ʱ��������ͼ��
Return:   NULL
Others:   NULL
***************************************************************************************/
void CClassifierView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	CRect rc;
	GetClientRect(&rc);
	CRBMemoryDC MemDC;
	MemDC.Create(rc.Width(), rc.Height(), pDC);
	MemDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(0,0,0));
	int i = 0;
	int nSize = m_ls.GetSize();
	while (i < nSize)
	{
		m_ls[i]->OnDraw(&MemDC, rc);
		i++;
	}
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
}


// CClassifierView ���
#ifdef _DEBUG
void CClassifierView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CClassifierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


/***************************************************************************************
Function: Ϊ����������ͼ������
Input:    sOneImg һ��ͼ��������Ϣ
Output:   void 
Description: ����ͼ�����ݺ�ˢ�½��棬��ʾͼ����Ϣ
Return:   NULL
Others:   NULL
***************************************************************************************/
void CClassifierView::SetImgData(sOneImg src)
{
	m_sOneImgShow.pixle = src.pixle.clone();
	m_sOneImgShow.info = src.info;
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


/***************************************************************************************
Function: ��ڷָ����Ӧ����
Input:    sOneImg һ��ͼ��������Ϣ
Output:   void 
Description: ����ͼ�����ݺ�ˢ�½��棬��ʾͼ����Ϣ
Return:   NULL
Others:   NULL
***************************************************************************************/
void CClassifierView::SegNodules(sOneImg src)
{
	CSegmentOperat seg;									//����һ���ָ����
	m_sOneImgShow.info = src.info;						//��������
	cv::Mat dst = seg.MoMSeg(src.pixle.clone());		//���÷ָ��ຯ����ͼ����зָ�
	m_sOneImgShow.pixle = dst.clone();					//�ָ�ṹ�������
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);		//����ͼ����Ϣ�����ҽ���ˢ����ʾ
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}





/***************************************************************************************
Function: ��ڷָ����Ӧ����
Input:    sOneImg һ��ͼ��������Ϣ
Output:   void 
Description: ����ͼ�����ݺ�ˢ�½��棬��ʾͼ����Ϣ
Return:   NULL
Others:   NULL
***************************************************************************************/
void CClassifierView::SegNodulesMean(sOneImg src)
{
	CSegmentOperat seg;									//����һ���ָ����
	m_sOneImgShow.info = src.info;						//��������
	cv::Mat dst = seg.IsodataSeg(src.pixle.clone());	//���÷ָ��ຯ����ͼ����зָ�
	
	sOneImg srcimg = ((CMainFrame*)AfxGetMainWnd())->m_pRBView->GetOneImg();
	
	bitwise_and(dst, srcimg.pixle, dst);
	//cv::subtract(dst, srcimg.pixle, dst);
	
	m_sOneImgShow.pixle = dst.clone();					//�ָ�ṹ�������
	
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);		//����ͼ����Ϣ�����ҽ���ˢ����ʾ
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}


void CClassifierView::HenssisenEnhance(sOneImg src)
{
	m_sOneImgShow.info = src.info;						//��������
	CEnhanceOperat enhance;
	cv::Mat dst = enhance.HessianEnhance(m_sOneImgShow.pixle.clone(),2,10);	//����һ���ָ����
	m_sOneImgShow.pixle = dst.clone();					//�ָ�ṹ�������
	m_ls[0]->SetLayerImgData(m_sOneImgShow.pixle);		//����ͼ����Ϣ�����ҽ���ˢ����ʾ
	m_ls[1]->SetLayerInfoData(m_sOneImgShow.info);
	Invalidate(FALSE);
}

sOneImg CClassifierView::GetOneImg()
{
	return m_sOneImgShow;
}

