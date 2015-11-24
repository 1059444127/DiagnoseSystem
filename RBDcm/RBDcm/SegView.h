#pragma once
#include "Layer.h"
#include "ImgLayer.h"
#include "ImgInfo.h"
#include "RBMermoryDC.h"
#include "SegmentOperat.h"

// CSegView ��ͼ
class CSegView : public CView
{
	DECLARE_DYNCREATE(CSegView)
public:
	sOneImg m_sOneImgShow;				//showing image
	CArray <CLayer*, CLayer*> m_ls;     //interface layers 
protected:
	CSegView();							// ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSegView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void SetImgData(sOneImg src);
	void SegRealLung(cv::Mat src);
	void GetSegRealLungs(sOneImg& seg);
};


