#if !defined(AFX_VIEWOPTIONDLG_H__EE57B9DB_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_VIEWOPTIONDLG_H__EE57B9DB_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ViewOptionDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CViewOptionDlg ダイアログ

class CViewOptionDlg : public CDialog
{
// コンストラクション
public:
	CViewOptionDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CViewOptionDlg)
	enum { IDD = IDD_VIEWOPTION };
	int		m_nZoomMagnify;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CViewOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CViewOptionDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VIEWOPTIONDLG_H__EE57B9DB_A60C_11D1_AB14_444553540000__INCLUDED_)
