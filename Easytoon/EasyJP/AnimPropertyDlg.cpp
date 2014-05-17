// AnimPropertyDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "EasyToon.h"
#include "AnimPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimPropertyDlg ダイアログ


CAnimPropertyDlg::CAnimPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnimPropertyDlg)
	m_nDefaultDelay = 10;
	//}}AFX_DATA_INIT
}


void CAnimPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimPropertyDlg)
	DDX_Text(pDX, IDC_EDIT_DEFAULTDELAY, m_nDefaultDelay);
	DDV_MinMaxInt(pDX, m_nDefaultDelay, 1, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimPropertyDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimPropertyDlg メッセージ ハンドラ
