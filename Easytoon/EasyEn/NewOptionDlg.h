#include "afxwin.h"
#if !defined(AFX_NEWOPTIONDLG_H__EE57B9DA_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_NEWOPTIONDLG_H__EE57B9DA_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewOptionDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CNewOptionDlg ダイアログ

class CNewOptionDlg : public CDialog
{
// コンストラクション
public:
	CNewOptionDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CNewOptionDlg)
	enum { IDD = IDD_NEWOPTION };
	int		m_nFrameHeight;
	int		m_nFrameWidth;
	int _iSelectedRdb;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CNewOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNewOptionDlg)
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	 virtual void OnPredefinedDimensionClickedRadio(UINT nID);
	 CEdit m_newWidth;
	 CEdit m_newHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NEWOPTIONDLG_H__EE57B9DA_A60C_11D1_AB14_444553540000__INCLUDED_)
