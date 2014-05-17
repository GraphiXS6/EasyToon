// PlayDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "EasyToon.h"
#include "MainFrm.h"
#include "EasyToonDoc.h"
#include "EasyToonView.h"
#include "PlayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayDlg ダイアログ


CPlayDlg::CPlayDlg(CWnd* pParent, CEasyToonDoc* pDoc, int nZoom, int nStart, int nEnd )
	: CDialog(CPlayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_pDoc = pDoc;
	m_nZoom = nZoom;
  m_nStart = nStart;
  m_nEnd = nEnd;
  
}


void CPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayDlg)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS2, m_progress);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress);
}


BEGIN_MESSAGE_MAP(CPlayDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayDlg メッセージ ハンドラ


void CPlayDlg::DrawCanvas() 
{
	CStatic*	pCanvas;
	CDC*		pdcCanvas;

	// キャンバスのＤＣ取得
	pCanvas	  = (CStatic*) GetDlgItem ( IDC_CANVAS );
	pdcCanvas = pCanvas->GetDC();
	// Ｂｌｔ
	pdcCanvas->StretchBlt ( 1,1,m_nZoom * m_pDoc->GetFrameSize().cx, m_nZoom * m_pDoc->GetFrameSize().cy,m_pDoc->GetFrameDC(m_nFrame),0,0,m_pDoc->GetFrameSize().cx, m_pDoc->GetFrameSize().cy,SRCCOPY);
	// 事後処理
	pCanvas->ReleaseDC ( pdcCanvas );
}

void CPlayDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	DrawCanvas();
}

void CPlayDlg::OnTimer(UINT nIDEvent) 
{
	if ( GetTickCount() - m_dwStart > m_dwDelay*10 )
	{
    if( m_nStart < m_nEnd ) {
  		m_nFrame++;
		m_progress.StepIt();
      if( m_nFrame > m_nEnd )
	  {
        m_nFrame = m_nStart;
		m_progress.SetPos(m_nStart);
	  }
    } else if( m_nEnd < m_nStart ) {
      m_nFrame--;
      if( m_nFrame < m_nEnd )
	  {
        m_nFrame = m_nStart;
	  m_progress.SetPos(m_nStart);
	  }
    }
 
//		if ( m_nFrame >= m_pDoc->GetCount() ) m_nFrame = 0;
		if ( (m_dwDelay = m_pDoc->GetFrameDelay (m_nFrame)) == 0 ) m_dwDelay = m_pDoc->GetDefaultDelay();
		m_dwStart = GetTickCount();
		DrawCanvas();
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CPlayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nFrame = m_nStart;
	m_nTimerID = SetTimer ( 1, 1, NULL );
	if ( (m_dwDelay = m_pDoc->GetFrameDelay (m_nFrame)) == 0 ) m_dwDelay = m_pDoc->GetDefaultDelay();
	m_dwStart = GetTickCount();

	int scr_w = GetSystemMetrics(SM_CXSCREEN);
	int scr_h = GetSystemMetrics(SM_CYSCREEN);
	int canvas_w = m_nZoom * m_pDoc->GetFrameSize().cx + 2;
	int canvas_h = m_nZoom * m_pDoc->GetFrameSize().cy + 2;
	int dlg_w = canvas_w + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	int dlg_h = canvas_h + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) + 60;
	int dlg_x = (scr_w - dlg_w) / 2;
	int dlg_y = (scr_h - dlg_h) / 2;
	int btn_x = (canvas_w - 92) / 2;
	int btn_y =  canvas_h + 8;

	WINDOWPLACEMENT	wp;
	memset ( &wp, 0, sizeof(WINDOWPLACEMENT) );
	wp.length = sizeof(WINDOWPLACEMENT);
	wp.showCmd = SW_SHOWNA;
	wp.rcNormalPosition = CRect ( 0, 0, canvas_w, canvas_h );
	GetDlgItem ( IDC_CANVAS ) -> SetWindowPlacement ( &wp );
	wp.rcNormalPosition = CRect ( dlg_x, dlg_y, dlg_x + dlg_w, dlg_y + dlg_h );
	SetWindowPlacement ( &wp );

	wp.rcNormalPosition = CRect ( 0, btn_y, canvas_w , btn_y + 14 );
	GetDlgItem ( IDC_PROGRESS2 ) -> SetWindowPlacement ( &wp );

	wp.rcNormalPosition = CRect ( btn_x, btn_y+20, btn_x + 92 , btn_y + 44 );
	GetDlgItem ( ID_CLOSE ) -> SetWindowPlacement ( &wp );

	m_progress.SetRange(m_nStart,m_nEnd);
    m_progress.SetStep(1); 
	return TRUE;
}

void CPlayDlg::OnClose() 
{
	KillTimer ( m_nTimerID );
	EndDialog ( 0 );
}
