// ViewOptionDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "EasyToon.h"
#include "ViewOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewOptionDlg ダイアログ


CViewOptionDlg::CViewOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewOptionDlg)
	m_nZoomMagnify = 0;
	//}}AFX_DATA_INIT
}


void CViewOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewOptionDlg)
	DDX_Text(pDX, IDC_EDIT_ZOOM, m_nZoomMagnify);
	DDV_MinMaxInt(pDX, m_nZoomMagnify, 1, 10);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CViewOptionDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOptionDlg メッセージ ハンドラ
