
#pragma once


class CSegmentOperat
{
public:
	CSegmentOperat(void);
	~CSegmentOperat(void);
	Mat IsodataSeg(Mat src, int n = 120);					//��ֵ�����ָ�
	Mat OstuSeg(Mat src);									//Ostu�ָ�
	Mat EntropySeg(Mat src);								//����طָ�
	Mat MoMSeg(Mat src);									//�������ַ�
	Mat GetLungArea(Mat src);								//ȡ�÷�ʵ��
	Mat RegionGrow(Mat src, CPoint pt,int th = 5);			//����������ȱʡ��ֵΪ5
	Mat GetObjectCenter(Mat src,CPoint &pt1,CPoint &pt2);	//��ȡ��Ƭ��ʵ�ʵ�����
};

