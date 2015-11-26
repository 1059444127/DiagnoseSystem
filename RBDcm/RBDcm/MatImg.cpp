/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MatImg.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description: Dcm images to Mat-info images Type, iamges elements all in vector 
  Others:         
***********************************************************************************/
#include "StdAfx.h"
#include "MatImg.h"


/***************************************************************************************
Function: ���캯��
Input:    LPCTSTR lpszPathName dcmͼ���ȫ·��
Output:   NULL
Description: ����LoadDcm���
Return:   NULL
Others:   NULL
***************************************************************************************/
CImgSerial::CImgSerial(CString lpszPathName)
{
	LoadDcm(lpszPathName);
}


/***************************************************************************************
Function: ����Dcmͼ�񣬹�������ⲿ���ýӿ�
Input:    LPCTSTR lpszPathName dcmͼ���ȫ·��
Output:   �����Ƿ�ɹ�
Description: ����Dcm2MatGroup��ɣ����������ͼ����뵱ǰͼ������
Return:   BOOL
Others:   NULL
***************************************************************************************/
BOOL CImgSerial::LoadDcm(CString lpszPathName)
{
	sOneImg mg = Dcm2MatGroup(lpszPathName);	//ͼ�����ת��
	mg.pSingleNodules = NULL;					//ͼ����ص�ʱ������Ϣȫ������
	m_vImgSerial.push_back(mg);					//��һ��ͼ��������ӵ�������
	m_itr = m_vImgSerial.begin();				//ָ��������itrָ�����еĿ�ͷ
	return TRUE;
}



/***************************************************************************************
Function: Dcmͼ��ת�������״���Ķ���Matͼ��+������Ϣ��
Input:    LPCTSTR lpszPathName dcmͼ���ȫ·��
Output:   dcmת�����Mat+info�ṹ����
Description: ʹ��DCMTK�⺯����ԭDCMͼ��ת��Ϊ8bit�Ҷ�ͼ�����ʧ����
Return:   MatGroup
Others:   NULL
***************************************************************************************/
sOneImg CImgSerial::Dcm2MatGroup(CString lpszPathName)
{
	sOneImg mg;													//����һ��MatGroup��
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile((LPSTR)(LPCTSTR)lpszPathName);				//��ȡһ��dcmͼ
	if(!oc.good())	                                                //�ж��ļ��Ƿ�򿪳ɹ�
	{
		AfxMessageBox("���ļ�ʧ��, Ӧ�ó�����ֹ��");              //�ļ���ʧ�ܺ��˳�
		AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);               //�ļ����Ժ�ֱ����ֹӦ�ó���
		return mg;
	}

	DcmDataset *mDataset = fileformat.getDataset();                 //��ȡ���ݼ�
	E_TransferSyntax xfer = mDataset->getOriginalXfer();            //�����﷨
	DicomImage *dcmImg = new DicomImage(lpszPathName);              //����һ��dcmͼ��
	//��ȡͼ��߿�
	int width  = dcmImg->getWidth();
	int height = dcmImg->getHeight();
	dcmImg->setMinMaxWindow();

	const char *pStr;
	mg.info.isDisplay = TRUE;										//��ʾͼ����Ϣ
	mDataset->findAndGetString(DCM_PatientName, pStr);				//��������
	mg.info.sName = pStr;
	mDataset->findAndGetString(DCM_PatientID, pStr);				//����ID
	mg.info.sId = pStr;
	mDataset->findAndGetString(DCM_InstitutionName, pStr);
	mg.info.sInstitutionName  = pStr;								//��������
	mDataset->findAndGetString(DCM_PatientSex, pStr);
	mg.info.sSex  = pStr;											//�����Ա�
	mDataset->findAndGetString(DCM_ImagePositionPatient, pStr);
	mg.info.sImgPositionPatient = pStr;								//����ͼ��λ��
	
	int num = 0;
	CString* pPosition = CImgInfoLayer::SplitString(mg.info.sImgPositionPatient,'\\', num);
	mg.info.nPositionX = CType::CString2Double(pPosition[0]);
	mg.info.nPositionY = CType::CString2Double(pPosition[1]);
	mg.info.nPositionZ = CType::CString2Double(pPosition[2]);
	
	mDataset->findAndGetString(DCM_WindowCenter, pStr);
	mg.info.sWindowCenter = pStr;									//��λ
	mDataset->findAndGetString(DCM_WindowWidth, pStr);
	mg.info.sWindowWidth = pStr;									//����

	Uint8 *pixelData = (Uint8 *)(dcmImg->getOutputData(8));         //ת��Ϊ8λ�Ҷ�ͼ��
	Mat tmp = Mat(width, height, CV_8UC1, (BYTE *)pixelData);		//ת����Mat����ʽ
	mg.pixle  = tmp.clone();										//��¡���ṹ��
	m_index = 1;													//��ͼ���룬��λ����һ����
	delete dcmImg;
	return mg;
}


/***************************************************************************************
Function: ����������һ��ͼƬ������������ƶ�
Input:    NULL
Output:   �����������ָ�Ĳ����������һ���򷵻���һ�ţ��������һ���򷵻ص�ǰ��
Description: ע�����е�end()���������е����һ�ţ������������һ�ŵ���һ��Ԫ��
             λ�ã��ǲ��ɷ��ʵ��ڴ���
Return:   MatGroup
Others:   NULL
***************************************************************************************/
sOneImg CImgSerial::NextMatImg()
{
	if (++m_itr != m_vImgSerial.end())   
	{
		++m_index;			//ͼ�������ݼ�
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"�Ѿ������һ��ͼƬ�ˣ�","RBDcm��ʾ��",MB_OK);
		return *(--m_itr);
	}
}


