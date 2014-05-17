#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "MainFrm.h"
#include "EasytoonDoc.h"
#include "EasyToonView.h"


// CMemoryViewer dialog

#define DEFTHUMBX 75
#define DEFTHUMBY 75


class CMemoryViewer : public CDialogBar
{
	DECLARE_DYNAMIC(CMemoryViewer)

public:
	CMemoryViewer(CWnd* pParent = NULL);   // standard constructor
	afx_msg void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL Create(CWnd * pParentWnd, UINT nIDTemplate,UINT nStyle, UINT nID);
	virtual BOOL Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName,UINT nStyle, UINT nID);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	virtual BOOL OnInitDialogBar();
	void setMemoryList(CObList *imgList);
	void setParentDocument(CEasyToonView* pView);
	virtual void Update();
	virtual ~CMemoryViewer();


// Dialog Data
	enum { IDD = IDD_MEMORYVIEWER };
	
private :

	void CalculateThumbnailsSize();
	void CalculateDefaultBarSize();

protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitDialogBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	
	DECLARE_MESSAGE_MAP()
private:
	CImageList* _imgLst;
	CObList* _objList; 
	CListCtrl _lvMemory;
	CEasyToonView* pView;
	CButton btnAddMemory;
	CButton btnDeleteMemory;
	CButton btnRestoreMemory;
	int selectedItem;
	CSize _thumbSize;
	CSize _memViewWndSize;
	CSize _memDefaultViewSize;
public:
	afx_msg void OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedBtnaddmemory();
	afx_msg void OnBnClickedBtnrestorememory();
	afx_msg void OnBnClickedBtndeletememory();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHdnFilterChangeList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult);
};
