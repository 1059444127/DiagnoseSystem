/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  MainFrm.h
  Author: Robin   Version: V1.0   Date: 2015.11.05 
  Description: application framework class 
               MFC structure
  Others:         
***********************************************************************************/

#pragma once

#include "RBDcmView.h"
#include "OprtView.h"
#include "SegView.h"
#include "ClassifierView.h"
#include "InfoView.h"
#include "MySplitterWndEx.h"
#include "SplitterWndExt.h"

const UINT UM_PROSSESS = WM_USER + 1;
class COprtView;
//class CSplitterWndExt;

class CMainFrame : public CFrameWnd
{
public:
	CSplitterWndExt		m_splt;				//���ָ���
	CMySplitterWndEx	m_splt2;			//�Ҳ�ָ���
	CSliderCtrl			m_slider;			//״̬��������
	COprtView*			m_pOpt;				//������崰��
	CRBDcmView*			m_pRBView;			//����ͼ����ʾ����
	CSegView*			m_pSegView;			//����ͼ����ʾ����
	CClassifierView*	m_pClassier;		//����ͼ����ʾ����
	CInfoView*			m_pInfoView;		//����ͼ����ʾ����
	BOOL                m_isLeftPaneMin;    //�������Ƿ�����
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// ����
public:

// ����
public:
	CStatusBar* GetStatusBar(){return &m_wndStatusBar;};

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:				// �ؼ���Ƕ���Ա
	CStatusBar   m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnProgress(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetMainFrm(int num);
	int ResetFrm(void);
};

