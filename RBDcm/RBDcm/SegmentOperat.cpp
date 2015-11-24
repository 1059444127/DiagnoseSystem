#include "StdAfx.h"
#include "SegmentOperat.h"

using namespace cv;    //for mat operator

CSegmentOperat::CSegmentOperat(void)
{
}


CSegmentOperat::~CSegmentOperat(void)
{
}


/***************************************************************************************
Function: ��ֵ��ֵ�����ָ��㷨
Input:    Mat ���ָ��ԭͼ��  n��ʼ��ֵ
Output:   �ָ��ͼ��
Description: 
Return:   Mat
Others:   NULL
***************************************************************************************/
Mat CSegmentOperat::IsodataSeg(Mat src, int n)
{
	int threshold = 0;							//��ʷ��ֵ
	int MeansO = 0;								//ǰ���ҶȾ�ֵ
	int nObject = 0;							//ʵ�����ص����
	int MeansB = 0;								//�����ҶȾ�ֵ
	int nBack = 0;								//�������ص����
	int nCol = src.cols * src.channels();		//ÿ�е����ظ���
	Mat dst;									//�ָ��ͼ��
	while (abs(threshold - n) > 10)				//����ֹͣ����
	{
		threshold = n;
		for (int i=0; i<src.rows; ++i)
		{
			uchar* pData = src.ptr<uchar>(i);
			for (int j=0; j<nCol; ++j)
			{
				//CTͼ����Ч��ɫ������࣬ȥ����������أ�����Լ����������
				if (pData[j] == 0)			//ȥ����ɫ�ĵ���������
				{
					++j;
					continue;
				}
					
				if (pData[j] < threshold)   //����
				{
					++nBack;
					MeansB += pData[j];
				}
				else						//����
				{
					++nObject;
					MeansO += pData[j];
				}
			}
		}
		n = (MeansB/nBack + MeansO/nObject) / 2;
	}
	cv::threshold(src, dst, n, 255, 0);    //������ֵ�ָ�
	return dst.clone();
}


/***************************************************************************************
Function: ��ֵ��ֵ�����ָ��㷨
Input:    Mat ���ָ��ԭͼ��  n��ʼ��ֵ
Output:   �ָ��ͼ��
Description: 
Return:   Mat
Others:   NULL
***************************************************************************************/
Mat CSegmentOperat::OstuSeg(Mat src)
{
	int tbHist[256] = {0};						//ֱ��ͼ����
	double average = 0.0;						//ƽ������ֵ
	double cov = 0.0;							//����
	double maxcov = 0.0;						//�������ֵ
	int index = 0;								//�ָ�����ֵ
	Mat dst;
	int nCol = src.cols * src.channels();		//ÿ�е����ظ���
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			if (pData[j] == 0)					//�ų�����ɫ�����ص�
			{
				++j;
				continue;
			}
			tbHist[pData[j]] += 1;
		}
	}

	int sum = 0;
	for (int i=1; i<256; ++i)					//�ų���ɫ���ص�
		sum += tbHist[i];

	double w0=0.0, w1=0.0, u0=0.0, u1=0.0;
	int count0 = 0;
	for (int i = 0; i < 255; ++i)
	{
		u0 = 0;
		count0 = 0;
		for (int j=0; j<=i; ++j)
		{
			u0 += j*tbHist[j];
			count0 += tbHist[j];
		}
		u0 = u0/count0;
		w0 = (float)count0/sum;

		u1 = 0;
		for (int j=i+1; j<256; ++j)
			u1 += j*tbHist[j];

		u1 = u1/(sum - count0);
		w1 = 1 - w0;
		cov = w0*w1*(u1-u0)*(u1-u0);
		if (cov > maxcov)
		{
			maxcov = cov;
			index = i;
		}
	}
	cv::threshold(src, dst, index, 255, 0);    //������ֵ�ָ�
	return dst.clone();
}


