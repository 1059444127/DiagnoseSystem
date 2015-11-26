/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MatImg.h
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: Dcm images to Mat-info images Type, iamges elements all in vector 
  Others:         
***********************************************************************************/
#pragma once

//dcmtkͷ�ļ�
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"

#include "xml/tinyxml.h"							//XML����
#include "ImgInfo.h"
#include "Type.h"




class CImgSerial
{
public:
	CImgSerial(void) : m_index(0)					//��Ĭ�Ϲ��캯��
	{
		m_vImgSerial.clear();						//��������
		m_itr = m_vImgSerial.begin();				//ָ��������ָʾλ��
	};

	~CImgSerial(void)								//��Ĭ����������
	{
		m_vImgSerial.clear();						//��������
	};

	CImgSerial(CString lpszPathName);				//���ع��캯��
	sOneImg Dcm2MatGroup(CString lpszPathName);		//���ع��캯��
	void Clear();									//����m_vMatGroup�ڵ�����
	sOneImg GetCurrentMatImg();						//��ȡ��ǰ��������ָͼ������
	list<sOneImg>::iterator GetBegin();				//ͼ��������ͼ�������λ��
	sOneImg NextMatImg();							//���������ƣ���ȡ��һ��ͼ��
	sOneImg LastMatImg();							//������ǰ�ƣ���ȡ��һ��ͼ��
	int GetImageNum();								//��ȡ��ǰ����ͼ������
	int GetCurrentNum();							//��ȡ��ǰ��������ָͼ���������е����
	BOOL LoadDcm(CString lpszPathName);				//����dcmͼ��
	BOOL LoadXml(CString lpszPathName);				//����xml�ļ�

private:
	list<sOneImg> m_vImgSerial;						//ͼ����������
	list<SingleImgNodule> m_vSingle;				//��Ž����������
	list<sOneImg>::iterator m_itr;					//ָ��ͼ���������еĵ�����
	int m_index;									//���������������е�������
};


/***************************************************************************************
Function: ��ȡ��ǰ��������ָ�洢ͼ�����е�������
Input:    NULL
Output:   ������
Description: 
Return:   
Others:   NULL
***************************************************************************************/
inline int CImgSerial::GetCurrentNum(void)
{
	return m_index;
}


/***************************************************************************************
Function: ��ȡ��ǰ�洢ͼ�����е�ͼ����Ŀ
Input:    NULL
Output:   ��Ŀ
Description: 
Return:   
Others:   NULL
***************************************************************************************/
inline int CImgSerial::GetImageNum(void)
{
	return m_vImgSerial.size();
}


/***************************************************************************************
Function: �����ǰ�洢����������
Input:    NULL
Output:   �����������ָ�Ĳ������е�һ���򷵻���һ�ţ����ǵ�һ���򷵻ص�ǰ��
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
inline void CImgSerial::Clear(void)
{
	m_vImgSerial.clear();
}


/***************************************************************************************
Function: ��ȡ��������ǰ��ָͼ������
Input:    NULL
Output:   ��������ǰ��ָ���ݵĿ���
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
inline sOneImg CImgSerial::GetCurrentMatImg(void)
{
	return (*m_itr);
}


/***************************************************************************************
Function: ��ȡ��ǰ�洢ͼ�����е���Ԫ�ص�����
Input:    NULL
Output:   ����������
Description: 
Return:   
Others:   NULL
***************************************************************************************/
inline list<sOneImg>::iterator CImgSerial::GetBegin()
{
	list<sOneImg>::iterator ir = m_vImgSerial.begin();
	return ir;
}




