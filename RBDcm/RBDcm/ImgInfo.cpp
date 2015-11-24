/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgInfo.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  Displayed layers about data besides image pixles 
				dicom image's relative data such as: Patient name .. 
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "ImgInfo.h"


CImgInfoLayer::CImgInfoLayer(void)
{
	//memset(&m_info, 0, sizeof(m_info));
	isEmpty = TRUE;
}


CImgInfoLayer::~CImgInfoLayer(void)
{
}


/***************************************************************************************
Function: ����ͼ��������ݲ�����
Input:    ͼ��������ݽṹ��
Output:  void
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::SetLayerInfoData(sOneImgInfo info)
{
	m_info = info;
	isEmpty = FALSE;
}

/***************************************************************************************
Function: ��ȡ��ǰͼ��������ݲ�����
Input:    NULL
Output:  ͼ��������ݽṹ��
Description:  NULL
Return:   void
Others:   NULL
***************************************************************************************/
sOneImgInfo CImgInfoLayer::GetLayerInfoData()
{
	return m_info;
}


/***************************************************************************************
Function: ͼ��������ݻ���
Input:    CDC* ���Ƶ�Ŀ��DC  CRect ����DC���������
Output:  void
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::OnDraw(CDC* pDC, CRect rect)
{
	if(!m_info.isDisplay)
		return;
	CRect rc = rect;
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	ft.CreateFont(14, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����");  //��������
	pDC->SelectObject(ft);
	if (isEmpty)				//ͼ�����ݲ�Ϊ��ʱ��д����
		return;
	//д������Ϣ
	CRect rc1(0, 0, 300, 300);												//���Ͻ�
	CRect rc2(rc.Width() - 303, 0, rc.Width() - 3, 300);					//���Ͻ�
	CRect rc3(0, rc.Height() - 15*4, 300, rc.Height());						//���½�
	CRect rc4(rc.Width()-303, rc.Height()- 15*2, rc.Width(), rc.Height());  //���½�

	CString txt1 = "Institution Name: " + m_info.sInstitutionName + "\n" +
		           "Patient Name: " + m_info.sName + "\n" +
				   "Patient Sex: " + m_info.sSex + "\n" +
				   "Patient ID: " + m_info.sId + "\n" +
				   "Patient Age: " + m_info.sAge;

	CString txt2 = "Window Width: " + m_info.sWindowWidth + "\n" +
		           "Window Center: " + m_info.sWindowCenter;

	CString txt3 = "ImagePositionPatient \nRL: " + CType::double2CString(m_info.nPositionX) 
		           + "\nAP: " + CType::double2CString(m_info.nPositionY)  
				   + "\nHF: " + CType::double2CString(m_info.nPositionZ);

	CString txt4 = "\nCopyright@2015 Robin in UESTC...";
	pDC->DrawText(txt1, &rc1, DT_LEFT);
	pDC->DrawText(txt2, &rc2, DT_RIGHT);
	pDC->DrawText(txt3, &rc3, DT_LEFT);
	pDC->DrawText(txt4, &rc4, DT_RIGHT);
}


/***************************************************************************************
Function: ������������Ϣ��Ӧ
Input:    �����λ�ü������Ϣ
Output:   NULL
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::OnLButtonDown(UINT nFlags, CPoint point)
{

}


/***************************************************************************************
Function: ������̧����Ϣ��Ӧ
Input:    ���̧��ʱλ��
Output:   NULL
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
void CImgInfoLayer::OnLButtonUp(UINT nFlags, CPoint point)
{

}


/***************************************************************************************
Function: �ַ����ָ��
Input:    ������ָ���ַ��� �ָ��ַ� �ָ�õ��Ľṹ��Ŀ
Output:   �ָ�õ����ַ�������
Description:  
Return:   void
Others:   NULL
***************************************************************************************/
CString* CImgInfoLayer::SplitString(CString str, char split, int& iSubStrs)
{
	int iPos = 0;				//�ָ��λ��
	int iNums = 0;				//�ָ��������
	CString strTemp = str;
	CString strRight;
	//�ȼ������ַ���������
	while (iPos != -1)
	{
		iPos = strTemp.Find(split);
		if (iPos == -1)
		{
			break;
		}
		strRight = strTemp.Mid(iPos + 1, str.GetLength());
		strTemp = strRight;
		iNums++;
	}
	if (iNums == 0)				//û���ҵ��ָ��
	{
		//���ַ����������ַ�������
		iSubStrs = 1; 
		return NULL;
	}
	//���ַ�������
	iSubStrs = iNums + 1; //�Ӵ������� = �ָ������ + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//���Ӵ�
		strLeft = strTemp.Left(iPos);
		//���Ӵ�
		strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
		strTemp = strRight;
		pStrSplit[i] = strLeft;
	}
	pStrSplit[iNums] = strTemp;
	return pStrSplit;
}


