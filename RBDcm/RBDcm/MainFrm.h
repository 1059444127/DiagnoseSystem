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
#include "AllImgView.h"
#include "MatImg.h"
#include "SrcShowDlg.h"
#include "SegmentOperat.h"
#include "EnhanceOperat.h"
#include "OneImgFeatures.h"


const UINT UM_PROSSESS = WM_USER + 1;
class COprtView;
//class CSplitterWndExt;

class CMainFrame : public CFrameWnd
{
public:
	CImgSerial  m_ImgSerial;                //Mat-image structure
	CPlaySeriesDlg* m_PlayDlg;
public:
	CMySplitterWndEx	m_splt;				//���ָ���
	CMySplitterWndEx	m_splt2;			//�Ҳ�ָ���
	CSliderCtrl			m_slider;			//״̬��������
	COprtView*			m_pOpt;				//������崰��
	CRBDcmView*			m_pRBView;			//����ͼ����ʾ����
	CSegView*			m_pSegView;			//����ͼ����ʾ����
	CClassifierView*	m_pClassier;		//����ͼ����ʾ����
	CInfoView*			m_pInfoView;		//����ͼ����ʾ����
	BOOL                m_isLeftPaneMin;    //�������Ƿ�����
	CToolBar            m_toolBar;			//������
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
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnBtnReset();
	afx_msg void OnBtnZoom();
	afx_msg void OnBtnSrcimg();
	afx_msg void OnBtnPlayimgs();
	afx_msg void OnBtnNextimg();
	afx_msg void OnBtnLoadxml();
	afx_msg void OnBtnLastimg();
	afx_msg void OnBtnHistimg();
	afx_msg void OnBtnCamera();
	afx_msg void OnSerileOpen();
	afx_msg void OnFileOpenOne();
	afx_msg void OnBtnEntropy();
	afx_msg void OnBtnOtsu();
	afx_msg void OnBtnReallung();
//	afx_msg void OnBtnNudoul();
	afx_msg void OnBtnNudoul();
	afx_msg void OnBtnMom();
	afx_msg void OnBtnMean();
};

