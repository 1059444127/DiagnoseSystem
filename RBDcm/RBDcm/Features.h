#pragma once

class CFeatures
{
public:
	CFeatures(void);
	~CFeatures(void);
	void SetMatImgAndContours(cv::Mat src, std::vector<std::vector<cv::Point> > &contours)
	{
		m_SrcImg = src.clone();
		m_contour = &contours;
	}


private:
	cv:: Mat m_SrcImg;						//ԭͼ��
	cv::Rect m_rect;						//
	std::vector<std::vector< cv::Point > >* m_contour;	//����
	double m_fGrayMean;						//�ҶȾ�ֵ
	double m_fVariance;						//�Ҷȷ���
	double m_fArea;							//���
	double m_fCircumference;				//�ܳ�
	double m_fDiameter;						//ֱ��
	double m_fSameCircle;					//Բ�ƶ�
	double m_fFlaten;						//���
	double m_fCompactedness;				//������
	double m_fConcavity;					//������
	double m_fSlightness;					//ϸ����
	double m_fSameRect;						//���ζ�
	Point m_ptPosition;						//λ��

private:
	double CalcuGrayMean(); 
public:
	void CalculateFeatures();
};

