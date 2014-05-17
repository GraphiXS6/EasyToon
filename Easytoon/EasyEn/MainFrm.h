#if !defined(AFX_MAINFRM_H__EE57B9CD_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_MAINFRM_H__EE57B9CD_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CMainFrame : public CFrameWnd
{
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	CFont			m_fontToolBar;

	CStatic			m_staticFrameInt;
	CEdit			m_editFrameInt;
	CSpinButtonCtrl	m_spinFrameInt;

	CStatic			m_staticViewZoom;
	CEdit			m_editViewZoom;
	CSpinButtonCtrl	m_spinViewZoom;

	CStatic			m_staticPenSize;
	CEdit			m_editPenSize;
	CSpinButtonCtrl	m_spinPenSize;

	CStatic			m_staticEraseSize;
	CEdit			m_editEraseSize;
	CSpinButtonCtrl	m_spinEraseSize;

	CEdit			m_editNextFrameNb;
	CSpinButtonCtrl	m_spinNextFrameNb;

	CEdit			m_editPrevFrameNb;
	CSpinButtonCtrl	m_spinPrevFrameNb;

	CStatic			m_staticPlaySeg;
	CEdit			  m_editPlayStart;
	CSpinButtonCtrl	m_spinPlayStart;
	CEdit			m_editPlayEnd;
	CSpinButtonCtrl	m_spinPlayEnd;

  CToolBar		m_wndToolBox;
  CToolBar		m_wndPlay;
  CToolBar		m_wndSelect;
  CToolBar		m_wndMove;
  CToolBar		m_wndBrush;
 
  CBitmap		m_bmpBrush;

	int		GetFrameDelay ();
	void	SetFrameDelay ( int nDelay );
	
	int		GetPenSize ();
	void	SetPenSize ( int nSize );
	BOOL VerifyBarState(LPCTSTR lpszProfileName);
public:
	int GetEndFrame();
	int GetStartFrame();
	void SetPlayPos( int nStart, int nEnd );
	void SetPlayLimit( int nFrames );
	void SetZoomMagnify( int nZoomMagnify );
	void SetEraseSize( int nSize );
	void SetStatusText(int pos, CString texte);
	void UpdateMemoryViewer();
	void InitializeMemoryViewer();
	



	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	//HMODULE SatDll;
	//LANGID CurrentLang;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewToolbox();
	afx_msg void OnUpdateViewToolbox(CCmdUI* pCmdUI);
	afx_msg void OnHelpIndex();
	//}}AFX_MSG
	afx_msg void OnViewZoomChange();
	afx_msg void OnFrameIntChange();
	afx_msg void OnPenSizeChange();
	afx_msg void OnEraseSizeChange();
	afx_msg void OnNextFrameNbChange();
	afx_msg void OnPrevFrameNbChange();
	afx_msg void OnUpdateFrameIndicator(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFrameDefaultDelayIndicator(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFrameDelayIndicator(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFrameDimensions(CCmdUI* pCmdUI);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewNavigationBar();
	afx_msg void OnViewFillstyleBar();
	afx_msg void OnViewSelectionBar();
	afx_msg void OnViewAnimationBar();
	afx_msg void OnViewMemoryBar();
	afx_msg void OnUpdateViewAnimationBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewSelectionBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewFillstyleBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewNavigationBar(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewMemoryBar(CCmdUI *pCmdUI);
	afx_msg void OnClose();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


//{{AFX_INSERT_LOCATION}}
#endif