/***************************************************************************************
Function: ����طָ��㷨
Input:    Mat ���ָ��ԭͼ��
Output:   �ָ��ͼ��
Description: 
Return:   Mat
Others:   NULL
***************************************************************************************/
Mat CSegmentOperat::EntropySeg(Mat src)
{
	int tbHist[256] = {0};					//ÿ������ֵ����
	int index = 0;							//����ض�Ӧ�ĻҶ�
	double Property = 0.0;					//������ռ����
	double maxEntropy = -1.0;				//�����
	double frontEntropy = 0.0;				//ǰ����
	double backEntropy = 0.0;				//������
	int TotalPixel = 0;						//����������������
	int nCol = src.cols * src.channels();	//ÿ�е����ظ���
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			if (pData[j] == 0)				//�ų�����ɫ�����ص�
			{
				++j;
				continue;
			}
			++TotalPixel;
			tbHist[pData[j]] += 1;
		}
	}

	for (int i = 0; i < 256; i++) 
	{
		//���㱳��������
		double backTotal = 0;
		for (int j = 0; j < i; j++)
		{
			backTotal += tbHist[j];
		}

		//������
		for (int j = 0; j < i; j++)
		{
			if (tbHist[j] != 0)
			{
				Property = tbHist[j] / backTotal;
				backEntropy += -Property * logf((float)Property);
			}
		}
		//ǰ����
		for (int k = i; k < 256; k++)
		{
			if (tbHist[k] != 0)
			{
				Property = tbHist[k] / (TotalPixel - backTotal);
				frontEntropy += -Property * logf((float)Property);
			}
		}

		if (frontEntropy + backEntropy > maxEntropy)	//�õ������
		{
			maxEntropy = frontEntropy + backEntropy;
			index = i;
		}
		//��ձ��μ�����ֵ
		frontEntropy = 0.0;
		backEntropy = 0.0;
	}
	Mat dst;
	cv::threshold(src, dst, index, 255, 0);    //������ֵ�ָ�
	return dst.clone();
}


