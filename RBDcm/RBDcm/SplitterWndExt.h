#pragma once
#include "afxext.h"

//CSplitterWnd���������࣬�����ػ������Ϣ�������������϶��ָ���
class CSplitterWndExt :
	public CSplitterWnd
{
public:
	CSplitterWndExt(void);
	~CSplitterWndExt(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};



