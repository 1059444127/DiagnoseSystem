#pragma once

//���������ص�������Ϣ
class CContourFeatures
{
public:
	CContourFeatures(void);					
	~CContourFeatures(void);
public:
	double m_fGrayMean;						//�ҶȾ�ֵ
	double m_fVariance;						//�Ҷȷ���
	double m_fArea;							//���
	double m_fCircumference;				//�ܳ�
	float m_fDiameter;						//ֱ��
	double m_fSameCircle;					//Բ�ƶ�
	double m_fFlaten;						//���
	double m_fCompactedness;				//������
	double m_fConcavity;					//������
	double m_fSameRect;						//���ζ�
	cv::Point2f m_ptPosition;				//λ��
};

