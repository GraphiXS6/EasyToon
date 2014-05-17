#include "stdafx.h"
#include "EasyToon.h"
#include "MainFrm.h"
#include "EasyToonDoc.h"
#include "EasyToonView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TOOLBOX, OnViewToolbox)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBOX, OnUpdateViewToolbox)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_VIEW_ZOOMEDIT, OnViewZoomChange)
	ON_EN_CHANGE(IDC_FRAMEINTEDIT, OnFrameIntChange)
	ON_EN_CHANGE(IDC_PENSIZEEDIT, OnPenSizeChange)
	ON_EN_CHANGE(IDC_ERASESIZEEDIT, OnEraseSizeChange)

	ON_UPDATE_COMMAND_UI(ID_INDICATOR_FRAME, OnUpdateFrameIndicator)
	ON_COMMAND(ID_VIEW_NAVIGATION_BAR, OnViewNavigationBar)
	ON_COMMAND(ID_VIEW_FILLSTYLE_BAR, OnViewFillstyleBar)
	ON_COMMAND(ID_VIEW_SELECTION_BAR, OnViewSelectionBar)
	ON_COMMAND(ID_VIEW_ANIMATION_BAR, OnViewAnimationBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANIMATION_BAR, OnUpdateViewAnimationBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTION_BAR, OnUpdateViewSelectionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILLSTYLE_BAR, OnUpdateViewFillstyleBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAVIGATION_BAR, OnUpdateViewNavigationBar)
	//ON_COMMAND(ID_LANG_FRENCH, OnIdLangFrench)
	//ON_COMMAND(ID_LANG_ENGLISH, OnIdLangEnglish)
	//ON_COMMAND(ID_LANG_JAPANESE, OnIdLangJapanese)
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_FRAME
};

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFrameWnd::PreCreateWindow(cs);
}