/***************************************************************************************
Function:  �������ַ�
Input:     Mat ���ָ��ԭͼ��
Output:    �ָ��ͼ��
Description: 
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::MoMSeg(Mat src)
{
	int tbHist[256] = {0};					//ÿ������ֵ����
	Mat dst;
	double moment1 = 0.0;					//ͼ���һ�׾�
	double P1 = 0.0;						//P1
	double tmpP = 0.0;
	int TotalPixel = 0;						//����������������
	int index = 0;
	int nCol = src.cols * src.channels();	//ÿ�е����ظ���
	for (int i = 0; i < src.rows; i++)
	{
		uchar* pData = src.ptr<uchar>(i);
		for (int j = 0; j < nCol; ++j)
		{
			if (pData[j] == 0)				//�ų�����ɫ�����ص�
			{
				++j;
				continue;
			}
			++TotalPixel;
			tbHist[pData[j]] += 1;			//����ÿ�����صĸ���
		}
	}

	//����ͼ���һ�׾�
	for (int i=0; i< 256; ++i)
	{
		moment1 += i * tbHist[i];
	}
	moment1 = moment1 / TotalPixel;
	P1 = (255 - moment1) / 255;

	int sum = 0;
	double div = 500.0;							//��ֵ
	for (int i=0; i<256; ++i)
	{
		sum = 0;
		for (int j=0; j<i; ++j)
		{
			sum += tbHist[j];
		}
		tmpP = (double)sum / TotalPixel;
		
		if (abs(tmpP - P1) < div)
		{
			div = abs(tmpP - P1);
			index  = i;
		}
	}
	index += 10;
	cv::threshold(src, dst, index, 255, 0);    //������ֵ�ָ�
	return dst.clone();
}


/***************************************************************************************
Function:  ��ȡ��ʵ�����ڵ����� 
Input:     Mat���� �Ѿ�����ֵ����ͼ��
Output:    ��ʵ�ʵ����ڵ����� ʵ�����ǰ�ɫ�����������Ǻ�ɫ
Description: 
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::GetLungArea(Mat src)
{
	Mat  et= getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));
	morphologyEx( src, src, MORPH_OPEN, et);

	Mat tmp = src.clone();
	Mat dst(src.size(), CV_8U, cv::Scalar(255));
	std::vector<std::vector<cv::Point>> contours;    //�߽�㼯��
	std::vector<cv::Vec4i> hierarchy;				 //�߽�
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//�ҵ��������
	std::vector<std::vector<cv::Point>>::const_iterator itc = contours.begin();
	UINT nSize = itc->size();
	int cId = 0;
	int nMaxId = 0;

	//����Ѱ�����ĵı߽磬���ı߽���Ƿβ�ͼ��
	while (itc != contours.end())
	{
		if (itc->size() > nSize)
		{
			nSize = itc->size();
			nMaxId = cId;
		}
		++itc;
		++cId;
	}

	//�������������result�������Ľ�����ڲ�Ϊ��ɫ���ⲿΪ��ɫ
	drawContours(dst, contours, nMaxId, Scalar(0), CV_FILLED, 1, hierarchy);
	bitwise_not(dst, dst);
	
	cv::Mat ele(9, 9, CV_8U, cv::Scalar(1));    //9X9�ṹԪ��
	erode(dst, dst, ele);						//��ͼ����и�ʴ,ȥ��ϸС�ı߽�
	cv::subtract(dst, tmp, dst);
	
	//ʹ��Բ��ģ����б����㣬��ϸС��ȱ�ڽ����޲������򷨣�
	Mat element = getStructuringElement(MORPH_ELLIPSE,cv::Size(7,7));
	morphologyEx( dst, dst, MORPH_CLOSE, element);
	
	//�ٴ���ͼ��߽磬��ͼ���ڲ��������
	findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	drawContours(dst, contours, -1, Scalar(255), CV_FILLED, 1, hierarchy);
	
	element = getStructuringElement(MORPH_ELLIPSE,cv::Size(5,5));    //ʹ��5*5ԭ��ģ����и�ʴ
	erode(dst, dst, element);
	return dst.clone();
}


/***************************************************************************************
Function:  ���������㷨
Input:     src ������ԭͼ�� pt ��ʼ������ th ��������ֵ����
Output:    ��ʵ�ʵ����ڵ����� ʵ�����ǰ�ɫ�����������Ǻ�ɫ
Description: �������������Ϊ��ɫ(255),����ɫΪ��ɫ(0)
Return:    Mat
Others:    NULL
***************************************************************************************/
Mat CSegmentOperat::RegionGrow(Mat src, CPoint pt, int th)
{
	CPoint ptGrowing;								//��������λ��
	int nGrowLable = 0;								//����Ƿ�������
	int nSrcValue = 0;								//�������Ҷ�ֵ
	int nCurValue = 0;								//��ǰ������Ҷ�ֵ
	Mat matDst = Mat::zeros(src.size(), CV_8UC1);	//����һ���հ��������Ϊ��ɫ
	//��������˳������
	int DIR[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};  
	Vector<CPoint> vcGrowPt;						//������ջ
	vcGrowPt.push_back(pt);							//��������ѹ��ջ��
	matDst.at<uchar>(pt.x, pt.y) = 255;				//���������
	nSrcValue = src.at<uchar>(pt.x, pt.y);			//��¼������ĻҶ�ֵ
	
	while (!vcGrowPt.empty())						//����ջ��Ϊ��������
	{
		pt = vcGrowPt.back();						//ȡ��һ��������
		vcGrowPt.pop_back();						

		//�ֱ�԰˸������ϵĵ��������
		for (int i = 0; i<9; ++i)
		{
			ptGrowing.x = pt.x + DIR[i][0];		
			ptGrowing.y = pt.y + DIR[i][1]; 
			//����Ƿ��Ǳ�Ե��
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x > (src.cols-1) || (ptGrowing.y > src.rows -1))
				continue;

			nGrowLable = matDst.at<uchar>(ptGrowing.x, ptGrowing.y);			//��ǰ��������ĻҶ�ֵ

			if (nGrowLable == 0)												//�����ǵ㻹û�б�����
			{
				nCurValue = src.at<uchar>(ptGrowing.x, ptGrowing.y);			
				if (abs(nSrcValue - nCurValue) < th)							//����ֵ��Χ��������
				{
					matDst.at<uchar>(ptGrowing.x, ptGrowing.y) = 255;			//���Ϊ��ɫ
					vcGrowPt.push_back(ptGrowing);								//����һ��������ѹ��ջ��
				}
			}
		}
	}
	return matDst.clone();
}

