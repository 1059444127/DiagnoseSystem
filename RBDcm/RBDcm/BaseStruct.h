/***************************************************************************************
  Copyright @ Robin 2015-2025
  File name:  BaseStruct.h
  Author: Robin   Version: V1.0   Date:2015.09.05 
  Description:  Commom data structure definition 
                Contain OpenCV header files 
  Others:      This File would be contiand in stdafx.h file.
               In order to simplify other files not to re-contain this file.
***************************************************************************************/
#ifndef __BASESTRCUT__
#define __BASESTRCUT__

//OpenCV header files 
#include <opencv2/highgui/highgui.hpp>
#include <cv.hpp>
#include <opencv2/opencv.hpp>
#include <core/core.hpp>

//STL header files
#include <list>
using namespace cv;				    //opencv namespace 
using namespace std;


//ͼ������Ͷ��� m_nType
#define IMG_LAYER             0    //ͼ�����ݲ����ͺ�
#define IMG_INFO_LAYER        1    //ͼ����Ϣ�����ͺ�


//��һ��dcmͼ����ص���Ϣ
struct sOneImgInfo 
{
	CString sId;					//Patient ID
	CString sName;					//Patient Name 
	CString sInstitutionName;		//Insitiyution Name
	CString sSex;					//Patient Sex
	CString sAge;					//Patient Age
	CString sWindowCenter;          //Window Center
	CString sWindowWidth;           //Window Width
	CString sImgPositionPatient;    //Image Position Patient
	double nPositionX;				//top-left position of X
	double nPositionY;				//top-left position of Y
	double nPositionZ;				//Key infomation about images
	bool isDisplay;					//is Displayed in the screen flag
	//������� = ��������
	sOneImgInfo& operator= (sOneImgInfo& vl)
	{
		sId = vl.sId;
		sName = vl.sName;
		sInstitutionName = vl.sInstitutionName;
		sSex = vl.sSex;
		sAge = vl.sAge;
		sWindowCenter = vl.sWindowCenter;
		sWindowWidth = vl.sWindowWidth;
		sImgPositionPatient = vl.sImgPositionPatient;
		nPositionX = vl.nPositionX;
		nPositionY = vl.nPositionY;
		nPositionZ = vl.nPositionZ;
		isDisplay = vl.isDisplay;
		return *this;
	}
};


/**************************************************************************************
 �������
 1��NONENODULE �ǽ��
 2��ISNODULE   ���
***************************************************************************************/
enum NoduleType
{
	NONENODULE=0,
	ISNODULE=1
};


/**************************************************************************************
 xml�н�ڱ�ע�����Ϣ
 1����ڵ��λ��
 2����ڵ�ı�ע�ͱ�
***************************************************************************************/
struct NodulePoint
{
	int x;					
	int y;
	NoduleType nt;
};


/**************************************************************************************
����ͼ���н�ڵ���Ϣ
1������ͼ��ͼ�������е�����
2������ͼ���z������
3������ͼ���н�ڵ�ļ���
***************************************************************************************/
struct SingleImgNodule
{
	int index;
	list<NodulePoint> vcNodulePoint;
	double ZPosition;
};


/***************************************************************************************
ÿ��ͼ���Ӧ����Ϣ�ṹ��
1���������ݣ���dcm�ļ��л�ȡ��
2��ͼ��ĸ�����Ϣ��eg: PatientName ect.. ����dcm�ļ��л�ȡ��
3��ר�ұ�ע���ĸ�ͼ���а����Ľ�ڡ�δ���Խ���Լ��ǲ����λ�� (��Ӧ�ļ�����xml�ļ��л�ȡ)
***************************************************************************************/
struct sOneImg
{
	Mat pixle;									//ͼ�����������
	sOneImgInfo info;							//ͼ��ĸ�����Ϣ
	//����ͼ����λ����Ϣ,ָ�������һ�����е�
	SingleImgNodule* pSingleNodules;			
};



#endif