//--------------------------------------------------------------------------
//	OnCreate
//--------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	//CurrentLang=DetectLanguage();
	//SatDll=LoadSatelliteDLL(CurrentLang);

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;
	
	//-------- ÉXÉeÅ[É^ÉXÉoÅ[
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	//-------- ÉcÅ[ÉãÉoÅ[ÅïÉcÅ[ÉãÉ{ÉbÉNÉXÇÃçÏê¨
#ifdef _EASYMODE
  // èâêSé“ÉÇÅ[Éh
	if ( !m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME) ||
		 !m_wndToolBox.Create(this) || !m_wndToolBox.LoadToolBar(IDR_TOOLBOX_EASY)	||
     !m_wndPlay.Create(this) || !m_wndPlay.LoadToolBar(IDR_TB_PLAY) || 
     !m_wndSelect.Create(this) || !m_wndSelect.LoadToolBar(IDR_TB_SELECT) ||
     !m_wndMove.Create(this) || !m_wndMove.LoadToolBar(IDR_TB_MOVE) ||
     !m_wndBrush.Create(this) || !m_wndBrush.LoadToolBar(IDR_TB_BRUSH))//||
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
#else
	if ( !m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME) ||
		 !m_wndToolBox.Create(this) || !m_wndToolBox.LoadToolBar(IDR_TOOLBOX)	||
     !m_wndPlay.Create(this) || !m_wndPlay.LoadToolBar(IDR_TB_PLAY) || 
     !m_wndSelect.Create(this) || !m_wndSelect.LoadToolBar(IDR_TB_SELECT) ||
     !m_wndMove.Create(this) || !m_wndMove.LoadToolBar(IDR_TB_MOVE) ||
     !m_wndBrush.Create(this) || !m_wndBrush.LoadToolBar(IDR_TB_BRUSH))// ||
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
#endif

  m_wndToolBar.SetBarStyle( m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBox.SetBarStyle( m_wndToolBox.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndPlay.SetBarStyle( m_wndToolBox.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndSelect.SetBarStyle( m_wndToolBox.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndMove.SetBarStyle( m_wndToolBox.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
	m_wndBrush.SetBarStyle( m_wndToolBox.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC  );

  m_wndBrush.SetWindowText("ìhÇËí◊ÇµÉpÉ^Å[Éì");
  m_wndMove.SetWindowText("ÉtÉåÅ[ÉÄà⁄ìÆ");
  m_wndSelect.SetWindowText("îÕàÕéwíË");
  m_wndToolBox.SetWindowText("¬∞Ÿ");
  m_wndToolBar.SetWindowText("¬∞Ÿ  ﬁ∞");
  m_wndPlay.SetWindowText("çƒê∂");

  m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
  m_wndToolBox.EnableDocking( CBRS_ALIGN_ANY);
  
  m_wndPlay.EnableDocking( CBRS_ALIGN_ANY );
  m_wndSelect.EnableDocking( CBRS_ALIGN_ANY );
  m_wndMove.EnableDocking( CBRS_ALIGN_ANY);
  m_wndBrush.EnableDocking( CBRS_ALIGN_ANY );
  
  EnableDocking( CBRS_ALIGN_ANY );
  CRect Posrect;
  DockControlBar(&m_wndToolBar);

 
  
  
  DockControlBar( &m_wndToolBox);
  RecalcLayout(TRUE);
  DockControlBar(&m_wndSelect,AFX_IDW_DOCKBAR_RIGHT);
  RecalcLayout(TRUE);

  
  DockControlBar( &m_wndBrush,AFX_IDW_DOCKBAR_BOTTOM);
  RecalcLayout(TRUE);
  m_wndBrush.GetWindowRect(Posrect);
  Posrect.OffsetRect(450, 0);

  DockControlBar( &m_wndMove,AFX_IDW_DOCKBAR_BOTTOM,Posrect);
  RecalcLayout(TRUE);
  //DockControlBar ( &m_mabarre );
  m_wndMove.GetWindowRect(Posrect);
  Posrect.OffsetRect(700, 0);
  DockControlBar(&m_wndPlay,AFX_IDW_DOCKBAR_BOTTOM,Posrect );
  RecalcLayout(TRUE);
  //-------- ÉcÅ[ÉãÉoÅ[Ç…ÉtÉåÅ[ÉÄÉCÉìÉ^Å[ÉoÉãÅEÉXÉsÉìÇí«â¡
	// É{É^ÉìÇÉZÉpÉåÅ[É^Ç…ïœçX
	int nIndex = m_wndToolBar.CommandToIndex(IDC_FRAMEINTCAP);
	m_wndToolBar.SetButtonInfo ( nIndex,     IDC_FRAMEINTCAP,  TBBS_SEPARATOR, 52);
	m_wndToolBar.SetButtonInfo ( nIndex + 1, IDC_FRAMEINTEDIT, TBBS_SEPARATOR, 48);
	// ÉRÉìÉgÉçÅ[ÉãÇÃçÏê¨ÅiÉLÉÉÉvÉVÉáÉìÅj
	CRect rect;
	m_wndToolBar.GetItemRect ( nIndex, &rect );
	rect.top += 4;
	m_staticFrameInt.Create ( "≤›¿∞ ﬁŸ", WS_VISIBLE, rect, &m_wndToolBar, IDC_FRAMEINTCAP );
	// ÅiÉGÉfÉBÉbÉgÅj
	m_wndToolBar.GetItemRect ( nIndex + 1, &rect );
	rect.top ++;
	rect.bottom --;
	m_editFrameInt.Create ( ES_RIGHT | WS_VISIBLE | WS_TABSTOP, rect, &m_wndToolBar, IDC_FRAMEINTEDIT );
	m_editFrameInt.ModifyStyleEx ( 0, WS_EX_CLIENTEDGE );
	m_editFrameInt.SetWindowText("0");
	// ÅiÉXÉsÉìÅj
	rect.left = rect.right-8;
	m_spinFrameInt.Create ( UDS_ALIGNRIGHT | WS_VISIBLE | WS_TABSTOP | UDS_SETBUDDYINT, rect, &m_wndToolBar, IDC_FRAMEINTSPIN );
	m_spinFrameInt.SetRange ( 0, 32767 );
	m_spinFrameInt.SetPos ( 0 );
	m_spinFrameInt.SetBuddy ( &m_editFrameInt );
	// ÉtÉHÉìÉgÇÃïœçX
	m_fontToolBar.CreateFont ( -12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,0,0,0,VARIABLE_PITCH | FF_SWISS,NULL );
//	m_fontToolBar.CreateFont ( -12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET ,0,0,0,0,NULL );
	m_staticFrameInt.SetFont ( &m_fontToolBar );
	m_editFrameInt.SetFont ( &m_fontToolBar );

	//-------- ÉcÅ[ÉãÉoÅ[Ç…ägëÂó¶ïœçXÅEÉXÉsÉìÇí«â¡
  nIndex = m_wndToolBar.CommandToIndex(IDC_VIEW_ZOOMCAP);
	m_wndToolBar.SetButtonInfo ( nIndex,     IDC_VIEW_ZOOMCAP,  TBBS_SEPARATOR, 36);
	m_wndToolBar.SetButtonInfo ( nIndex + 1, IDC_VIEW_ZOOMEDIT, TBBS_SEPARATOR, 48);
	// ÉRÉìÉgÉçÅ[ÉãÇÃçÏê¨ÅiÉLÉÉÉvÉVÉáÉìÅj
	m_wndToolBar.GetItemRect ( nIndex, &rect );
	rect.top += 4;
	m_staticViewZoom.Create ( "ägëÂó¶", WS_VISIBLE, rect, &m_wndToolBar, IDC_VIEW_ZOOMCAP );
	// ÅiÉGÉfÉBÉbÉgÅj
	m_wndToolBar.GetItemRect ( nIndex + 1, &rect );
	rect.top ++;
	rect.bottom --;
	m_editViewZoom.Create ( ES_RIGHT | WS_VISIBLE | WS_TABSTOP, rect, &m_wndToolBar, IDC_VIEW_ZOOMEDIT );
	m_editViewZoom.ModifyStyleEx ( 0, WS_EX_CLIENTEDGE );
	m_editViewZoom.SetWindowText("0");
	// ÅiÉXÉsÉìÅj
	rect.left = rect.right-8;
	m_spinViewZoom.Create ( UDS_ALIGNRIGHT | WS_VISIBLE | WS_TABSTOP | UDS_SETBUDDYINT, rect, &m_wndToolBar, IDC_VIEW_ZOOMSPIN );
	m_spinViewZoom.SetRange ( 1, 30 );
	m_spinViewZoom.SetPos ( 0 );
	m_spinViewZoom.SetBuddy ( &m_editViewZoom );
	// ÉtÉHÉìÉgÇÃïœçX
	m_staticViewZoom.SetFont ( &m_fontToolBar );
	m_editViewZoom.SetFont ( &m_fontToolBar );

	//--------ÉcÅ[ÉãÉ{ÉbÉNÉXÇ…ÉyÉìÉTÉCÉYÅEÉXÉsÉìÇí«â¡
	// É{É^ÉìÇÉZÉpÉåÅ[É^Ç…ïœçX
	nIndex = m_wndToolBox.CommandToIndex(IDC_PENSIZECAP);
	m_wndToolBox.SetButtonInfo ( nIndex,     IDC_PENSIZECAP,  TBBS_SEPARATOR, 42);
	m_wndToolBox.SetButtonInfo ( nIndex + 1, IDC_PENSIZEEDIT, TBBS_SEPARATOR, 48);

  // ÉRÉìÉgÉçÅ[ÉãÇÃçÏê¨ÅiÉLÉÉÉvÉVÉáÉìÅj
	m_wndToolBox.GetItemRect ( nIndex, &rect );
	rect.top   += 4;
	rect.left+=5;
	rect.right+=5;
	m_staticPenSize.Create ( "Õﬂ›ïù", WS_VISIBLE, rect, &m_wndToolBox, IDC_PENSIZECAP );
	// ÅiÉGÉfÉBÉbÉgÅj
	m_wndToolBox.GetItemRect ( nIndex + 1, &rect );
	rect.top ++;
	rect.left+=5;
	rect.right+=5;
	rect.bottom --;
	m_editPenSize.Create ( ES_RIGHT | WS_VISIBLE | WS_TABSTOP, rect, &m_wndToolBox, IDC_PENSIZEEDIT );
	m_editPenSize.ModifyStyleEx ( 0, WS_EX_CLIENTEDGE );
	m_editPenSize.SetWindowText("0");
  // ÅiÉXÉsÉìÅj
	rect.left = rect.right-8;
	

	m_spinPenSize.Create ( UDS_ALIGNRIGHT | WS_VISIBLE | UDS_SETBUDDYINT, rect, &m_wndToolBox, IDC_PENSIZESPIN );
	m_spinPenSize.SetRange ( 1, 64 );
	m_spinPenSize.SetPos ( 0 );
	m_spinPenSize.SetBuddy ( &m_editPenSize );

  // ÉtÉHÉìÉgÇÃïœçX
	m_staticPenSize.SetFont ( &m_fontToolBar );
	m_editPenSize.SetFont ( &m_fontToolBar );

  //-------- 
	// É{É^ÉìÇÉZÉpÉåÅ[É^Ç…ïœçX
	nIndex = m_wndToolBox.CommandToIndex(IDC_ERASESIZECAP);
	m_wndToolBox.SetButtonInfo ( nIndex,     IDC_ERASESIZECAP,  TBBS_SEPARATOR, 62);
	m_wndToolBox.SetButtonInfo ( nIndex + 1, IDC_ERASESIZEEDIT, TBBS_SEPARATOR, 48);

  // ÉRÉìÉgÉçÅ[ÉãÇÃçÏê¨ÅiÉLÉÉÉvÉVÉáÉìÅj
	m_wndToolBox.GetItemRect ( nIndex, &rect );
	rect.top+= 4;
	rect.left+=15;
	m_staticEraseSize.Create ( "è¡ÇµÉSÉÄïù", WS_VISIBLE, rect, &m_wndToolBox, IDC_ERASESIZECAP );

  // ÅiÉGÉfÉBÉbÉgÅj
	m_wndToolBox.GetItemRect ( nIndex + 1, &rect );
	rect.top ++;
	rect.bottom --;
	m_editEraseSize.Create ( ES_RIGHT | WS_VISIBLE | WS_TABSTOP, rect, &m_wndToolBox, IDC_ERASESIZEEDIT );
	m_editEraseSize.ModifyStyleEx ( 0, WS_EX_CLIENTEDGE );
	m_editEraseSize.SetWindowText("0");
  // ÅiÉXÉsÉìÅj
	rect.left = rect.right-8;
	m_spinEraseSize.Create ( UDS_ALIGNRIGHT | WS_VISIBLE | WS_TABSTOP | UDS_SETBUDDYINT, rect, &m_wndToolBox, IDC_ERASESIZESPIN );
	m_spinEraseSize.SetRange ( 1, 64 );
	m_spinEraseSize.SetPos ( 0 );
	m_spinEraseSize.SetBuddy ( &m_editEraseSize );
  // ÉtÉHÉìÉgÇÃïœçX
	m_staticEraseSize.SetFont ( &m_fontToolBar );
	m_editEraseSize.SetFont ( &m_fontToolBar );

	nIndex=m_wndToolBox.CommandToIndex(ID_SWAP_COLOR);
	m_wndToolBox.GetItemRect(nIndex,&rect);
	rect.left+=50;
	rect.right+=50;
  //--------

	//-------- çƒê∂ÉoÅ[Ç…ÉtÉåÅ[ÉÄéwíËÉXÉsÉìÇí«â¡
  nIndex = m_wndPlay.CommandToIndex(IDC_VIEW_PLAYSEGCAP);
	m_wndPlay.SetButtonInfo ( nIndex,     IDC_VIEW_PLAYSEGCAP, TBBS_SEPARATOR, 72);
	m_wndPlay.SetButtonInfo ( nIndex + 1, IDC_VIEW_PLAYSTARTEDIT, TBBS_SEPARATOR, 48);
	m_wndPlay.SetButtonInfo ( nIndex + 5, IDC_VIEW_PLAYENDEDIT, TBBS_SEPARATOR, 48);

  // ÉRÉìÉgÉçÅ[ÉãÇÃçÏê¨ÅiÉLÉÉÉvÉVÉáÉìÅj
	m_wndPlay.GetItemRect ( nIndex, &rect );
	rect.top += 4;
	rect.left-=5;
	rect.right+=30;
	m_staticPlaySeg.Create ( "ÉtÉåÅ[ÉÄéwíË", WS_VISIBLE, rect, &m_wndPlay, IDC_VIEW_PLAYSEGCAP );
	// ÅiÉGÉfÉBÉbÉgÅj
	m_wndPlay.GetItemRect ( nIndex + 1, &rect );
	rect.top ++;
	rect.bottom --;
	rect.left+=10;
	rect.right+=5;
	
	m_editPlayStart.Create ( ES_RIGHT | WS_VISIBLE | WS_TABSTOP, rect, &m_wndPlay, IDC_VIEW_PLAYSTARTEDIT );
	m_editPlayStart.ModifyStyleEx ( 0, WS_EX_CLIENTEDGE );
	m_editPlayStart.SetWindowText("0");
  // ÅiÉXÉsÉìÅj
	rect.left+=40;
	rect.left = rect.right-8;
	
	m_spinPlayStart.Create ( UDS_ALIGNRIGHT | WS_VISIBLE | WS_TABSTOP | UDS_SETBUDDYINT, rect, &m_wndPlay, IDC_VIEW_PLAYSTARTSPIN );
	m_spinPlayStart.SetRange ( 1, 255 );
	m_spinPlayStart.SetPos ( 0 );
	m_spinPlayStart.SetBuddy ( &m_editPlayStart );

  m_wndPlay.GetItemRect ( nIndex + 5, &rect );
	rect.top ++;
	rect.bottom --;
	m_editPlayEnd.Create ( ES_RIGHT | WS_VISIBLE | WS_TABSTOP, rect, &m_wndPlay, IDC_VIEW_PLAYENDEDIT );
	m_editPlayEnd.ModifyStyleEx ( 0, WS_EX_CLIENTEDGE );
	m_editPlayEnd.SetWindowText("0");
  // ÅiÉXÉsÉìÅj
	rect.left = rect.right-8;
	m_spinPlayEnd.Create ( UDS_ALIGNRIGHT | WS_VISIBLE | WS_TABSTOP | UDS_SETBUDDYINT, rect, &m_wndPlay, IDC_VIEW_PLAYENDSPIN );
	m_spinPlayEnd.SetRange ( 1, 255 );
	m_spinPlayEnd.SetPos ( 0 );
	m_spinPlayEnd.SetBuddy ( &m_editPlayEnd );
  
	// ÉtÉHÉìÉgÇÃïœçX
	m_staticPlaySeg.SetFont ( &m_fontToolBar );
	m_editPlayStart.SetFont ( &m_fontToolBar );
	m_editPlayEnd.SetFont ( &m_fontToolBar );

  // ÉuÉâÉVÇÃê›íË
  m_bmpBrush.CreateBitmap( 16*16, 15, 1, 1, NULL );
  CDC dc;
  dc.CreateCompatibleDC( NULL );
  dc.SaveDC();
  dc.SelectObject( &m_bmpBrush );
  int i;
  for( i = 0 ; i < 16 ; i++ ) {
    CBrush brush( RGB(i*16, i*16, i*16));
    dc.SelectObject( &brush );
    dc.Rectangle( i*16, 0, (i+1)*16, 15 );
  }

  dc.RestoreDC(-1);

  m_wndBrush.SetBitmap( (HBITMAP) m_bmpBrush.GetSafeHandle() );



#ifdef _EASYMODE
  // èâêSé“ÉÇÅ[ÉhópÇÃ UI ïœçX
  m_wndPlay.ModifyStyle( WS_VISIBLE, 0 );
  m_wndMove.ModifyStyle( WS_VISIBLE, 0 );
  m_wndBrush.ModifyStyle( WS_VISIBLE, 0 );
#endif

return 0;
}

int CMainFrame::GetFrameDelay ()
{
	return m_spinFrameInt.GetPos();
}

void CMainFrame::SetFrameDelay ( int nDelay )
{
	m_spinFrameInt.SetPos( nDelay );
}


int CMainFrame::GetPenSize ()
{
	return m_spinPenSize.GetPos();
}

void CMainFrame::SetPenSize ( int nSize )
{
	m_spinPenSize.SetPos( nSize );
}

void CMainFrame::SetEraseSize(int nSize)
{
	m_spinEraseSize.SetPos( nSize );
}

void CMainFrame::SetZoomMagnify(int nZoomMagnify)
{
  m_spinViewZoom.SetPos( nZoomMagnify );
}

void CMainFrame::OnViewToolbox() 
{

	ShowControlBar(&m_wndToolBox,!m_wndToolBox.IsVisible(),FALSE);
	RecalcLayout();
	
}

void CMainFrame::OnUpdateViewToolbox(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck ( m_wndToolBox.GetStyle() & WS_VISIBLE ? 1 : 0 );
}

void CMainFrame::OnUpdateFrameIndicator(CCmdUI* pCmdUI)
{
	CEasyToonView*  pView = (CEasyToonView*)GetActiveView();
	CString		 sFrame;
	int			 nCurFrame = pView->GetCurrentFrame() + 1;
	int			 nMaxFrame = pView->GetDocument()->GetCount();
	sFrame.Format ( "%i/%i", nCurFrame, nMaxFrame );
	pCmdUI->SetText ( sFrame );
}

void CMainFrame::OnFrameIntChange()
{
	CEasyToonView*  pView = (CEasyToonView*)GetActiveView();
	if (pView) pView->SetFrameDelay ( m_spinFrameInt.GetPos() );
}

void CMainFrame::OnViewZoomChange()
{
	CEasyToonView*  pView = (CEasyToonView*)GetActiveView();
	if (pView) pView->SetZoomMagnify( m_spinViewZoom.GetPos() );

}

void CMainFrame::OnPenSizeChange()
{
	CEasyToonView*  pView = (CEasyToonView*)GetActiveView();
	if (pView) pView->SetPenSize ( m_spinPenSize.GetPos() );
}

void CMainFrame::OnEraseSizeChange()
{
	CEasyToonView*  pView = (CEasyToonView*)GetActiveView();
	if (pView) pView->SetEraseSize ( m_spinEraseSize.GetPos() );
}

void CMainFrame::OnHelpIndex() 
{
	AfxGetApp()->WinHelp(0,HELP_CONTENTS);	
}

void CMainFrame::SetPlayLimit(int nFrames)
{
  m_spinPlayStart.SetRange( 1, nFrames );
  m_spinPlayEnd.SetRange( 1, nFrames );
}

// èâä˙âªóp
void CMainFrame::SetPlayPos(int nStart, int nEnd)
{
  m_spinPlayStart.SetPos( nStart );
  m_spinPlayEnd.SetPos( nEnd );
}

int CMainFrame::GetStartFrame()
{
  return m_spinPlayStart.GetPos();
}

int CMainFrame::GetEndFrame()
{
  return m_spinPlayEnd.GetPos();
}

void CMainFrame::OnViewNavigationBar()
{
	ShowControlBar(&m_wndMove,!m_wndMove.IsVisible(),FALSE);
	RecalcLayout();
}

void CMainFrame::OnViewFillstyleBar()
{
	ShowControlBar(&m_wndBrush,!m_wndBrush.IsVisible(),FALSE);
	RecalcLayout();
}

void CMainFrame::OnViewSelectionBar()
{
	ShowControlBar(&m_wndSelect,!m_wndSelect.IsVisible(),FALSE);
	RecalcLayout();
}

void CMainFrame::OnViewAnimationBar()
{
	ShowControlBar(&m_wndPlay,!m_wndPlay.IsVisible(),FALSE);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewAnimationBar(CCmdUI *pCmdUI)
{
	// TODO : ajoutez ici le code du gestionnaire d'interface utilisateur de mise ‡ jour des commandes
	pCmdUI->SetCheck(m_wndPlay.IsVisible());
}

void CMainFrame::OnUpdateViewSelectionBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndSelect.IsVisible());
}

void CMainFrame::OnUpdateViewFillstyleBar(CCmdUI *pCmdUI)
{
	// TODO : ajoutez ici le code du gestionnaire d'interface utilisateur de mise ‡ jour des commandes
	pCmdUI->SetCheck(m_wndBrush.IsVisible());

}

void CMainFrame::OnUpdateViewNavigationBar(CCmdUI *pCmdUI)
{
	// TODO : ajoutez ici le code du gestionnaire d'interface utilisateur de mise ‡ jour des commandes
	pCmdUI->SetCheck(m_wndMove.IsVisible());
}

void CMainFrame::SetStatusText(int pos, CString texte)
{
	m_wndStatusBar.SetPaneText(pos,texte);
}

void CMainFrame::OnClose()
{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramËtres par dÈfaut des appels
	CFrameWnd::OnClose();
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
// TODO : ajoutez ici le code de votre gestionnaire de messages
}

BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
	CDockState state;
	state.LoadState(lpszProfileName);

	for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];

		ASSERT(pInfo != NULL);

		int nDockedCount = pInfo->m_arrBarID.GetSize();
		if (nDockedCount > 0)
		{
			// dockbar
			for (int j = 0; j < nDockedCount; j++)
			{
				UINT nID = (UINT) pInfo->m_arrBarID[j];
				if (nID == 0) continue; // row separator
				if (nID > 0xFFFF)
					nID &= 0xFFFF; // placeholder - get the ID
				if (GetControlBar(nID) == NULL)
					return FALSE;
			}
		}

		if (!pInfo->m_bFloating) // floating dockbars can be created later
			if (GetControlBar(pInfo->m_nBarID) == NULL)
				return FALSE; // invalid bar ID
	}

	return TRUE;
}
