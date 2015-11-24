#pragma once
#include "MatImg.h"
#include "Type.h"

// CInfoView ��ͼ
class CInfoView : public CListView
{
	DECLARE_DYNCREATE(CInfoView)
public:
	sOneImg img;

protected:
	CInfoView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CInfoView();

public:
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
	void ShowData(void);
	void DeleteAll(void);
};


