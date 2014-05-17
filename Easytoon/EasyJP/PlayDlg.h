#include "afxcmn.h"
#if !defined(AFX_PLAYDLG_H__EE57B9DD_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_PLAYDLG_H__EE57B9DD_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PlayDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPlayDlg ダイアログ

class CPlayDlg : public CDialog
{
protected:
	CEasyToonDoc*	m_pDoc;			// ドキュメントへのポインタ
	int				m_nZoom;		// キャンバスのズーム比率
	int				m_nTimerID;		// タイマＩＤ
	DWORD			m_dwDelay;		// 遅延時間
	DWORD			m_dwStart;		// 待ち始め
	int				m_nFrame;		// フレーム番号
	CProgressCtrl m_progress;
	void DrawCanvas();

  int m_nStart, m_nEnd;

// コンストラクション
public:
	CPlayDlg(CWnd* pParent, CEasyToonDoc* pDoc, int nZoom, int nStart, int nEnd );   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPlayDlg)
	enum { IDD = IDD_PLAY };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPlayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPlayDlg)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PLAYDLG_H__EE57B9DD_A60C_11D1_AB14_444553540000__INCLUDED_)
