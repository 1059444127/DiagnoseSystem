#include "StdAfx.h"
#include "OneImgFeatures.h"


COneImgFeatures::COneImgFeatures(void)
{
}


COneImgFeatures::~COneImgFeatures(void)
{
}


/***************************************************************************************
Function: ����һ����ͨ��ĻҶȾ�ֵ
Input:    cv::Rect rect ���������ͨ����С�������
Output:   ����õ��ĻҶȾ�ֵ
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
double COneImgFeatures::CalcuGrayMean(cv::Rect rect, double& Area)
{
	double fgary = 0.0;											//�Ҷ�
	int count = 0;												//��¼��0�Ҷ�ֵ�ĵ�ĸ���
	for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//�������������ڵ�ÿһ�����ص�	
	{
		for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
		{
			if (m_SrcImg.at<uchar>(nY, nX) == 0)				//���ص�ĻҶ�ֵΪ0�����������
				continue;

			fgary += m_SrcImg.at<uchar>(nY, nX);				//���ص�ĻҶ�ֵ����0�����������
			++count;
		}
	}
	Area = count;												//������ͨ�����
	return fgary/count;											//���ػҶȾ�ֵ
}


/***************************************************************************************
Function: ����һ����ͨ��ĻҶȷ���
Input:    cv::Rect rect ���������ͨ����С�������
		  double mean ��ͨ��ĻҶȾ�ֵ
Output:   �õ�����ͨ��Ҷȷ���
Description: 
Return:   void
Others:   NULL
***************************************************************************************/
double COneImgFeatures::CalcuVariance(cv::Rect rect, double mean)
{
	double fVariance = 0.0;										//�Ҷȷ���
	int count = 0;												//��¼��0�Ҷ�ֵ�ĵ�ĸ���
	for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//�������������ڵ�ÿһ�����ص�	
	{
		for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
		{
			uchar tmp = m_SrcImg.at<uchar>(nY, nX);				//ȡ�����ص�Ҷ�ֵ
			if (tmp == 0)										//���ص�ĻҶ�ֵΪ0�����������
				continue;
			fVariance += (mean - tmp) * (mean - tmp);		
			++count;
		}
	}
	return fVariance;											//���ػҶȷ���
}



/***************************************************************************************
Function: ����һ��ͼ���е�������ͨ�������
Input:    void
Output:   
Description:  ���������������ͨ�򣬼�����ͨ�������
Return:   void
Others:   NULL
***************************************************************************************/
void COneImgFeatures::CalculateFeatures()
{
	Mat drawing = Mat::zeros(m_SrcImg.size(), CV_8UC3 );				//����һ����ͨ����RGB��ɫMatͼ��
	cv::Rect rect;														//��ͨ��������
	cv::Mat fe(m_contour->size(), 12, CV_32F, Scalar(0));				//��������
	for( int i = 0; i< m_contour->size(); i++ )							//������ͨ������
	{
		CContourFeatures features;												//һ����ͨ���������Ϣ
		rect = boundingRect(m_contour->at(i));									//��ͨ�����С�������
		features.m_fGrayMean = CalcuGrayMean(rect, features.m_fArea);			//�ҶȾ�ֵ �������
		features.m_fVariance = CalcuVariance(rect, features.m_fGrayMean);		//�Ҷȷ���
		features.m_fCircumference = cv::arcLength(m_contour->at(i),true);		//�ܳ�
		//���ĵ㣬�뾶���������С���Բ��Ϣ������ó�
		cv::minEnclosingCircle(m_contour->at(i), features.m_ptPosition, features.m_fDiameter);		
		features.m_fDiameter *= 2.0;
		//Բ�ƶ�
		features.m_fSameCircle = 4*features.m_fArea/features.m_fDiameter/features.m_fCircumference;  
		//���
		cv::RotatedRect rorect = cv::minAreaRect(m_contour->at(i));
		if (rorect.size.height > rorect.size.width)
			features.m_fFlaten = rorect.size.width / rorect.size.height;
		else
			features.m_fFlaten = rorect.size.height / rorect.size.width;
		//������
		features.m_fCompactedness = 4*3.1415*features.m_fArea / features.m_fCircumference;  
		//������
		features.m_fConcavity = (rorect.size.height * rorect.size.width - features.m_fArea) / features.m_fArea;
		//���ζ�
		features.m_fSameRect = features.m_fArea / (rorect.size.height * rorect.size.width);

		//д����������
		fe.at<float>(i, 0) = features.m_fGrayMean;			//��ֵ
		fe.at<float>(i, 1) = features.m_fVariance;			//����
		fe.at<float>(i, 2) = features.m_fArea;				//���
		fe.at<float>(i, 3) = features.m_fCircumference;		//�ܳ�
		fe.at<float>(i, 4) = features.m_fDiameter;			//ֱ��
		fe.at<float>(i, 5) = features.m_fSameCircle;		//Բ�ƶ�
		fe.at<float>(i, 6) = features.m_fFlaten;			//���
		fe.at<float>(i, 7) = features.m_fCompactedness;		//������
		fe.at<float>(i, 8) = features.m_fConcavity;			//������
		fe.at<float>(i, 9) = features.m_fSameRect;			//���ƶ�
		fe.at<float>(i, 10) = features.m_ptPosition.x;		//X��λ��
		fe.at<float>(i, 11) = features.m_ptPosition.y;		//Y��λ��
		
		//fe.at<CV_32F>() = features.m_fGrayMean;
		//m_features.push_back(features);
		for (int nX=rect.x; nX<(rect.x+rect.width); ++nX)			//
		{
			for (int nY=rect.y; nY<(rect.y+rect.height); ++nY)
			{
				if (m_SrcImg.at<uchar>(nY,nX) == 0)
					continue;
				//if ((features.m_ptPosition.x - nX < 1) && (features.m_ptPosition.y - nY < 1))
				//{
				//	drawing.at<cv::Vec3b>(nY,nX)[0] = 0;
				//	drawing.at<cv::Vec3b>(nY,nX)[1] = 255;
				//	drawing.at<cv::Vec3b>(nY,nX)[2] = 255;
				//}
				//else
				{
					/*CvFont font;
					cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,1.0,1.0,0,2,8);*/
					char tt[10];
					sprintf_s(tt,"%d",i);
					string abc(tt); 
					drawing.at<cv::Vec3b>(nY, nX)[0] = 0;
					drawing.at<cv::Vec3b>(nY, nX)[1] = 0;
					drawing.at<cv::Vec3b>(nY, nX)[2] = 255;
					putText(drawing, abc, Point(features.m_ptPosition.x, features.m_ptPosition.y),
					CV_FONT_HERSHEY_PLAIN, 0.7, Scalar(0,255,255), 1);
				}
			}
		}
	}

	//д��xml�ļ�
	/*
	FileStorage fs("F:\\TrainingData.xml", FileStorage::WRITE);
	fs << "TrainingData" << fe;
	fs.release();
	*/
	ofstream file("test.csv");
	file << format(fe,"csv");
	file.close();
	cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
	cv::imshow("test",drawing);
}