/***************************************************************************************
Function: ����������һ��ͼƬ������������ƶ�
Input:    NULL
Output:   �����������ָ�Ĳ������е�һ���򷵻���һ�ţ����ǵ�һ���򷵻ص�ǰ��
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
sOneImg CImgSerial::LastMatImg()
{
	if (m_itr != m_vImgSerial.begin())   
	{
		m_itr--;
		--m_index;			//ͼ�������ݼ�
		return *m_itr;
	}
	else
	{
		MessageBox(NULL,"�Ѿ��ǵ�һ��ͼƬ�ˣ�","RBDcm��ʾ��",MB_OK);
		return *(m_itr);
	}
}



/***************************************************************************************
Function: ����Xml�ļ�
Input:    �ļ�·��+�ļ���
Output:   
Description: 
Return:   MatGroup
Others:   NULL
***************************************************************************************/
BOOL CImgSerial::LoadXml(CString lpszPathName)
{
	const char* pFile = (LPSTR)(LPCTSTR)lpszPathName;						//XML�ļ�·��
	m_vSingle.clear();														//����ǰ�������Ϣ
	TiXmlDocument doc(pFile);												//������ȡXML��ʱ����
	BOOL loadOK = doc.LoadFile();											//����XML�ļ�
	if (!loadOK)															//����ʧ�ܵ�����ʾ���˳��ú���
	{
		MessageBox(NULL, "xml�ļ���ȡʧ�ܣ�", "RBDcm��ʾ��", MB_OK);
		return FALSE;
	}

	TiXmlElement* root = doc.RootElement();									//XML�ĸ��ڵ�
	for (TiXmlNode* SpecialistItem = root->FirstChild("readingSession");    //��readingSession�ڵ����ѭ��
		SpecialistItem; SpecialistItem = SpecialistItem->NextSibling("readingSession"))
	{   
		TiXmlNode* unblindedReadNodule = SpecialistItem->FirstChild("unblindedReadNodule");		//���
		TiXmlNode* nonNodule = SpecialistItem->FirstChild("nonNodule");							//�ǽ��

		while (unblindedReadNodule)								//�ڵ�unblindedReadNoduleѭ��
		{
			TiXmlNode* roi = unblindedReadNodule->FirstChild("roi");
			while (roi)											//�ڵ�roiѭ��
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");	 //imageZposition��Ϣ
				const char* Zposition = imageZposition->ToElement()->GetText();  
				single.ZPosition = CType::pChar2Double(Zposition); 
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap"); 
				while (edgeMap)													//edgeMap�ڵ�ѭ��
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = ISNODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			unblindedReadNodule = unblindedReadNodule->NextSibling("unblindedReadNodule");
		}

		while (nonNodule)																		
		{
			TiXmlNode* roi = nonNodule->FirstChild("roi");
			while (roi)
			{
				SingleImgNodule single;
				TiXmlNode* imageZposition = roi->FirstChild("imageZposition");		//��ȡ�����µĽڵ�
				const char* Zposition = imageZposition->ToElement()->GetText();		//zPosition����ֵ			
				single.ZPosition = CType::pChar2Double(Zposition);
				TiXmlNode* edgeMap = roi->FirstChild("edgeMap");
				while (edgeMap)
				{
					TiXmlNode* xCoord = edgeMap->FirstChild("xCoord");
					TiXmlNode* yCoord = edgeMap->FirstChild("yCoord");
					const char* szX = xCoord->ToElement()->GetText();
					const char* szY = yCoord->ToElement()->GetText();
					NodulePoint pt;
					pt.nt = NONENODULE;
					pt.x = CType::pChar2int(szX);
					pt.y = CType::pChar2int(szY);
					single.vcNodulePoint.push_back(pt);
					edgeMap = edgeMap->NextSibling("edgeMap");
				}
				//��ŵ�������
				m_vSingle.push_back(single);
				roi = roi->NextSibling("roi");
			}
			nonNodule = nonNodule->NextSibling("nonNodule");
		}
	}

	//������ɵ���Ϣ��ԭ���ݽ��й���
	list<sOneImg>::iterator itr = m_vImgSerial.begin();
	int count = 1;
	for (;itr != m_vImgSerial.end(); ++count)
	{
		double dbTmp = itr->info.nPositionZ;
		list<SingleImgNodule>::iterator itrSg = m_vSingle.begin();        //xml������Ϣ
 		for (; itrSg != m_vSingle.end(); ++itrSg)						   //�ڽ����ı��в���ƥ���Ȼ����
		{
			double tmp = itrSg->ZPosition;
			if (fabs(tmp - dbTmp) > 0.1)
			{
				itr->pSingleNodules = NULL;		//û�������Ϣʱ�����ָ��
				++count;
				continue;
			}
			itr->pSingleNodules = &(*itrSg);   //���д���˽����Ϣ��ֱ�ӽ������ֲ�ѯ
			break;
		}
		++itr;
	}
	return TRUE;
}







