#include "stdafx.h"

#include <math.h>
#include "EasyToon.h"
#include "MainFrm.h"
#include "EasyToonDoc.h"
#include "EasyToonView.h"
#include "FontDlgEt.h"
#include "PlayDlg.h"
#include "AnimPropertyDlg.h"
#include "ViewOptionDlg.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CEasyToonView, CScrollView)

BEGIN_MESSAGE_MAP(CEasyToonView, CScrollView)
	//{{AFX_MSG_MAP(CEasyToonView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FRAME_PREV, OnFramePrev)
	ON_COMMAND(ID_FRAME_NEXT, OnFrameNext)
	ON_COMMAND(ID_FRAME_INSERT, OnFrameInsert)
	ON_COMMAND(ID_FRAME_DELETE, OnFrameDelete)
	ON_COMMAND(ID_FRAME_CLEAR, OnFrameClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_TOOL_SELECT, OnToolSelect)
	ON_UPDATE_COMMAND_UI(ID_TOOL_SELECT, OnUpdateToolSelect)
	ON_COMMAND(ID_FILE_EXPORTGIF, OnFileExportgif)
	ON_COMMAND(ID_VIEW_OPTION, OnViewOption)
	ON_COMMAND(ID_VIEW_PLAY, OnViewPlay)
	ON_COMMAND(ID_VIEW_PROPERTY, OnViewProperty)
	ON_UPDATE_COMMAND_UI(ID_FRAME_PREV, OnUpdateFramePrev)
	ON_COMMAND(ID_FILE_EXPORTBMP, OnFileExportbmp)
	ON_COMMAND(ID_FILE_IMPORTGIF, OnFileImportgif)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW_NEXTFRAME, OnDrawNextframe)
	ON_UPDATE_COMMAND_UI(ID_DRAW_NEXTFRAME, OnUpdateDrawNextframe)
	ON_COMMAND(ID_DRAW_PREVFRAME, OnDrawPrevframe)
	ON_UPDATE_COMMAND_UI(ID_DRAW_PREVFRAME, OnUpdateDrawPrevframe)
	ON_COMMAND(ID_SWAP_COLOR, OnSwapColor)
	ON_COMMAND(ID_VIEW_PLAYSEG, OnViewPlayseg)
	ON_COMMAND(ID_FILE_IMPORTEZT, OnFileImportezt)
	ON_COMMAND(ID_EDIT_PASTEFROMFILE, OnEditPastefromfile)
	ON_COMMAND(ID_MASKCOPY, OnMaskcopy)
	ON_COMMAND(ID_TBSELECT_SCALEFREE, OnTbselectScalefree)
	ON_UPDATE_COMMAND_UI(ID_TBSELECT_SCALEFREE, OnUpdateTbselectScalefree)
	ON_COMMAND(ID_TBSELECT_SCALEFIX, OnTbselectScalefix)
	ON_UPDATE_COMMAND_UI(ID_TBSELECT_SCALEFIX, OnUpdateTbselectScalefix)
	ON_COMMAND(ID_TBSELECT_ROTATE, OnTbselectRotate)
	ON_UPDATE_COMMAND_UI(ID_TBSELECT_ROTATE, OnUpdateTbselectRotate)
	ON_COMMAND(ID_TBSELECT_FREE, OnTbselectFree)
	ON_UPDATE_COMMAND_UI(ID_TBSELECT_FREE, OnUpdateTbselectFree)
	ON_COMMAND(ID_MOVE_END, OnMoveEnd)
	ON_COMMAND(ID_MOVE_NEXT1, OnMoveNext1)
	ON_COMMAND(ID_MOVE_NEXT10, OnMoveNext10)
	ON_COMMAND(ID_MOVE_PREV1, OnMovePrev1)
	ON_COMMAND(ID_MOVE_PREV10, OnMovePrev10)
	ON_COMMAND(ID_MOVE_START, OnMoveStart)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_BRUSH_PT0, ID_BRUSH_PT15, OnBrushBtn)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BRUSH_PT0, ID_BRUSH_PT15, OnUpdateBrushBtn)
	ON_COMMAND_RANGE(ID_TOOL_PEN, ID_TOOL_PARALINE, OnToolBoxBtn)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_PEN, ID_TOOL_PARALINE, OnUpdateToolBoxBtn)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_TBSELECT_FLIP_HORIZONTAL, OnFlipHorizontal)
	ON_COMMAND(ID_TBSELECT_FLIP_VERTICAL, OnFlipVertical)
	ON_COMMAND(ID_TBSELECT_NEGATIF, OnNegatif)
	ON_COMMAND(ID_TEXT, OnText)
	ON_WM_KEYDOWN()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CEasyToonView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CEasyToonView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CEasyToonDoc* CEasyToonView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEasyToonDoc)));
	return (CEasyToonDoc*)m_pDocument;
}
#endif //_DEBUG






//--------------------------------------------------------------------------
//	Constructor
//--------------------------------------------------------------------------
CEasyToonView::CEasyToonView()
{
	m_pbmpCanvas    = NULL;
	m_pbmpUndoBuf   = NULL;
	m_pbmpRedoBuf   = NULL;

  m_pbmpBackFrame = NULL;
  m_pbmpInterface = NULL;
  m_bFirstUsePara=false;
  m_pbmpCopySrc = NULL;
  m_shift=false;
  m_pbmpPattern = NULL;

  // Initialization of the various memories DC
	m_dcCanvas.   CreateCompatibleDC( NULL );
	m_dcUndoBuf.  CreateCompatibleDC( NULL );
	m_dcBackFrame.CreateCompatibleDC( NULL );
  m_dcInterface.CreateCompatibleDC( NULL );

  m_dcCopySrc.CreateCompatibleDC( NULL );

  m_dcCanvas.   SaveDC();
	m_dcUndoBuf.  SaveDC();
  m_dcBackFrame.SaveDC();
  m_dcInterface.SaveDC();

  m_dcCopySrc.SaveDC();
 
  // Early stages of canvas
	m_dcCanvas.SelectStockObject( WHITE_PEN );
	m_dcCanvas.SelectStockObject( NULL_BRUSH );
	m_dcCanvas.SetROP2( R2_XORPEN );
}

//--------------------------------------------------------------------------
//	Destructor
//--------------------------------------------------------------------------
CEasyToonView::~CEasyToonView()
{
	m_dcCanvas.   RestoreDC( -1 );
	m_dcUndoBuf.  RestoreDC( -1 );

  m_dcBackFrame.RestoreDC( -1 );
  m_dcInterface.RestoreDC( -1 );

  m_dcCopySrc.RestoreDC( -1 );

  if ( m_pbmpCanvas )    delete m_pbmpCanvas;
	if ( m_pbmpUndoBuf )   delete m_pbmpUndoBuf;
	if ( m_pbmpRedoBuf )   delete m_pbmpRedoBuf;

  if( m_pbmpBackFrame ) delete m_pbmpBackFrame;
  if( m_pbmpInterface ) delete m_pbmpInterface;

  if( m_pbmpCopySrc ) delete m_pbmpCopySrc;

  if( m_pbmpPattern ) delete m_pbmpPattern;
}

//--------------------------------------------------------------------------
// PreCreateWindow
//--------------------------------------------------------------------------
BOOL CEasyToonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// A new class is registered in order to change the background color of a window.
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW ,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_BTNSHADOW+1),
		AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	return CScrollView::PreCreateWindow(cs);
	
}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	Document linkage
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	OnInitialUpdate "New creation"
//--------------------------------------------------------------------------
void CEasyToonView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	CEasyToonDoc*	pDoc		= GetDocument();
	CMainFrame*		pMainFrame	= GetMainFrame();
	CDC*			pdc			= GetDC();

	//-------- Initialization of each attribute
	m_sizeFrame		= pDoc->GetFrameSize();		// Frame size
	m_nZoomMagnify	= FRAMEVIEW_DEFAULTMAGNIFY;	// The rate of zoom
	m_nTraceDepth	= 1;				// The depth which acts as Torres
	m_dTraceFade	= 0.2;					// Torres's light intensity
	m_crLeftPen		= RGB(0,0,0);		// The color of a left pen (black)
	m_crRightPen	= RGB(255,255,255 );// The color of a right pen (white)
	m_nPenSize		= 2;						// Pen size (2)
	m_nRPenSize   = 10;           // Eraser size (16)
	m_bButton = false;
	m_bLine   = false;
	m_nLastButton = -1;
    m_iCopyMode = SCALEFREE;

	m_paraline_B=0;
	m_paraline_coefdir=0;

	m_CanvasMarginH=20;
    m_CanvasMarginW=(GetSystemMetrics(SM_CYSCREEN)-m_sizeFrame.cy)/2;


	//-------- Initialization of each state
	m_nCurrentFrame = 0;						// Current frame
	m_nTool			= pen;						// Tool (Pen)
	m_blCanUndo		= FALSE;					// Undoing flag
	m_blCanRedo		= FALSE;					// Re-doing flag

  // Is the frame of order drawn or not?
  m_bDrawPrev = true;
  m_bDrawNext = true;
  m_rectCopyDst.SetRectEmpty();
  m_rectCopySrc.SetRectEmpty();

	//-------- Initialization of a frame window (tool bar tool box)
	pMainFrame->SetPenSize( m_nPenSize );								 
//	Pen size
	pMainFrame->SetEraseSize( m_nRPenSize );								// Erase pen size
	pMainFrame->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );	
// Frame interval
  pMainFrame->SetZoomMagnify( m_nZoomMagnify ); // Screen magnifying power

  pMainFrame->SetPlayPos( 1, 1 );
  pMainFrame->SetPlayLimit( pDoc->GetCount() );

  m_ptCopyDst[0].x = 
  m_ptCopyDst[1].x = 
  m_ptCopyDst[2].x = 
  m_ptCopyDst[3].x = 
  m_ptCopyDst[0].y = 
  m_ptCopyDst[1].y = 
  m_ptCopyDst[2].y = 
  m_ptCopyDst[3].y = 0;

	//-------- Re-initialization of various bit maps
	DWORD* pBmpSrc = new DWORD[ m_sizeFrame.cx * m_sizeFrame.cy ];
	memset ( pBmpSrc, 0xff,  m_sizeFrame.cx * m_sizeFrame.cy * 4 );

	if( m_pbmpCanvas )    delete m_pbmpCanvas;
	if( m_pbmpUndoBuf )   delete m_pbmpUndoBuf;
	if( m_pbmpRedoBuf )   delete m_pbmpRedoBuf;
  if( m_pbmpBackFrame ) delete m_pbmpBackFrame;
  if( m_pbmpInterface ) delete m_pbmpInterface;

  m_pbmpCanvas    = new CBitmap();
	m_pbmpUndoBuf   = new CBitmap();
	m_pbmpRedoBuf   = new CBitmap();
  m_pbmpBackFrame = new CBitmap();
  m_pbmpInterface = new CBitmap();

	m_pbmpCanvas   ->CreateCompatibleBitmap( pdc, m_sizeFrame.cx, m_sizeFrame.cy );
	m_pbmpUndoBuf  ->CreateBitmap( m_sizeFrame.cx, m_sizeFrame.cy, 1,  1, pBmpSrc );
	m_pbmpRedoBuf  ->CreateBitmap( m_sizeFrame.cx, m_sizeFrame.cy, 1,  1, pBmpSrc );
  m_pbmpBackFrame->CreateCompatibleBitmap( pdc, m_sizeFrame.cx, m_sizeFrame.cy );
  m_pbmpInterface->CreateCompatibleBitmap( pdc, m_sizeFrame.cx, m_sizeFrame.cy );
  
	m_dcCanvas.   SelectObject( m_pbmpCanvas );
	m_dcUndoBuf.  SelectObject( m_pbmpUndoBuf );
	m_dcBackFrame.SelectObject( m_pbmpBackFrame );
	m_dcInterface.SelectObject( m_pbmpInterface );
  
	delete [] pBmpSrc;

	//-------- Initialization of view size
	SetScrollSizes( MM_TEXT, CSize( 2 * m_CanvasMarginW + m_nZoomMagnify * m_sizeFrame.cx, 
								    2 * m_CanvasMarginH + m_nZoomMagnify * m_sizeFrame.cy  ) );

	ReleaseDC(pdc);

  _DrawBackFrame();

  // Pattern
  m_nBrush = 0;
  _CreatePatternBrush();
}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	Screen rewriting relation
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// BackFrame: Light color display portion   It draws only at the time of a frame change.
// Interface: Only the primitive before drawing decision draws.
// Canvas: BackFrame Interface What compounded the current frame
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// The action at the time of copy/expansion reduction
// BackFrame Current frame
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//	The bit map of only a light color display portion is drawn.
//--------------------------------------------------------------------------
void CEasyToonView::_DrawBackFrame()
{
  // –Ñ‚Ú`‚ñ
	CEasyToonDoc* pDoc = GetDocument();
  DWORD dwROP = SRCCOPY;

	//-------- It is in thin blue about a front frame.
  if ( m_nTraceDepth && (m_nCurrentFrame != 0) && (m_bDrawPrev != false) ) {
    int nCont = 256 - (int)(256.0 * m_dTraceFade);
		m_dcBackFrame.SetTextColor( RGB(nCont*2,nCont*2,nCont) );
		m_dcBackFrame.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, 
                          pDoc->GetFrameDC(m_nCurrentFrame-1), 0, 0, dwROP );
    dwROP = SRCAND;
	}

  //-------- It is in thin red about the following frame.

  if( (m_bDrawNext != false) && (m_nCurrentFrame < (pDoc->GetCount()-1) ) ) {
    int nCont = 256 - (int)(256.0 * m_dTraceFade);
  	m_dcBackFrame.SetTextColor( RGB(nCont,nCont*2,nCont*2) );
  	m_dcBackFrame.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, 
                          pDoc->GetFrameDC(m_nCurrentFrame+1), 0, 0, dwROP );
    dwROP = SRCAND;
  }

  // When drawing nothing
  if( dwROP == SRCCOPY ) {
    CBrush brush( 0xffffff );
    CRect rect( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy );
    m_dcBackFrame.FillRect( &rect, &brush );
  
  }
}

//--------------------------------------------------------------------------
// Interface drawing
// The display of the state before drawing decision
//--------------------------------------------------------------------------
void CEasyToonView::_DrawInterface()
{
  switch( m_nTool ) {
	case rect:
	case fillRect:  _DrawInterfaceRectangle(); break;
	case ellipse: 
	case fillEllipse: _DrawInterfaceEllipse(); break;
	case line:case paraline: _DrawInterfaceLine(); break;
  case linestrip: _DrawInterfaceLine(); break;
  case move: _DrawInterfaceMove(); break;
  }
}

//--------------------------------------------------------------------------
// Line
//--------------------------------------------------------------------------
void CEasyToonView::_DrawInterfaceLine()
{
  if( m_nInterfacePenSize <= 0 ) return; 
  // Pen creation
  CPen pen, *ppenOld;
	pen.CreatePen( PS_SOLID, m_nInterfacePenSize, m_crInterfacePenColor );

  ppenOld = m_dcInterface.SelectObject( &pen );
	// drawing
	m_dcInterface.MoveTo( m_ptFrom );
	m_dcInterface.LineTo( m_ptTo );
  // Something   こ - Since a point is not struck to the place of LineTo, carry out.
  if( m_nInterfacePenSize == 1 ) {
    m_dcInterface.LineTo( m_ptTo.x - 1, m_ptTo.y );
  }

  m_dcInterface.SelectObject( ppenOld );
  // End
}


//--------------------------------------------------------------------------
// Rectangle
//--------------------------------------------------------------------------
void CEasyToonView::_DrawInterfaceRectangle()
{
  if( m_nInterfacePenSize <= 0 ) return; 
  // Pen creation
  CPen pen;
	pen.CreatePen( PS_SOLID, m_nInterfacePenSize, m_crInterfacePenColor );
  CBrush brush( m_pbmpPattern );

  m_dcInterface.SaveDC();
  m_dcInterface.SelectObject( &pen );

  if( m_nTool == fillRect ) {
    m_dcInterface.SelectObject( &brush );
    m_dcInterface.SetTextColor( m_crInterfacePenColor );
    m_dcInterface.SetBkColor( ~m_crInterfacePenColor );
  } else
  	m_dcInterface.SelectStockObject( NULL_BRUSH );

  // Rectangle drawing
  CRect Rect(m_ptFrom,m_ptTo);

  CString  deb;	

  if ((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0)
		{
			SquareRect(Rect, m_ptFrom, m_ptTo);
		}  

  Rect.SetRect(m_ptFrom,m_ptTo);
  Rect.NormalizeRect();

  m_dcInterface.Rectangle( Rect ); 

  m_dcInterface.RestoreDC( -1 );
  // End
}



//--------------------------------------------------------------------------
// Ellipse
//--------------------------------------------------------------------------
void CEasyToonView::_DrawInterfaceEllipse()
{
  if( m_nInterfacePenSize <= 0 ) return; 
  // Pen Creation
  CPen pen;
  pen.CreatePen( PS_SOLID, m_nInterfacePenSize, m_crInterfacePenColor );
  CBrush brush( m_pbmpPattern );

  m_dcInterface.SaveDC();
  m_dcInterface.SelectObject( &pen );

  if( m_nTool == fillEllipse ) {
    m_dcInterface.SelectObject( &brush );
    m_dcInterface.SetTextColor( m_crInterfacePenColor );
    m_dcInterface.SetBkColor( ~m_crInterfacePenColor );
  } else
  	m_dcInterface.SelectStockObject( NULL_BRUSH );
  // Rectangle drawing
  CPoint pt;
  pt = m_ptFrom - m_ptTo;
  if( pt.x < 0 ) pt.x = -pt.x;
  if( pt.y < 0 ) pt.y = -pt.y;

  CRect rect;

  rect.top = m_ptFrom.y - pt.y;
  rect.bottom = m_ptFrom.y + pt.y;
  rect.left = m_ptFrom.x - pt.x;
  rect.right = m_ptFrom.x + pt.x;

 if ((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0)
 {
	SquareRect(rect,m_ptFrom,m_ptTo);
 }
  rect.top = m_ptFrom.y - pt.y;
  rect.bottom = m_ptFrom.y + pt.y;
  rect.left = m_ptFrom.x - pt.x;
  rect.right = m_ptFrom.x + pt.x;

  m_dcInterface.Ellipse ( rect );

  m_dcInterface.RestoreDC( -1 );
  // Fin
}

//--------------------------------------------------------------------------
// Rectangle movement
//--------------------------------------------------------------------------
void CEasyToonView::_DrawInterfaceMove()
{
  if( (m_iCopyMode == ROTATE) || (m_iCopyMode == FREE) ) {
    // ƒeƒLƒg[
    if( m_nMoveSubStat == stretchview ) {
      int x;
      int y;
      POINT ptRaster[4], ptPixel[4];
      
      BYTE *pPixel;
      int nbPixelBytes = (m_rectCopySrc.Width() >> 3) + (m_rectCopySrc.Width() & 0x7 ? 1 : 0); // Raster size
      nbPixelBytes += (4-(nbPixelBytes & 0x3)) & 0x3; // DWORD ƒoƒEƒ“ƒ_ƒŠ
      nbPixelBytes *= m_rectCopySrc.Height();

      pPixel = new BYTE[nbPixelBytes];

	  // Copied material pixel acquisition
      m_pbmpCopySrc->GetBitmapBits( nbPixelBytes, (LPVOID) pPixel );

      m_dcInterface.SaveDC();

      // Size of a copy place
      for( y = 0 ; y < m_rectCopySrc.Height() ; y++ ) {
        ptRaster[0].x = (m_ptCopyDst[0].x*(m_rectCopySrc.Height()-y) + m_ptCopyDst[3].x*y)/m_rectCopySrc.Height();
        ptRaster[0].y = (m_ptCopyDst[0].y*(m_rectCopySrc.Height()-y) + m_ptCopyDst[3].y*y)/m_rectCopySrc.Height();
        ptRaster[1].x = (m_ptCopyDst[1].x*(m_rectCopySrc.Height()-y) + m_ptCopyDst[2].x*y)/m_rectCopySrc.Height();
        ptRaster[1].y = (m_ptCopyDst[1].y*(m_rectCopySrc.Height()-y) + m_ptCopyDst[2].y*y)/m_rectCopySrc.Height();
        ptRaster[2].x = (m_ptCopyDst[1].x*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[2].x*(y+1))/m_rectCopySrc.Height();
        ptRaster[2].y = (m_ptCopyDst[1].y*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[2].y*(y+1))/m_rectCopySrc.Height();
        ptRaster[3].x = (m_ptCopyDst[0].x*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[3].x*(y+1))/m_rectCopySrc.Height();
        ptRaster[3].y = (m_ptCopyDst[0].y*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[3].y*(y+1))/m_rectCopySrc.Height();

        for( x = 0 ; x < m_rectCopySrc.Width() ; x++ ) {
          if( CEasyToonDoc::_GetPixelColor1( pPixel, m_rectCopySrc.Width(), m_rectCopySrc.Height(), x, y) != 0 ) {
            m_dcInterface.SelectStockObject( WHITE_PEN );
            m_dcInterface.SelectStockObject( WHITE_BRUSH );
          } else {
            m_dcInterface.SelectStockObject( BLACK_PEN );
            m_dcInterface.SelectStockObject( BLACK_BRUSH );
          }
          
          ptPixel[0].x = (ptRaster[0].x*(m_rectCopySrc.Width()-x) + ptRaster[1].x*x)/m_rectCopySrc.Width();
          ptPixel[0].y = (ptRaster[0].y*(m_rectCopySrc.Width()-x) + ptRaster[1].y*x)/m_rectCopySrc.Width();
          ptPixel[1].x = (ptRaster[0].x*(m_rectCopySrc.Width()-(x+1)) + ptRaster[1].x*(x+1))/m_rectCopySrc.Width();
          ptPixel[1].y = (ptRaster[0].y*(m_rectCopySrc.Width()-(x+1)) + ptRaster[1].y*(x+1))/m_rectCopySrc.Width();
          ptPixel[2].x = (ptRaster[3].x*(m_rectCopySrc.Width()-(x+1)) + ptRaster[2].x*(x+1))/m_rectCopySrc.Width();
          ptPixel[2].y = (ptRaster[3].y*(m_rectCopySrc.Width()-(x+1)) + ptRaster[2].y*(x+1))/m_rectCopySrc.Width();
          ptPixel[3].x = (ptRaster[3].x*(m_rectCopySrc.Width()-x) + ptRaster[2].x*x)/m_rectCopySrc.Width();
          ptPixel[3].y = (ptRaster[3].y*(m_rectCopySrc.Width()-x) + ptRaster[2].y*x)/m_rectCopySrc.Width();

          m_dcInterface.Polygon( ptPixel, 4 );
        }
      }
      m_dcInterface.RestoreDC( -1 );
      delete [] pPixel;
    }
    return;
  }
  
  if( (m_rectCopySrc.IsRectEmpty() == FALSE) && (m_rectCopyDst.IsRectEmpty() == FALSE) ) {
	  // Also m_rectCopySrc m_rectCopyDst is also contained with canvas coordinates.

    
    CRect rectDst;
    rectDst = m_rectCopyDst;

    rectDst.top    = _ClipCanvasV( rectDst.top );
    rectDst.bottom = _ClipCanvasV( rectDst.bottom );
    rectDst.left   = _ClipCanvasH( rectDst.left );
    rectDst.right  = _ClipCanvasH( rectDst.right );

    // Only ぶん shrunk by clipping transforms a copied material.
    // めんど -- it is stinking
    CRect rectSrc;
    rectSrc.top = ((rectDst.top - m_rectCopyDst.top)*m_rectCopySrc.Height())/m_rectCopyDst.Height();
    rectSrc.bottom = m_rectCopySrc.Height() + ((rectDst.bottom - m_rectCopyDst.bottom)*m_rectCopySrc.Height())/m_rectCopyDst.Height();
    rectSrc.left = ((rectDst.left - m_rectCopyDst.left)*m_rectCopySrc.Width())/m_rectCopyDst.Width();
    rectSrc.right = m_rectCopySrc.Width() + ((rectDst.right - m_rectCopyDst.right)*m_rectCopySrc.Width())/m_rectCopyDst.Width();

    CBrush brush( 0xffffff );
    m_dcInterface.FillRect( rectDst, &brush );
    m_dcInterface.StretchBlt( rectDst.left, rectDst.top, rectDst.Width(), rectDst.Height(), 
    &m_dcCopySrc, rectSrc.left, rectSrc.top, rectSrc.Width(), rectSrc.Height(), SRCCOPY );
  }
}


//--------------------------------------------------------------------------
//	DrawFrames: It is Blt to canvas in piles about a frame.
//--------------------------------------------------------------------------
void CEasyToonView::DrawFrames( CRect* pRect, BOOL blSelReg )
{
	CEasyToonDoc* pDoc = GetDocument();
	CRect rcUpdate;
	
	//-------- If the updating domain is specified, AND with canvas will be taken.
	if ( pRect ) rcUpdate = *pRect & CRect( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy );

  // A background is copied first.  
  DWORD dwROP = SRCCOPY;
	if ( pRect ) {
		m_dcCanvas.BitBlt(	rcUpdate.left, 
								rcUpdate.top,
								rcUpdate.Width(), 
								rcUpdate.Height(), 
								&m_dcBackFrame,
								rcUpdate.left,
								rcUpdate.top,
								SRCCOPY );
  } else {
			m_dcCanvas.BitBlt(	0, 
								0, 
								m_sizeFrame.cx, 
								m_sizeFrame.cy, 
								&m_dcBackFrame,
								0, 
								0, 
								SRCCOPY );
  }

  // A current frame is copied to an interface.
  m_dcInterface.SetTextColor ( RGB ( 0, 0, 0 ) );
 	if ( pRect ) {
    m_dcInterface.BitBlt( rcUpdate.left, 
	  						rcUpdate.top, 
		  					rcUpdate.Width(),
			  				rcUpdate.Height(),
				  			pDoc->GetFrameDC( m_nCurrentFrame ),
					  		rcUpdate.left,
						  	rcUpdate.top,
							  SRCCOPY );
  } else {
		m_dcInterface.BitBlt(	0, 
							0, 
							m_sizeFrame.cx,
							m_sizeFrame.cy,
							pDoc->GetFrameDC( m_nCurrentFrame ),
							0,
							0,
							SRCCOPY );
	}

  // Interface creation
   _DrawInterface();

  // It is AND about an interface at canvas, and is Blt.
	m_dcCanvas.SetTextColor ( RGB ( 0, 0, 0 ) );
 	if ( pRect ) {
    m_dcCanvas.BitBlt( rcUpdate.left, 
	  						rcUpdate.top, 
		  					rcUpdate.Width(),
			  				rcUpdate.Height(),
				  			&m_dcInterface,
					  		rcUpdate.left,
						  	rcUpdate.top,
							  SRCAND );
  } else {
		m_dcCanvas.BitBlt(	0, 
							0, 
							m_sizeFrame.cx,
							m_sizeFrame.cy,
							&m_dcInterface,
							0,
							0,
							SRCAND );
	}
}

//--------------------------------------------------------------------------
//	DrawCanvas: It is Blt to a window about canvas.
//--------------------------------------------------------------------------
void CEasyToonView::DrawCanvas( CDC* pDC, CRect* pRect )
{
	CDC* pDestDC;
	if ( !pDC ) OnPrepareDC( pDestDC = GetDC() ); else pDestDC = pDC;

  if ( pRect ) {
    // A part is updated.
		CRect rcUpdate = *pRect & CRect( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy );
		pDestDC->StretchBlt( m_CanvasMarginW + m_nZoomMagnify * rcUpdate.left, 
                         m_CanvasMarginH + m_nZoomMagnify * rcUpdate.top,
                         m_nZoomMagnify * rcUpdate.Width(),
                         m_nZoomMagnify * rcUpdate.Height(), 
                         &m_dcCanvas, 
                         rcUpdate.left, 
                         rcUpdate.top,
                         rcUpdate.Width(), 
                         rcUpdate.Height(), 
                         SRCCOPY );
  
    /*
    if( m_nZoomMagnify > 1 ) {
      int i;
      CPen pen( PS_SOLID, 1, 0xcccccc );
      CPen *ppenOld;
      ppenOld = pDestDC->SelectObject( &pen );
      for( i = rcUpdate.left ; i < rcUpdate.right ; i++ ) {
        pDestDC->MoveTo( FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN + rcUpdate.top*m_nZoomMagnify );
        pDestDC->LineTo( FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN + rcUpdate.bottom*m_nZoomMagnify );
      }
      for( i = rcUpdate.top ; i < rcUpdate.bottom ; i++ ) {
        pDestDC->MoveTo( FRAMEVIEW_CANVASMARGIN + rcUpdate.left*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify );
        pDestDC->LineTo( FRAMEVIEW_CANVASMARGIN + rcUpdate.right*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify );
      }
      pDestDC->SelectObject( ppenOld );
    }
    */
		
  } else {
    // The whole is updated.
		pDestDC->StretchBlt( m_CanvasMarginW, 
							 m_CanvasMarginH, 
							 m_nZoomMagnify * m_sizeFrame.cx, 
							 m_nZoomMagnify * m_sizeFrame.cy, 
							 &m_dcCanvas, 
							 0,
							 0, 
							 m_sizeFrame.cx, 
							 m_sizeFrame.cy, 
							 SRCCOPY );
    
	/*
    if( m_nZoomMagnify > 1 ) {
      int i;
      CPen pen( PS_SOLID, 1, 0xcccccc );
      CPen *ppenOld;
      ppenOld = pDestDC->SelectObject( &pen );
      for( i = 0 ; i < m_sizeFrame.cx ; i++ ) {
        pDestDC->MoveTo( FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN );
        pDestDC->LineTo( FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN + m_sizeFrame.cy*m_nZoomMagnify );
      }
      for( i = 0 ; i < m_sizeFrame.cy ; i++ ) {
        pDestDC->MoveTo( FRAMEVIEW_CANVASMARGIN, FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify );
        pDestDC->LineTo( FRAMEVIEW_CANVASMARGIN + m_sizeFrame.cx*m_nZoomMagnify, FRAMEVIEW_CANVASMARGIN + i*m_nZoomMagnify );
      }
      pDestDC->SelectObject( ppenOld );
    }
    */
	}


  
	if ( !pDC ) ReleaseDC( pDestDC );
}

//--------------------------------------------------------------------------
//	OnDraw
//--------------------------------------------------------------------------
void CEasyToonView::OnDraw(CDC* pDC)
{
	CEasyToonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	DrawFrames();
	DrawCanvas(pDC);

}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	Tool processing routine group
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Coordinates conversion
//--------------------------------------------------------------------------
POINT CEasyToonView::TranslatePoint( POINT pt, BOOL blClip )
{
	//-------- Conversion on local coordinates
	CClientDC dc( this );
	OnPrepareDC( &dc );
	dc.DPtoLP( &pt );

	pt.x = (pt.x - m_CanvasMarginW) / m_nZoomMagnify;
	pt.y = (pt.y - m_CanvasMarginH) / m_nZoomMagnify;
	//-------- Clipping
	if ( blClip )
	{
		pt.x = pt.x < 0 ? 0 : ( pt.x > m_sizeFrame.cx ? m_sizeFrame.cx : pt.x );
		pt.y = pt.y < 0 ? 0 : ( pt.y > m_sizeFrame.cy ? m_sizeFrame.cy : pt.y );
	}
	return pt;
}

//--------------------------------------------------------------------------
// Coordinates conversion at canvas from a screen
//--------------------------------------------------------------------------
void CEasyToonView::ScreenToCanvas( POINT* pt, BOOL blClip )
{
	//-------- Conversion on local coordinates
	CClientDC dc( this );
	OnPrepareDC( &dc );
	dc.DPtoLP( pt );
	pt->x = (pt->x - m_CanvasMarginW) / m_nZoomMagnify;
	pt->y = (pt->y - m_CanvasMarginH) / m_nZoomMagnify;
	//-------- Clipping
	if ( blClip )
	{
		pt->x = pt->x < 0 ? 0 : ( pt->x > m_sizeFrame.cx ? m_sizeFrame.cx : pt->x );
		pt->y = pt->y < 0 ? 0 : ( pt->y > m_sizeFrame.cy ? m_sizeFrame.cy : pt->y );
	}
}

//--------------------------------------------------------------------------
// It quantizes according to the resolution of canvas.
//--------------------------------------------------------------------------
int CEasyToonView::_QuantizeForCanvas(int pos)
{
  return ((pos - m_CanvasMarginW)/(pos-m_CanvasMarginH))/m_nZoomMagnify;
}

void CEasyToonView::QuantizeForCanvas( POINT* pt, BOOL blClip  )
{
  if( blClip != FALSE ) {
  	pt->x = _CanvasToScreen(_ClipCanvasH(_QuantizeForCanvas( pt->x )));
	  pt->y = _CanvasToScreen(_ClipCanvasV(_QuantizeForCanvas( pt->y )));
  } else {
  	pt->x = _CanvasToScreen(_QuantizeForCanvas( pt->x ));
	  pt->y = _CanvasToScreen(_QuantizeForCanvas( pt->y ));
  }
}

CRect CEasyToonView::_QuantizeForCanvas(CRect rect, bool bClip)
{
  CRect rectResult;
  if( bClip != false ) {
    rectResult.top    = _CanvasToScreen(_ClipCanvasV(_QuantizeForCanvas( rect.top )));
    rectResult.bottom = _CanvasToScreen(_ClipCanvasV(_QuantizeForCanvas( rect.bottom )));
    rectResult.left   = _CanvasToScreen(_ClipCanvasH(_QuantizeForCanvas( rect.left )));
    rectResult.right  = _CanvasToScreen(_ClipCanvasH(_QuantizeForCanvas( rect.right )));
  } else {
    rectResult.top    = _CanvasToScreen(_QuantizeForCanvas( rect.top ));
    rectResult.bottom = _CanvasToScreen(_QuantizeForCanvas( rect.bottom ));
    rectResult.left   = _CanvasToScreen(_QuantizeForCanvas( rect.left ));
    rectResult.right  = _CanvasToScreen(_QuantizeForCanvas( rect.right ));
  }
  return rectResult;
}

int CEasyToonView::_ClipCanvasH(int pos)
{
  if( pos < 0 ) return 0;
  if( m_sizeFrame.cx < pos ) return m_sizeFrame.cx;
  return pos;
}

int CEasyToonView::_ClipCanvasV(int pos)
{
  if( pos < 0 ) return 0;
  if( m_sizeFrame.cy < pos ) return m_sizeFrame.cy;
  return pos;
}

//--------------------------------------------------------------------------
// A range specification buffer is breathed out.
//--------------------------------------------------------------------------
void CEasyToonView::PasteSelectBuf()
{
  if( ((m_iCopyMode == ROTATE) || (m_iCopyMode == FREE)) && (m_pbmpCopySrc != NULL) ) {
    CDC *pDC = GetDocument()->GetFrameDC(m_nCurrentFrame);

    // ƒeƒLƒg[
//    if( m_nMoveSubStat == stretchview ) {
      int x;
      int y;
      POINT ptRaster[4], ptPixel[4];
      
      BYTE *pPixel;
      int nbPixelBytes = (m_rectCopySrc.Width() >> 3) + (m_rectCopySrc.Width() & 0x7 ? 1 : 0); // Raster size
      nbPixelBytes += (4-(nbPixelBytes & 0x3)) & 0x3; // DWORD ƒoƒEƒ“ƒ_ƒŠ
      nbPixelBytes *= m_rectCopySrc.Height();

      pPixel = new BYTE[nbPixelBytes];

      // Copied material pixel acquisition
      m_pbmpCopySrc->GetBitmapBits( nbPixelBytes, (LPVOID) pPixel );

      pDC->SaveDC();

      // Size of a copy place
      for( y = 0 ; y < m_rectCopySrc.Height() ; y++ ) {
        ptRaster[0].x = (m_ptCopyDst[0].x*(m_rectCopySrc.Height()-y) + m_ptCopyDst[3].x*y)/m_rectCopySrc.Height();
        ptRaster[0].y = (m_ptCopyDst[0].y*(m_rectCopySrc.Height()-y) + m_ptCopyDst[3].y*y)/m_rectCopySrc.Height();
        ptRaster[1].x = (m_ptCopyDst[1].x*(m_rectCopySrc.Height()-y) + m_ptCopyDst[2].x*y)/m_rectCopySrc.Height();
        ptRaster[1].y = (m_ptCopyDst[1].y*(m_rectCopySrc.Height()-y) + m_ptCopyDst[2].y*y)/m_rectCopySrc.Height();
        ptRaster[2].x = (m_ptCopyDst[1].x*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[2].x*(y+1))/m_rectCopySrc.Height();
        ptRaster[2].y = (m_ptCopyDst[1].y*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[2].y*(y+1))/m_rectCopySrc.Height();
        ptRaster[3].x = (m_ptCopyDst[0].x*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[3].x*(y+1))/m_rectCopySrc.Height();
        ptRaster[3].y = (m_ptCopyDst[0].y*(m_rectCopySrc.Height()-(y+1)) + m_ptCopyDst[3].y*(y+1))/m_rectCopySrc.Height();

        for( x = 0 ; x < m_rectCopySrc.Width() ; x++ ) {
          if( CEasyToonDoc::_GetPixelColor1( pPixel, m_rectCopySrc.Width(), m_rectCopySrc.Height(), x, y) != 0 ) {
            pDC->SelectStockObject( WHITE_PEN );
            pDC->SelectStockObject( WHITE_BRUSH );
          } else {
            pDC->SelectStockObject( BLACK_PEN );
            pDC->SelectStockObject( BLACK_BRUSH );
          }
          
          ptPixel[0].x = (ptRaster[0].x*(m_rectCopySrc.Width()-x) + ptRaster[1].x*x)/m_rectCopySrc.Width();
          ptPixel[0].y = (ptRaster[0].y*(m_rectCopySrc.Width()-x) + ptRaster[1].y*x)/m_rectCopySrc.Width();
          ptPixel[1].x = (ptRaster[0].x*(m_rectCopySrc.Width()-(x+1)) + ptRaster[1].x*(x+1))/m_rectCopySrc.Width();
          ptPixel[1].y = (ptRaster[0].y*(m_rectCopySrc.Width()-(x+1)) + ptRaster[1].y*(x+1))/m_rectCopySrc.Width();
          ptPixel[2].x = (ptRaster[3].x*(m_rectCopySrc.Width()-(x+1)) + ptRaster[2].x*(x+1))/m_rectCopySrc.Width();
          ptPixel[2].y = (ptRaster[3].y*(m_rectCopySrc.Width()-(x+1)) + ptRaster[2].y*(x+1))/m_rectCopySrc.Width();
          ptPixel[3].x = (ptRaster[3].x*(m_rectCopySrc.Width()-x) + ptRaster[2].x*x)/m_rectCopySrc.Width();
          ptPixel[3].y = (ptRaster[3].y*(m_rectCopySrc.Width()-x) + ptRaster[2].y*x)/m_rectCopySrc.Width();

          pDC->Polygon( ptPixel, 4 );
        }
      }
      pDC->RestoreDC( -1 );
      delete [] pPixel;
//    }
    return;
  }

 	if ( !m_rectCopySrc.IsRectEmpty() && !m_rectCopyDst.IsRectEmpty() )
	{
    CRect rect;
    rect = m_rectCopyDst;

    rect.top = _ClipCanvasV( rect.top );
    rect.bottom = _ClipCanvasV( rect.bottom );
    rect.left = _ClipCanvasH( rect.left );
    rect.right = _ClipCanvasH( rect.right );

    CRect rectSrc;
    rectSrc.top = ((rect.top - m_rectCopyDst.top)*m_rectCopySrc.Height())/m_rectCopyDst.Height();
    rectSrc.bottom = m_rectCopySrc.Height() + ((rect.bottom - m_rectCopyDst.bottom)*m_rectCopySrc.Height())/m_rectCopyDst.Height();

    rectSrc.left = ((rect.left - m_rectCopyDst.left)*m_rectCopySrc.Width())/m_rectCopyDst.Width();
    rectSrc.right = m_rectCopySrc.Width() + ((rect.right - m_rectCopyDst.right)*m_rectCopySrc.Width())/m_rectCopyDst.Width();

    GetDocument()->GetFrameDC(m_nCurrentFrame)->StretchBlt(
      rect.left, rect.top, rect.Width(), rect.Height(), 
			&m_dcCopySrc, rectSrc.left, rectSrc.top, rectSrc.Width(), rectSrc.Height(), SRCCOPY );
	}
}

//--------------------------------------------------------------------------
//	Drawing start
//--------------------------------------------------------------------------
void CEasyToonView::BeginDraw ( CPoint point, int nBtn ) 
{
	CPoint	 ptMouse	= TranslatePoint ( point, FALSE );
	COLORREF crPen		= nBtn ? m_crRightPen : m_crLeftPen;
	CDC*	 pdcFrame	= GetDocument()->GetFrameDC( m_nCurrentFrame );
    int nPenSize = nBtn ? m_nRPenSize : m_nPenSize;
    CRect rectUpdate;
  
	switch ( m_nTool )
	{
		//-------- Processing of a pen
		case pen:
			{
			if(m_nBrush>0 && crPen!=RGB(255,255,255))
			{

				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = RGB(m_nBrush*16,m_nBrush*16,m_nBrush*16);
				m_penDraw.CreatePen(PS_GEOMETRIC, nPenSize, &logBrush);
			}
			else
			{
			// Renewal of a UNDO buffer
			UpdateUndoBuffer( restoreBitmap );
			// Pen creation
			
			m_penDraw.CreatePen( PS_GEOMETRIC | PS_SOLID, nPenSize, crPen );
			}
/*
      BITMAPINFO bmpinfo;
      bmpinfo.bmiHeader.biBitCount = 24;
      bmpinfo.bmiHeader.biClrImportant = 0;
      bmpinfo.bmiHeader.biClrUsed = 0;
      bmpinfo.bmiHeader.biCompression = 0;
      bmpinfo.bmiHeader.biHeight = 8;
      bmpinfo.bmiHeader.biPlanes = 1;
      bmpinfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
      bmpinfo.bmiHeader.biSizeImage = 24*8;
      bmpinfo.bmiHeader.biWidth = 8;
      bmpinfo.bmiHeader.biXPelsPerMeter = 100;
      bmpinfo.bmiHeader.biYPelsPerMeter = 100;
      void* pPixels;

      HBITMAP hBitmap = CreateDIBSection( NULL, &bmpinfo, DIB_RGB_COLORS, &pPixels, NULL, 0 );

      

      LOGBRUSH logbrush;
      logbrush.lbStyle = BS_PATTERN;
      logbrush.lbColor = DIB_RGB_COLORS;
      logbrush.lbHatch = (long) hBitmap;
      
      m_penDraw.CreatePen( PS_GEOMETRIC, nPenSize, &logbrush);
*/
	  // Drawing preparation

			pdcFrame->SaveDC();
			pdcFrame->SelectObject( &m_penDraw );
      // Windows cannot strike a point.
      if( nPenSize == 1 ) {
        pdcFrame->MoveTo( ptMouse.x, ptMouse.y );
	  		pdcFrame->LineTo( ptMouse.x-1, ptMouse.y );
        pdcFrame->MoveTo( ptMouse.x, ptMouse.y );
      } else {
        pdcFrame->MoveTo( ptMouse.x, ptMouse.y );
	  		pdcFrame->LineTo( ptMouse.x, ptMouse.y );
      }
//      pdcFrame->Rectangle( ptMouse.x, ptMouse.y, ptMouse.x, ptMouse.y );
			//CDC* pdc = GetDocument()->GetFrameDC(m_nCurrentFrame);

      // Calculation of the updating range
			CRect rc( ptMouse, ptMouse );
			rc.NormalizeRect();
      if( nBtn )
  			rc.InflateRect( m_nRPenSize, m_nRPenSize );
      else
  			rc.InflateRect( m_nPenSize, m_nPenSize );
			// Renewal of canvas
			DrawFrames(&rc);
			DrawCanvas(NULL,&rc);
      
		}
		break;
		//-------- Processing of painting out
		//ici
		case fill:
		{
			// Renewal of a UNDO buffer
			UpdateUndoBuffer( restoreBitmap );
			// Painting out
			CBrush brush( RGB(m_nBrush*16, m_nBrush*16, m_nBrush*16) );
			CBrush*	pbrushOld = pdcFrame->SelectObject( &brush );
			pdcFrame->SetTextColor( crPen );
			pdcFrame->SetBkColor( ~crPen );
			pdcFrame->FloodFill( ptMouse.x, ptMouse.y, crPen);
			pdcFrame->SelectObject( pbrushOld );
			// Renewal of canvas
			DrawFrames();
			DrawCanvas(NULL);
		}
		break;
		//-------- Linear processing
    case rect:
    case fillRect:
		case ellipse:
		case fillEllipse:
		case paraline:
    case line:
		{
		
	    // Renewal of a UNDO buffer
	    UpdateUndoBuffer( restoreBitmap );
			// Starting point acquisition
		
			ScreenToCanvas(&point);
			m_ptFrom = m_ptTo = point;
			
			if ((m_nTool==paraline) && (m_paraline_B==0.f))
			{
				m_paraline_B=point.y-(point.x*m_paraline_coefdir);
				//m_paraline_B=-m_paraline_B;
			}

			rectUpdate.SetRect( m_ptFrom, m_ptTo );
			rectUpdate.InflateRect( nPenSize, nPenSize );
			 m_crInterfacePenColor = crPen;
			m_nInterfacePenSize = nPenSize;
			// Renewal of canvas
			DrawFrames(&rectUpdate);
			DrawCanvas(NULL,&rectUpdate);
		}
		break;
    case linestrip:
      {
        if( m_bLine == false ) {
          // Drawing start
			    // Renewal of a UNDO buffer
			    UpdateUndoBuffer( restoreBitmap );
    			// Starting point acquisition
          ScreenToCanvas( &point );
		    	m_ptFrom = m_ptTo = point;
          rectUpdate.SetRect( m_ptFrom, m_ptTo );
          rectUpdate.InflateRect( nPenSize, nPenSize );
          m_crInterfacePenColor = crPen;
          m_nInterfacePenSize = nPenSize;
          // Renewal of canvas
		    	DrawFrames(&rectUpdate);
			    DrawCanvas(NULL,&rectUpdate);
          m_bLine = true;
        } else {
          _EndLineStrip();
        }
        m_bButton = true; // YO pushed.
      }
      break;

	  //-------- Processing of a rectangle and a circle
//		case rect:
//		case fillRect:
//		case ellipse:
//		case fillEllipse:
/*		{
			// Starting point acquisition
			QuantizeForCanvas(&point);
			m_ptFrom = m_ptTo = point;
		}*/
		break;
		//-------- It is processing at the time of & range movement at the time of range selection.
		case select:
		case move:
		{
      // If a mouse is specification within the limits, they are range movement, otherwise, a range specification start.
      switch( m_iCopyMode ) {
      case SCALEFREE:
      case SCALEFIX:
        {
          if( (m_nMoveSubStat = (enum MOVESUBSTAT) _PickSelectionRect( m_rectCopyDst, 2, ptMouse)) != stretchout ) {
            // move
		    		// Starting point acquisition
			ScreenToCanvas(&point);
            m_ptFrom = point;

            // Frame
            CDC* pdc;
            OnPrepareDC( pdc = GetDC() );
            _DrawSelectionRect( pdc, m_rectCopyDst, 2 );
            ReleaseDC( pdc );
          }
          break;
        }
      case ROTATE:
      case FREE:
        {
          if( (m_nMoveSubStat = (enum MOVESUBSTAT) _PickSelectionArea( m_ptCopyDst, 2, ptMouse)) != stretchout ) {
            // move
		    		// Starting point acquisition
				    ScreenToCanvas(&point);
            m_ptFrom = m_ptTo = point;

            // Frame
/*            CDC* pdc;
            OnPrepareDC( pdc = GetDC() );
            _DrawSelectionArea( pdc, m_ptCopyDst, 2 );
            ReleaseDC( pdc );*/
          }
          break;
        }
		

      }
      
      if( m_nMoveSubStat == stretchout ) {
        // select
				m_nTool = select;
				// A certain selection range is already pasted.
        _EndCopy();
				// Starting point acquisition
				m_ptFrom = m_ptTo = ptMouse;
        m_rectCopyDst.SetRectEmpty();
        // Frame
        CDC* pdc;
        OnPrepareDC( pdc = GetDC() );
        m_rectCopySrc.SetRect( m_ptFrom, m_ptTo );
        m_rectCopySrc.NormalizeRect();
        _DrawSelectionRect( pdc, m_rectCopySrc, 2 );
        ReleaseDC( pdc );
			}
  		break;
		}
	}
}

//--------------------------------------------------------------------------
// Only the end of a copy is somewhat special.
//--------------------------------------------------------------------------
void CEasyToonView::_EndCopy()
{
  PasteSelectBuf();

  // 枠線 is erased.
/*
  CDC* pdc = GetDC();
  OnPrepareDC( pdc );
  _DrawSelectionRect( pdc, m_rectCopyDst, 2 );
  ReleaseDC( pdc );
*/  
  // There would be no selection.
	m_rectCopySrc.SetRectEmpty();
	m_rectCopyDst.SetRectEmpty();

  DrawFrames();
	DrawCanvas(NULL);
}

//--------------------------------------------------------------------------
// Only the end of a line strip is somewhat special.
//--------------------------------------------------------------------------
void CEasyToonView::_EndLineStrip()
{
  CDC*	pdcFrame = GetDocument()->GetFrameDC( m_nCurrentFrame );
	CPen	pen;
	CPen*	ppenOld;
  CRect rectUpdate;

  // Line drawing
  pen.CreatePen( PS_SOLID, m_nInterfacePenSize, m_crInterfacePenColor );
  ppenOld = pdcFrame->SelectObject( &pen );
 	pdcFrame->MoveTo( m_ptFrom );
  pdcFrame->LineTo( m_ptTo );
  pdcFrame->SelectObject( ppenOld );

  // Calculation of the updating range
  rectUpdate.SetRect( m_ptFrom, m_ptTo );
  rectUpdate.NormalizeRect();
  rectUpdate.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

  // Interface drawing is finished.
//  m_nInterfacePenSize = -1;

  // Renewal of canvas
 	DrawFrames(&rectUpdate);
  DrawCanvas(NULL,&rectUpdate);

  // Renewal of coordinates
  m_ptFrom= m_ptTo;
}

//--------------------------------------------------------------------------
//	Drawing continuation (at the time of mouse movement)
//--------------------------------------------------------------------------
void CEasyToonView::KeepDraw( CPoint point, int nBtn )
{
	CPoint ptMouse = TranslatePoint( point, FALSE );
  CRect rectUpdate;
  int nPenSize = nBtn ? m_nRPenSize : m_nPenSize;

	switch ( m_nTool )
	{
		//-------- Processing of a pen
		case pen:
		{
			CDC* pdc = GetDocument()->GetFrameDC(m_nCurrentFrame);
			// Calculation of the updating range
			CRect rc( pdc->GetCurrentPosition(), ptMouse );
			rc.NormalizeRect();


      rc.InflateRect( nPenSize, nPenSize );
	  // Drawing
			pdc->LineTo( ptMouse );
	
	  // Windows cannot strike a point.
      if( nPenSize == 1 ) {
        pdc->MoveTo( ptMouse.x, ptMouse.y );
		pdc->LineTo( ptMouse.x-1, ptMouse.y );
        pdc->MoveTo( ptMouse.x, ptMouse.y );
      }

      // Renewal of canvas
			DrawFrames(&rc);
			DrawCanvas(NULL,&rc);
		}
		break;
		//-------- Linear processing
		case line:
		case paraline:
		case rect:
    case fillRect:
		{
			// preparation

					
			
      rectUpdate.SetRect( m_ptFrom, m_ptTo );
      rectUpdate.NormalizeRect();
      rectUpdate.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

//      ScreenToCanvas( &point );
      m_ptTo = ptMouse;

	  CRect rect( m_ptFrom, m_ptTo );

	  if (m_nTool==paraline && m_bFirstUsePara==true)
	  {

		  if (m_paraline_coefdir!=0)
		  {
			  if (m_paraline_rect.Width()>=m_paraline_rect.Height())
			  {
				  m_ptTo.y=(m_ptTo.x*m_paraline_coefdir)+	m_paraline_B;
				  
			  }
			  else
			  {
				  m_ptTo.x= (m_ptTo.y-m_paraline_B)/m_paraline_coefdir;
			  }
		  }
		  else
		  {
			  if (constx==true)
			  {
				  m_ptTo.x=m_ptFrom.x;
			  }

			  if (consty==true)
			  {
				  m_ptTo.y=m_ptFrom.y;
			  }
		  }


	  }
	  
	  rect.SetRect(m_ptFrom, m_ptTo);
      rect.NormalizeRect();
      rect.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

      rectUpdate |= rect;

      // Something since 1 dot garbage comes out ...
      rectUpdate.InflateRect( 1, 1 );
      // Renewal of canvas
			DrawFrames(&rectUpdate);
			DrawCanvas(NULL,&rectUpdate);
		}
		break;

		case ellipse:
		case fillEllipse:
		{
			// Preparation
      CPoint pt;
      pt = m_ptFrom - m_ptTo;
      if( pt.x < 0 ) pt.x = -pt.x;
      if( pt.y < 0 ) pt.y = -pt.y;
  
      rectUpdate.top = m_ptFrom.y - pt.y;
      rectUpdate.bottom = m_ptFrom.y + pt.y;
      rectUpdate.left = m_ptFrom.x - pt.x;
      rectUpdate.right = m_ptFrom.x + pt.x;

      rectUpdate.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

      ScreenToCanvas( &point );
      m_ptTo = ptMouse;

      CRect rect;
      pt = m_ptFrom - m_ptTo;
      if( pt.x < 0 ) pt.x = -pt.x;
      if( pt.y < 0 ) pt.y = -pt.y;
  
      rect.top = m_ptFrom.y - pt.y;
      rect.bottom = m_ptFrom.y + pt.y;
      rect.left = m_ptFrom.x - pt.x;
      rect.right = m_ptFrom.x + pt.x;

	  if ((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
		{
			SquareRect(rect,m_ptFrom,m_ptTo);
		}

      rect.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

      rectUpdate |= rect;

      // Something   Since 1 dot garbage comes out ...
      rectUpdate.InflateRect( 1, 1 );
      // Renewal of canvas
			DrawFrames(&rectUpdate);
			DrawCanvas(NULL,&rectUpdate);
		}
		break;
    
    
    case linestrip:
		{
      if( m_bLine == false ) break;

      rectUpdate.SetRect( m_ptFrom, m_ptTo );
      rectUpdate.NormalizeRect();
      rectUpdate.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

      ScreenToCanvas( &point );
      m_ptTo = point;

      CRect rect( m_ptFrom, m_ptTo );
      rect.NormalizeRect();
      rect.InflateRect( m_nInterfacePenSize, m_nInterfacePenSize );

      rectUpdate |= rect;

      // Something   Since 1 dot garbage comes out ...
      rectUpdate.InflateRect( 1, 1 );
      // Renewal of canvas
			DrawFrames(&rectUpdate);
			DrawCanvas(NULL,&rectUpdate);
		}
    break;

		//-------- Processing of a rectangle and a circle
//		case fillRect:
//		case ellipse:
//		case fillEllipse:
/*		{
			// Processing
			CDC* pdc = GetDC();
			pdc->SaveDC();
			pdc->SetROP2( R2_XORPEN );
//			pdc->SelectStockObject( WHITE_PEN );
			pdc->SelectStockObject( NULL_BRUSH );

      // Pen creation
      CPen pen, *ppenOld;
      if( nBtn )
  			pen.CreatePen( PS_SOLID, m_nRPenSize*m_nZoomMagnify, 0xffffff );
      else
  			pen.CreatePen( PS_SOLID, m_nPenSize*m_nZoomMagnify, 0xffffff );

      ppenOld = pdc->SelectObject( &pen );
			// Frame drawing
			QuantizeForCanvas(&point);
			if ( m_nTool == rect || m_nTool == fillRect || m_nTool == select )
			{
				pdc->Rectangle( m_ptFrom.x, m_ptFrom.y, m_ptTo.x, m_ptTo.y );
				pdc->Rectangle( m_ptFrom.x, m_ptFrom.y, point.x, point.y );
			}
			else 
			{
				pdc->Ellipse( m_ptFrom.x, m_ptFrom.y, m_ptTo.x, m_ptTo.y );
        pdc->Ellipse( m_ptFrom.x, m_ptFrom.y, point.x, point.y );
			}
			m_ptTo = point;
      pdc->SelectObject( ppenOld );
			// End
			pdc->RestoreDC(-1);
			ReleaseDC(pdc);
		}
		break;
*/
    case select:
		{
			// Preparation

      CDC* pdc;
      OnPrepareDC( pdc = GetDC() );
			// Frame drawing
      CRect rect;
      rect.SetRect( m_ptFrom, m_ptTo );
      rect.NormalizeRect();
      _DrawSelectionRect( pdc, rect, 2 );      

      m_ptTo = ptMouse;
      rect.SetRect( m_ptFrom, m_ptTo );
      rect.NormalizeRect();
      _DrawSelectionRect( pdc, rect, 2 );      

      ReleaseDC(pdc);
		}
		break;

		// processing at the time of range movement
		case move:
		{
      switch( m_iCopyMode ) {
      case SCALEFREE:
      case SCALEFIX:
        {
          CRect rect;

          // An old rectangle is saved.
          rect = m_rectCopyDst;

          // Calculation of the amount of movements
	  		  ScreenToCanvas( &point, FALSE );
  		  	m_ptTo = CPoint( point - m_ptFrom );
          if( m_nMoveSubStat == stretchmove )
              m_rectCopyDst.OffsetRect( m_ptTo.x, m_ptTo.y );
  
         // Upper expansion and contraction
          if( (m_nMoveSubStat == stretchtop) || (m_nMoveSubStat == stretchlefttop) || (m_nMoveSubStat == stretchrighttop) ) {
            m_rectCopyDst.top  += m_ptTo.y;
            if( m_rectCopyDst.top >= m_rectCopyDst.bottom )
              m_rectCopyDst.top = m_rectCopyDst.bottom - 1;
            if( m_iCopyMode == SCALEFIX ) {
              int width = (m_rectCopySrc.Width()*m_rectCopyDst.Height())/m_rectCopySrc.Height();
              int center = (m_rectCopyDst.left + m_rectCopyDst.right)/2;            
              m_rectCopyDst.left = center - (width/2);
              m_rectCopyDst.right = center + (width/2);
            }
          }
  
          // lower expansion and contraction
          if( (m_nMoveSubStat == stretchbottom) || (m_nMoveSubStat == stretchleftbottom) || (m_nMoveSubStat == stretchrightbottom) ) {
            m_rectCopyDst.bottom += m_ptTo.y;
            if( m_rectCopyDst.top  >= m_rectCopyDst.bottom  )
              m_rectCopyDst.bottom = m_rectCopyDst.top + 1;
            if( m_iCopyMode == SCALEFIX ) {
              int width = (m_rectCopySrc.Width()*m_rectCopyDst.Height())/m_rectCopySrc.Height();
              int center = (m_rectCopyDst.left + m_rectCopyDst.right)/2;            
              m_rectCopyDst.left = center - (width/2);
              m_rectCopyDst.right = center + (width/2);
            }
          }

          // Left expansion and contraction
          if( (m_nMoveSubStat == stretchlefttop) || (m_nMoveSubStat == stretchleft) || (m_nMoveSubStat == stretchleftbottom) ) {
            m_rectCopyDst.left += m_ptTo.x;
            if( m_rectCopyDst.left >= m_rectCopyDst.right )
              m_rectCopyDst.left = m_rectCopyDst.right - 1;
            if( m_iCopyMode == SCALEFIX ) {
              int height = (m_rectCopySrc.Height()*m_rectCopyDst.Width())/m_rectCopySrc.Width();
              int center = (m_rectCopyDst.top + m_rectCopyDst.bottom)/2;            
              m_rectCopyDst.top = center - (height/2);
              m_rectCopyDst.bottom = center + (height/2);
            }
          }

          // right expansion and contraction
          if( (m_nMoveSubStat == stretchrighttop) || (m_nMoveSubStat == stretchright) || (m_nMoveSubStat == stretchrightbottom) ) {
            m_rectCopyDst.right += m_ptTo.x;
            if( m_rectCopyDst.left >= m_rectCopyDst.right )
              m_rectCopyDst.right = m_rectCopyDst.left + 1;
            if( m_iCopyMode == SCALEFIX ) {
              int height = (m_rectCopySrc.Height()*m_rectCopyDst.Width())/m_rectCopySrc.Width();
              int center = (m_rectCopyDst.top + m_rectCopyDst.bottom)/2;            
              m_rectCopyDst.top = center - (height/2);
              m_rectCopyDst.bottom = center + (height/2);
            }
          }

          // Composition
          rect |= m_rectCopyDst;
    
          m_ptFrom = point;
		  // It is since garbage comes out occasionally something.
          rect.InflateRect( 1, 1 );
          DrawFrames( &rect );
          DrawCanvas( NULL, &rect );
          break;
        }

      case ROTATE:
        {
          CDC* pdc;
          OnPrepareDC( pdc = GetDC() );
          // A front's is erased.
          _DrawSelectionArea( pdc, m_ptCopyDst, 2 );      
  
          // Calculation of the amount of movements
	    		ScreenToCanvas( &point, FALSE );
 		    	m_ptTo = CPoint( point - m_ptFrom );

          switch( m_nMoveSubStat ) {
          case stretchlefttop:
          case stretchrighttop:
          case stretchrightbottom:
          case stretchleftbottom:
            {
              float prev[2],current[2],length;
              prev[0] = (float)(m_ptFrom.x - m_ptCopyCenterDst.x);
              prev[1] = (float)(m_ptFrom.y - m_ptCopyCenterDst.y);
              length = ::sqrtf( prev[0]*prev[0] + prev[1]*prev[1] );
              prev[0] /= length;
              prev[1] /= length; // Regular-izing
              length = prev[0];
              prev[0] = -prev[1];
			  prev[1] = length; // Rotation

              current[0] = (float)(point.x - m_ptCopyCenterDst.x);
              current[1] = (float)(point.y - m_ptCopyCenterDst.y);
              length = ::sqrtf( current[0]*current[0] + current[1]*current[1] );
              current[0] /= length;
              current[1] /= length; // I want a vector calculation library.
              
              m_fRotate += (3.14f/2.0f) - ::acosf( prev[0]*current[0] + prev[1]*current[1] );
            }
            break;

          case stretchmove:
            m_ptCopyCenterDst += m_ptTo;
            break;
          }
          m_ptFrom = point;

          // It moves to a center.
          float x[4], y[4];
          x[0] = (float)(m_rectCopySrc.left - m_ptCopyCenterSrc.x);
          y[0] = (float)(m_rectCopySrc.top - m_ptCopyCenterSrc.y);
          x[1] = (float)(m_rectCopySrc.right - m_ptCopyCenterSrc.x);
          y[1] = (float)(m_rectCopySrc.top - m_ptCopyCenterSrc.y);
          x[2] = (float)(m_rectCopySrc.right - m_ptCopyCenterSrc.x);
          y[2] = (float)(m_rectCopySrc.bottom - m_ptCopyCenterSrc.y);
          x[3] = (float)(m_rectCopySrc.left - m_ptCopyCenterSrc.x);
          y[3] = (float)(m_rectCopySrc.bottom - m_ptCopyCenterSrc.y);

          // Rotation and movement
          int i;
          for( i = 0 ; i < 4 ; i++ ) {
            m_ptCopyDst[i].x = (int)(x[i]*::cosf(m_fRotate) - y[i]*::sinf(m_fRotate)) + m_ptCopyCenterDst.x;
            m_ptCopyDst[i].y = (int)(x[i]*::sinf(m_fRotate) + y[i]*::cosf(m_fRotate)) + m_ptCopyCenterDst.y;
          }

          // New one is taken out.
          _DrawSelectionArea( pdc, m_ptCopyDst, 2 );      
          ReleaseDC(pdc);
        }
        break;

      case FREE:
        {
          CDC* pdc;
          OnPrepareDC( pdc = GetDC() );
		  // A front's is erased.
          _DrawSelectionArea( pdc, m_ptCopyDst, 2 );      
  
		  // Calculation of the amount of movements
	    		ScreenToCanvas( &point, FALSE );
		    	m_ptTo = CPoint( point - m_ptFrom );
          m_ptFrom = point;

          switch( m_nMoveSubStat ) {
          case stretchlefttop:
            m_ptCopyDst[0] = ptMouse; break;
          case stretchrighttop:
            m_ptCopyDst[1] = ptMouse; break;
          case stretchrightbottom:
            m_ptCopyDst[2] = ptMouse; break;
          case stretchleftbottom:
            m_ptCopyDst[3] = ptMouse; break;
          case stretchmove:
            m_ptCopyDst[0] += m_ptTo;
            m_ptCopyDst[1] += m_ptTo;
            m_ptCopyDst[2] += m_ptTo;
            m_ptCopyDst[3] += m_ptTo;
            break;
          }

          // New one is taken out.
          _DrawSelectionArea( pdc, m_ptCopyDst, 2 );      
  
          ReleaseDC(pdc);
          break;
        }
      }
		}
		break;
	}
}

//--------------------------------------------------------------------------
//	Drawing end
//--------------------------------------------------------------------------
void CEasyToonView::EndDraw ( CPoint point, int nBtn )
{
	COLORREF crPen		= nBtn ? m_crRightPen : m_crLeftPen;
	CDC*	 pdcFrame	= GetDocument()->GetFrameDC( m_nCurrentFrame );
  int nPenSize = nBtn ? m_nRPenSize : m_nPenSize;

	switch ( m_nTool )
	{
		//-------- Processing of a pen
		case pen:
		{
			pdcFrame->RestoreDC(-1);
			m_penDraw.DeleteObject();
		}
		break;
		//-------- Processing of a line
		case line:case paraline:
		{
			CPen	pen;
			CPen*	ppenOld;
			// Renewal of a UNDO buffer
			UpdateUndoBuffer( restoreBitmap );

            // drawing
			// pen creation

  			pen.CreatePen( PS_SOLID, nPenSize, crPen );
      
			ppenOld = pdcFrame->SelectObject( &pen );
			
			if (m_nTool==paraline && m_bFirstUsePara==false)
			{
				if ((m_ptTo.x != m_ptFrom.x) && (m_ptTo.y!=m_ptFrom.y))
				{

					m_paraline_coefdir=((double)((m_ptTo.y - m_ptFrom.y)/(double)(m_ptTo.x - m_ptFrom.x)));	
					
				}
				else
				{
					m_paraline_coefdir=0;
				}

				if ((m_ptTo.x - m_ptFrom.x)==0)
				{
					constx=true;
				}

				if ((m_ptTo.y-m_ptFrom.y)==0)
				{
					consty=true;
				}
					m_bFirstUsePara=true;

					//Variable m_paraline_rect de type CRect
					//Utilisée pour déterminer quelle méthode utiliser pour le							//calcule des coordonnées dans KeepDraw
					m_paraline_rect.SetRect(m_ptFrom,m_ptTo);
					m_paraline_rect.NormalizeRect();
			}
			
			//Remet à zéro l'ordonnée à l'origine
			m_paraline_B=0;
			
			pdcFrame->MoveTo( m_ptFrom );
			pdcFrame->LineTo( m_ptTo );
			
      // Something   こ - Since a point is not struck to the place of LineTo, carry out.
      if( m_nInterfacePenSize == 1 ) {
        pdcFrame->LineTo( m_ptTo.x - 1, m_ptTo.y );
      }

			pdcFrame->SelectObject( ppenOld );

      // Calculation of the updating range
			CRect rc( m_ptFrom, m_ptTo );
			rc.NormalizeRect();
      rc.InflateRect( nPenSize, nPenSize );

      m_nInterfacePenSize = -1; // A pen is not drawn.
      // renewal of canvas

			
			DrawFrames(&rc);
			DrawCanvas(NULL,&rc);
		}
		break;
		
		//-------- Processing of a rectangle and a circle
		case rect:
		case fillRect:
      {
        if( m_nInterfacePenSize <= 0 ) return; 
        // Pen creation
        CPen pen;
	      pen.CreatePen( PS_SOLID, m_nInterfacePenSize, m_crInterfacePenColor );
        CBrush brush( RGB(m_nBrush*16,m_nBrush*16,m_nBrush*16));

        pdcFrame->SaveDC();
        pdcFrame->SelectObject( &pen );
        if( m_nTool == fillRect ) {
          pdcFrame->SelectObject( &brush );
          pdcFrame->SetTextColor( m_crInterfacePenColor );
          pdcFrame->SetBkColor( ~m_crInterfacePenColor );
        } else
          pdcFrame->SelectStockObject( NULL_BRUSH );

        // Rectangle drawing
        
		CRect rc(m_ptFrom,m_ptTo);
		
        rc.NormalizeRect();

 		pdcFrame->Rectangle( rc ); 

        // End
        pdcFrame->RestoreDC( -1 );
        m_nInterfacePenSize = -1; // A rectangle is not drawn.

        // renewal of canvas
			  DrawFrames();
			  DrawCanvas(NULL);
        break;
      }

    case ellipse:
		case fillEllipse:
      {
        if( m_nInterfacePenSize <= 0 ) return; 
        // Pen creation
        CPen pen;
	     pen.CreatePen( PS_SOLID, m_nInterfacePenSize, m_crInterfacePenColor );
		 CBrush brush( RGB(m_nBrush*16,m_nBrush*16,m_nBrush*16));

        pdcFrame->SaveDC();
        pdcFrame->SelectObject( &pen );
        if( m_nTool == fillEllipse ) {
          pdcFrame->SelectObject( &brush );
          pdcFrame->SetTextColor( m_crInterfacePenColor );
          pdcFrame->SetBkColor( ~m_crInterfacePenColor );
        } else
          pdcFrame->SelectStockObject( NULL_BRUSH );

        // Rectangle drawing
        CPoint pt;
        CRect rc;
        pt = m_ptFrom - m_ptTo;
        if( pt.x < 0 ) pt.x = -pt.x;
        if( pt.y < 0 ) pt.y = -pt.y;
  
		rc.top = m_ptFrom.y - pt.y;
		rc.bottom = m_ptFrom.y + pt.y;
		rc.left = m_ptFrom.x - pt.x;
		rc.right = m_ptFrom.x + pt.x;
		
		int debi=GetKeyState(VK_SHIFT);
		CString deb;
		deb.Format("%d",debi);
	AfxOutputDebugString(deb);
		if ((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
		{
			SquareRect(rc,m_ptFrom,m_ptTo);
		}

	
  rc.top = m_ptFrom.y - pt.y;
  rc.bottom = m_ptFrom.y + pt.y;
  rc.left = m_ptFrom.x - pt.x;
  rc.right = m_ptFrom.x + pt.x;
       

	  	pdcFrame->Ellipse ( rc );

		// end

        pdcFrame->RestoreDC( -1 );
        m_nInterfacePenSize = -1; // rectangle is not drawn.

        // renewal of canvas
			  DrawFrames();
			  DrawCanvas(NULL);
        break;
      }
/*
		case ellipse:
//		case fillRect:
		case fillEllipse:
		{
			CPen	pen;
//			CBrush	brush(crPen);
			CBrush	brush( m_pbmpPattern );
			// Renewal of a UNDO buffer
			UpdateUndoBuffer( restoreBitmap );
			// Coordinates conversion
			ScreenToCanvas( &m_ptFrom );
			ScreenToCanvas( &m_ptTo );
			CRect rc( m_ptFrom, m_ptTo );
			// Drawing
      // Pen creation
      if( nBtn )
  			pen.CreatePen( PS_SOLID, m_nRPenSize, crPen );
      else
  			pen.CreatePen( PS_SOLID, m_nPenSize, crPen );
			pdcFrame->SaveDC();
			pdcFrame->SelectObject( &pen );
      pdcFrame->SetTextColor( crPen );
      pdcFrame->SetBkColor( ~crPen );
			if ( m_nTool == fillRect || m_nTool == fillEllipse )
				pdcFrame->SelectObject ( &brush );
			else
				pdcFrame->SelectStockObject( NULL_BRUSH );
			if ( m_nTool == rect || m_nTool == fillRect ) 
				pdcFrame->Rectangle( rc ); 
			else 
				pdcFrame->Ellipse ( rc );
			pdcFrame->RestoreDC(-1);
			// Calculation of the updating range
			rc.NormalizeRect();
      if( nBtn )
  			rc.InflateRect( m_nRPenSize, m_nRPenSize );
      else
  			rc.InflateRect( m_nPenSize, m_nPenSize );
			// Renewal of canvas
			DrawFrames(&rc);
			DrawCanvas(NULL,&rc);
		}
		break;
*/
		//-------- Processing at the time of range selection
		case select:
		{
			// Renewal of a UNDO buffer
			UpdateUndoBuffer( restoreBitmap );

      // Coordinates conversion
      m_rectCopySrc.SetRect( m_ptFrom.x, m_ptFrom.y, m_ptTo.x, m_ptTo.y );
      m_rectCopySrc.NormalizeRect();
      m_rectCopyDst = m_rectCopySrc;
      m_ptCopyDst[0].x = m_rectCopySrc.left;
      m_ptCopyDst[0].y = m_rectCopySrc.top;
      m_ptCopyDst[1].x = m_rectCopySrc.right;
      m_ptCopyDst[1].y = m_rectCopySrc.top;
      m_ptCopyDst[2].x = m_rectCopySrc.right;
      m_ptCopyDst[2].y = m_rectCopySrc.bottom;
      m_ptCopyDst[3].x = m_rectCopySrc.left;
      m_ptCopyDst[3].y = m_rectCopySrc.bottom;
      m_ptCopyCenterSrc = m_ptCopyCenterDst = m_rectCopySrc.CenterPoint();
      m_fRotate = 0.0f;
			
      // ‘I‘ð”ÍˆÍ‚ª‚d‚‚‚”‚™‚È‚ç‚Î”²‚¯‚é
      if ( m_rectCopySrc.IsRectEmpty() ) {
        
        break;
      }

	  // It will escape, if the selection range becomes Empty.
      _CurrentFrameToCopyBuffer( m_rectCopySrc );

	  // The selection range is copied to a selection buffer.
    	CDC* pdcFrame = GetDocument()->GetFrameDC( m_nCurrentFrame );
      pdcFrame->SaveDC();
	    pdcFrame->SelectStockObject( WHITE_PEN );
	    pdcFrame->SelectStockObject( WHITE_BRUSH );
	    pdcFrame->Rectangle( &m_rectCopySrc );
      pdcFrame->RestoreDC( -1 );

      // It shifts to selection range move mode.
			m_nTool = move;

      CRect rect;

      if( ((m_iCopyMode == ROTATE) || (m_iCopyMode == FREE) ) ) {
		  // A frame is erased.
        CDC* pdc;
        OnPrepareDC( pdc = GetDC() );
        _DrawSelectionRect( pdc, m_rectCopyDst, 2 );

        //A frame is taken out.
        _DrawSelectionArea( pdc, m_ptCopyDst, 2 );
        ReleaseDC( pdc );
      }
		}
		break;

	// Processing at the time of range movement

		case move:
		{
			// Range movement
      switch( m_iCopyMode ) {
      case SCALEFREE:
      case SCALEFIX:
        {
          // A frame is taken out.
    			CDC* pdc;
          OnPrepareDC( pdc = GetDC() );
          _DrawSelectionRect( pdc, m_rectCopyDst, 2 );
          ReleaseDC( pdc );
        }
        break;

      case ROTATE:
      case FREE:
        {
          m_nMoveSubStat = stretchview;
     			// Renewal of canvas
    			DrawFrames();
		    	DrawCanvas(NULL);
          
          // A frame is taken out.
          CDC* pdc;
          OnPrepareDC( pdc = GetDC() );
          _DrawSelectionArea( pdc, m_ptCopyDst, 2 );
          ReleaseDC( pdc );

        }
      }
		}
		break;
	}
}

//--------------------------------------------------------------------------
//	•`‰æ’†Ž~ (“ñdButtonDown)
//--------------------------------------------------------------------------
void CEasyToonView::StopDraw()
{
	switch ( m_nTool )
	{
	// ƒyƒ“‚Ìˆ—
	case pen:
		GetDocument()->GetFrameDC( m_nCurrentFrame )->RestoreDC(-1);
		m_penDraw.DeleteObject();
		break;
	// ”ÍˆÍˆÚ“®Žž‚Ìˆ—
	case move:
		m_ptTo = CPoint( 0, 0 );
		break;
	}
	DrawFrames();
	DrawCanvas( NULL );
}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	ƒ}ƒEƒXƒnƒ“ƒhƒ‰ŒQ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	¶ƒ{ƒ^ƒ“‰Ÿ‰º
//--------------------------------------------------------------------------
void CEasyToonView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ( GetCapture() )
	{
    if( m_nTool == linestrip ) {
  		BeginDraw(point,0);
	  	KeepDraw(point,0);

	} else if (m_nTool==Text){}
	else{
  		StopDraw();
	  	ReleaseCapture();
    }
	}
	else
	{
		BeginDraw(point,0);
		KeepDraw(point,0);
		SetCapture();
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

//--------------------------------------------------------------------------
//	‰Eƒ{ƒ^ƒ“‰Ÿ‰º
//--------------------------------------------------------------------------
void CEasyToonView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if ( GetCapture() )
	{
    if( m_nTool == linestrip ) {
  		BeginDraw(point,1);
	  	KeepDraw(point,1);
    } else {
  		StopDraw();
	  	ReleaseCapture();
    }
	}
	else
	{
		BeginDraw(point,1);
		KeepDraw(point,1);
		SetCapture();
	}
	CScrollView::OnRButtonDown(nFlags, point);
}

//--------------------------------------------------------------------------
//	ƒ}ƒEƒXˆÚ“®
//--------------------------------------------------------------------------
void CEasyToonView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString Pos;
	CString x;
	CString y;
	CPoint LocalPoint;

	SetCursor(LoadCursorFromFile("..\res\stylo.cur"));

	LocalPoint=TranslatePoint(point,FALSE);
	if (LocalPoint.x>m_sizeFrame.cx)
	{
		LocalPoint.x=m_sizeFrame.cx;
	}

	if (LocalPoint.y>m_sizeFrame.cy)
	{
		LocalPoint.y=m_sizeFrame.cy;
	}

	x.Format("%d",LocalPoint.x);
	y.Format("%d",LocalPoint.y);
	Pos += x+ ',' + y;

	GetMainFrame()->SetStatusText(0,Pos);

  if ( GetCapture() )
    KeepDraw( point, nFlags & MK_RBUTTON ); 

//if(m_nTool==pen)
//{
//	ShowCursor(false);
//	AfxGetApp()->LoadStandardCursor(IDC_CROSS);
//	ShowCursor(true);
//}
  CScrollView::OnMouseMove(nFlags, point);
}

//--------------------------------------------------------------------------
//	¶ƒ{ƒ^ƒ“‰ðœ
//--------------------------------------------------------------------------
void CEasyToonView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ( GetCapture() )
	{
    if( m_nTool == linestrip ) {
      m_bButton = false;
    } else {
  		EndDraw( point, 0 );
	  	ReleaseCapture();
		  GetDocument()->SetModifiedFlag();
    }
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

//--------------------------------------------------------------------------
//	‰Eƒ{ƒ^ƒ“‰ðœ
//--------------------------------------------------------------------------
void CEasyToonView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if ( GetCapture() )
	{
    if( m_nTool == linestrip ) {
      m_bButton = false;
    } else {
  		EndDraw( point, 1 );
	  	ReleaseCapture();
		  GetDocument()->SetModifiedFlag();
    }
	}
	CScrollView::OnRButtonUp(nFlags, point);
}

//--------------------------------------------------------------------------
//	¶ƒ{ƒ^ƒ“ƒ_ƒuƒ‹ƒNƒŠƒbƒN
//--------------------------------------------------------------------------
void CEasyToonView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ( GetCapture() )
	{
 		EndDraw( point, 0 );
  	ReleaseCapture();
	  GetDocument()->SetModifiedFlag();
    if( m_nTool == linestrip ) {
      m_bLine = false;
      _EndLineStrip();
      m_nInterfacePenSize = -1;
    }
	}
	CScrollView::OnLButtonDblClk(nFlags, point);
}

//--------------------------------------------------------------------------
//	‰Eƒ{ƒ^ƒ“ƒ_ƒuƒ‹ƒNƒŠƒbƒN
//--------------------------------------------------------------------------
void CEasyToonView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if ( GetCapture() )
	{
 		EndDraw( point, 1 );
  	ReleaseCapture();
	  GetDocument()->SetModifiedFlag();
    if( m_nTool == linestrip ) {
      m_bLine = false;
      _EndLineStrip();
      m_nInterfacePenSize = -1;
    }
	}
	CScrollView::OnRButtonDblClk(nFlags, point);
}



//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	ƒtƒŒ[ƒ€‘€ì
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	"©"ƒ{ƒ^ƒ“
//--------------------------------------------------------------------------
void CEasyToonView::OnFramePrev() 
{
	if ( m_nCurrentFrame > 0 )
	{
		// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
		if ( m_nTool == move ) 
		{
      _EndCopy();
//			PasteSelectBuf();
//			m_rectSelect.SetRectEmpty();
		}
		// ƒtƒŒ[ƒ€‚ð–ß‚·
		m_nCurrentFrame--;
		// ‰æ–ÊXV
		GetMainFrame()->SetFrameDelay( GetDocument()->GetFrameDelay(m_nCurrentFrame) );
    _DrawBackFrame();
		DrawFrames();
		DrawCanvas(NULL);
	}
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}
// ƒJƒŒƒ“ƒgƒtƒŒ[ƒ€‚ª‚O‚ÌŽž‚Í©ƒ{ƒ^ƒ“‚ÍŽg—p•s‰Â
void CEasyToonView::OnUpdateFramePrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_nCurrentFrame > 0 );
}

//--------------------------------------------------------------------------
//	"¨"ƒ{ƒ^ƒ“
//--------------------------------------------------------------------------
void CEasyToonView::OnFrameNext() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
	{
    _EndCopy();
//		PasteSelectBuf();
//		m_rectSelect.SetRectEmpty();
	}
	// ƒtƒŒ[ƒ€‚ði‚ß‚éA–”‚ÍAV‹KƒtƒŒ[ƒ€‚Ì’Ç‰Á
	m_nCurrentFrame++;
  if ( m_nCurrentFrame >= pDoc->GetCount() ) {
    pDoc->AppendNewFrame();
    GetMainFrame()->SetPlayLimit( pDoc->GetCount() );
  }
	// ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );

  _DrawBackFrame();
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}

//--------------------------------------------------------------------------
//	‘}“üƒ{ƒ^ƒ“
//--------------------------------------------------------------------------
void CEasyToonView::OnFrameInsert() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
	{
    _EndCopy();
//		PasteSelectBuf();
//		m_rectSelect.SetRectEmpty();
	}
	// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
	UpdateUndoBuffer( deleteFrame );
	// ƒtƒŒ[ƒ€‚Ì‘}“ü
	pDoc->InsertNewFrame( m_nCurrentFrame );

  GetMainFrame()->SetPlayLimit( pDoc->GetCount() );
	// ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );

  _DrawBackFrame();
  DrawFrames();
	DrawCanvas(NULL);
}


//--------------------------------------------------------------------------
//	íœƒ{ƒ^ƒ“
//--------------------------------------------------------------------------
void CEasyToonView::OnFrameDelete() 
{
	// Šm”F
	if ( MessageBox("‚±‚ÌÌÚ°Ñ‚ðíœ‚µ‚Ü‚·B‚æ‚ë‚µ‚¢‚Å‚·‚©H","ÌÚ°Ñíœ",MB_OKCANCEL) != IDOK ) return;

	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
	{
    _EndCopy();
    //		PasteSelectBuf();
//		m_rectSelect.SetRectEmpty();
	}
	// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
	UpdateUndoBuffer ( restoreFrame );
	// ƒtƒŒ[ƒ€íœ
	pDoc->DeleteFrame(m_nCurrentFrame);
  if ( pDoc->GetCount() == 0 ) {
    pDoc->AppendNewFrame();
    GetMainFrame()->SetPlayLimit( pDoc->GetCount() );
  }
	if ( pDoc->GetCount() == m_nCurrentFrame ) m_nCurrentFrame--;
	// ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay ( pDoc->GetFrameDelay( m_nCurrentFrame ) );

  _DrawBackFrame();
	DrawFrames();
	DrawCanvas(NULL);
}


//--------------------------------------------------------------------------
//	ƒNƒŠƒAƒ{ƒ^ƒ“
//--------------------------------------------------------------------------
void CEasyToonView::OnFrameClear() 
{
	// Šm”F
	if ( MessageBox("‚±‚ÌÌÚ°Ñ‚ð¸Ø±‚µ‚Ü‚·B‚æ‚ë‚µ‚¢‚Å‚·‚©H","ÌÚ°Ñ¸Ø±",MB_OKCANCEL) != IDOK ) return;

	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
	{
    _EndCopy();
//		PasteSelectBuf();
//		m_rectSelect.SetRectEmpty();
	}
	// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
	UpdateUndoBuffer( restoreBitmap );
	// íœ‚µ‚ÄAV‹K‘}“ü
	pDoc->DeleteFrame(m_nCurrentFrame);
	if ( m_nCurrentFrame == pDoc->GetCount() ) pDoc->AppendNewFrame(); else pDoc->InsertNewFrame(m_nCurrentFrame);
	// ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );

  _DrawBackFrame();
  DrawFrames();
	DrawCanvas(NULL);
}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	ƒJƒbƒg•ƒy[ƒXƒg
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ƒJƒŒƒ“ƒgƒtƒŒ[ƒ€‚ÌŽw’è—Ìˆæ‚ðƒNƒŠƒbƒvƒ{[ƒh‚Ö CF_DIB ‚ÅƒRƒs[
// 1 bit DIB ‚¾‚Æ‚¤‚Ü‚­ƒy[ƒXƒg‚Å‚«‚È‚¢ƒAƒvƒŠ(PS)‚Ì‚½‚ß‚É
// 24 bit DIB ‚É•ÏŠ·‚·‚éB
//--------------------------------------------------------------------------
void CEasyToonView::_CurrentFrameToClipboard(CRect rect)
{
  // ‹ó
  if( rect.IsRectEmpty() != false ) return;
  rect.NormalizeRect();

  // ƒRƒs[Œ³‚ÌƒsƒNƒZƒ‹‚ðŠi”[‚·‚é—Ìˆæ‚ðì¬
  BYTE *pSrc;
  int nbSrcBytes = (m_sizeFrame.cx >> 3) + (m_sizeFrame.cx & 0x7 ? 1 : 0); // ƒ‰ƒXƒ^ƒTƒCƒY
  nbSrcBytes += (2-(nbSrcBytes & 0x1)) & 0x1; // WORD ƒoƒEƒ“ƒ_ƒŠ
  nbSrcBytes *= m_sizeFrame.cy;
  pSrc = new BYTE[nbSrcBytes];
  // ƒRƒs[Œ³ƒsƒNƒZƒ‹Žæ“¾
  GetDocument()->GetFrameBmp( m_nCurrentFrame )->GetBitmapBits( nbSrcBytes, (LPVOID) pSrc );

  // ƒRƒs[æ‚ÌƒTƒCƒY
  int nbDstBytes = rect.Width()*3; // ƒ‰ƒXƒ^ƒTƒCƒY
  nbDstBytes += (4-(nbDstBytes & 0x3)) & 0x3; // DWORD ƒoƒEƒ“ƒ_ƒŠ
  nbDstBytes *= rect.Height();

  // ƒƒ‚ƒŠŠm•Û
  HGLOBAL hCopy = ::GlobalAlloc( GHND, sizeof(BITMAPINFOHEADER)+nbDstBytes );
  BYTE* dst = (BYTE*) ::GlobalLock ( hCopy );

  // ƒwƒbƒ_ì¬
  BITMAPINFOHEADER *pBmpInfoHeader = (BITMAPINFOHEADER*) dst;
  pBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
  pBmpInfoHeader->biWidth = rect.Width();
  pBmpInfoHeader->biHeight = rect.Height();
  pBmpInfoHeader->biPlanes = 1;
  pBmpInfoHeader->biBitCount = 24;
  pBmpInfoHeader->biCompression = BI_RGB;    // –³ˆ³k
  pBmpInfoHeader->biSizeImage = nbDstBytes;
  pBmpInfoHeader->biXPelsPerMeter = 0xec4; // ‚È‚ñ‚©ƒyƒCƒ“ƒg‚Åo—Í‚µ‚½‚ç‚±‚ñ‚È‚ñ‚È‚Á‚Ä‚½B
  pBmpInfoHeader->biYPelsPerMeter = 0xec4;
  pBmpInfoHeader->biClrUsed = 0;
  pBmpInfoHeader->biClrImportant = 0;

  // ƒsƒNƒZƒ‹‘‚«ž‚Ý
  int x, y;
  for( y = 0 ; y < rect.Height() ; y++ ) {
    for( x = 0 ; x < rect.Width() ; x++ ) {
      if( CEasyToonDoc::_GetPixelColor1( pSrc, m_sizeFrame.cx, m_sizeFrame.cy, rect.left+x, rect.top+y ) == 1 ) {
        CEasyToonDoc::_SetPixelColor24( dst + sizeof(BITMAPINFOHEADER), rect.Width(), rect.Height(), x, rect.Height() - y - 1, 0xffffff );
      } else {
        CEasyToonDoc::_SetPixelColor24( dst + sizeof(BITMAPINFOHEADER), rect.Width(), rect.Height(), x, rect.Height() - y - 1, 0x000000 );
      }
    }
  }

  ::GlobalUnlock( hCopy );

  delete [] pSrc;

  // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒvƒ“
  if( OpenClipboard () == 0 ) return; // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒvƒ“Ž¸”s
  // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒi[‚É‚È‚é
  if( EmptyClipboard () == 0 ) return;  // ƒI[ƒi[‚É‚È‚ê‚¸
  
  // ƒNƒŠƒbƒvƒ{[ƒh‚Öƒf[ƒ^‚ðÝ’è
  if( ::SetClipboardData( CF_DIB, (HANDLE) hCopy ) == NULL )  {
    // Ž¸”s‚µ‚½‚Æ‚«
    CloseClipboard();
    return;
  }
  
  // ¬Œ÷
  CloseClipboard();
}



void CEasyToonView::_ClipboardToCopyBuffer()
{
  m_dcCopySrc.RestoreDC( -1 );
  
  // ƒy[ƒXƒgƒoƒbƒtƒ@‚ÌƒNƒŠƒA
  if( m_pbmpCopySrc != NULL ) delete m_pbmpCopySrc;
  m_pbmpCopySrc = NULL;

  // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒvƒ“
  try {
    if( OpenClipboard () == 0 ) throw( 1 );
    HANDLE handle = ::GetClipboardData ( CF_DIB );
    if ( handle == NULL ) throw( 2 );

    // ƒf[ƒ^Žæ“¾
    BYTE *pClipboardData = (BYTE*) ::GlobalLock ( handle );

    BITMAPINFOHEADER *pBmpInfoHeader = (BITMAPINFOHEADER*) pClipboardData;
    if( pBmpInfoHeader->biBitCount != 24 ) throw( 3 );

    // BMP ‚ð‰Šú‰»‚·‚éƒsƒNƒZƒ‹‚ðŠi”[‚·‚é—Ìˆæ‚ðì¬
    BYTE *pInit;
    int nbSrcBytes = (pBmpInfoHeader->biWidth >> 3) + (pBmpInfoHeader->biWidth & 0x7 ? 1 : 0); // ƒ‰ƒXƒ^ƒTƒCƒY
    nbSrcBytes += (2-(nbSrcBytes & 0x1)) & 0x1; // WORD ƒoƒEƒ“ƒ_ƒŠ
    nbSrcBytes *= pBmpInfoHeader->biHeight;
    pInit = new BYTE[nbSrcBytes];
    
    // 24 ¨ 1 •ÏŠ·
    int x, y;
    for( y = 0 ; y < pBmpInfoHeader->biHeight ; y++ ) {
      for( x = 0 ; x < pBmpInfoHeader->biWidth ; x++ ) {
        if( CEasyToonDoc::_GetPixelColor24( pClipboardData + sizeof(BITMAPINFOHEADER),
             pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight, x, y ) != 0 ) {
          CEasyToonDoc::_SetPixelColor1( pInit, pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight,
                           x, pBmpInfoHeader->biHeight - y - 1, 1 );
        } else {
          CEasyToonDoc::_SetPixelColor1( pInit, pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight,
                           x, pBmpInfoHeader->biHeight - y - 1, 0 );
        }
      }
    }

    // bmp ì¬
    m_pbmpCopySrc = new CBitmap;
  	m_pbmpCopySrc->CreateBitmap( pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight, 1,  1, pInit );

    m_rectCopySrc.SetRect( 0, 0, pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight );
    m_rectCopyDst = m_rectCopySrc;
   
    // Ž©—R•ÏŒ`‚È‚Æ‚«
    m_ptCopyDst[0].x = m_rectCopySrc.left;
    m_ptCopyDst[0].y = m_rectCopySrc.top;
    m_ptCopyDst[1].x = m_rectCopySrc.right;
    m_ptCopyDst[1].y = m_rectCopySrc.top;
    m_ptCopyDst[2].x = m_rectCopySrc.right;
    m_ptCopyDst[2].y = m_rectCopySrc.bottom;
    m_ptCopyDst[3].x = m_rectCopySrc.left;
    m_ptCopyDst[3].y = m_rectCopySrc.bottom;

    m_ptCopyCenterSrc = m_ptCopyCenterDst = m_rectCopySrc.CenterPoint();
    m_fRotate = 0.0f;

    delete [] pInit;
    ::GlobalUnlock( handle );
  } catch( int nError ) {
    switch( nError ) {
    case 1: MessageBox( "ƒI[ƒvƒ“‚ÉŽ¸”s" ); break;
    case 2: MessageBox( "ƒf[ƒ^Žæ“¾‚ÉŽ¸”s" ); break;
    case 3: MessageBox( "24 ƒrƒbƒg DIB ‚Å‚È‚¢" ); break;
    default: MessageBox( "‚È‚ñ‚©ƒGƒ‰[" ); break;
    }
  }
  CloseClipboard ();


  m_dcCopySrc.SaveDC();

  if( m_pbmpCopySrc != NULL )
    m_dcCopySrc.SelectObject( m_pbmpCopySrc );
}

void CEasyToonView::_CopyBufferToClipboard()
{
  // ‹ó
  if( m_rectCopySrc.IsRectEmpty() != false ) return;
//  rect.NormalizeRect();

  // ƒRƒs[Œ³‚ÌƒsƒNƒZƒ‹‚ðŠi”[‚·‚é—Ìˆæ‚ðì¬
  BYTE *pSrc;
  int nbSrcBytes = (m_rectCopySrc.Width() >> 3) + (m_rectCopySrc.Width() & 0x7 ? 1 : 0); // ƒ‰ƒXƒ^ƒTƒCƒY
  nbSrcBytes += (2-(nbSrcBytes & 0x1)) & 0x1; // WORD ƒoƒEƒ“ƒ_ƒŠ
  nbSrcBytes *= m_rectCopySrc.Height();
  pSrc = new BYTE[nbSrcBytes];
  // ƒRƒs[Œ³ƒsƒNƒZƒ‹Žæ“¾
  m_pbmpCopySrc->GetBitmapBits( nbSrcBytes, (LPVOID) pSrc );

  // ƒRƒs[æ‚ÌƒTƒCƒY
  int nbDstBytes = m_rectCopySrc.Width()*3; // ƒ‰ƒXƒ^ƒTƒCƒY
  nbDstBytes += (4-(nbDstBytes & 0x3)) & 0x3; // DWORD ƒoƒEƒ“ƒ_ƒŠ
  nbDstBytes *= m_rectCopySrc.Height();

  // ƒƒ‚ƒŠŠm•Û
  HGLOBAL hCopy = ::GlobalAlloc( GHND, sizeof(BITMAPINFOHEADER)+nbDstBytes );
  BYTE* dst = (BYTE*) ::GlobalLock ( hCopy );

  // ƒwƒbƒ_ì¬
  BITMAPINFOHEADER *pBmpInfoHeader = (BITMAPINFOHEADER*) dst;
  pBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
  pBmpInfoHeader->biWidth  = m_rectCopySrc.Width();
  pBmpInfoHeader->biHeight = m_rectCopySrc.Height();
  pBmpInfoHeader->biPlanes = 1;
  pBmpInfoHeader->biBitCount = 24;
  pBmpInfoHeader->biCompression = BI_RGB;    // –³ˆ³k
  pBmpInfoHeader->biSizeImage = nbDstBytes;
  pBmpInfoHeader->biXPelsPerMeter = 0xec4; // ‚È‚ñ‚©ƒyƒCƒ“ƒg‚Åo—Í‚µ‚½‚ç‚±‚ñ‚È‚ñ‚È‚Á‚Ä‚½B
  pBmpInfoHeader->biYPelsPerMeter = 0xec4;
  pBmpInfoHeader->biClrUsed = 0;
  pBmpInfoHeader->biClrImportant = 0;

  // ƒsƒNƒZƒ‹‘‚«ž‚Ý
  int x, y;
  for( y = 0 ; y < m_rectCopySrc.Height() ; y++ ) {
    for( x = 0 ; x < m_rectCopySrc.Width() ; x++ ) {
      if( CEasyToonDoc::_GetPixelColor1( pSrc, m_rectCopySrc.Width(), m_rectCopySrc.Height(), x, y ) == 1 ) {
        CEasyToonDoc::_SetPixelColor24( dst + sizeof(BITMAPINFOHEADER), m_rectCopySrc.Width(), m_rectCopySrc.Height(),
                           x, m_rectCopySrc.Height() - y - 1, 0xffffff );
      } else {
        CEasyToonDoc::_SetPixelColor24( dst + sizeof(BITMAPINFOHEADER), m_rectCopySrc.Width(), m_rectCopySrc.Height(),
                           x, m_rectCopySrc.Height() - y - 1, 0x000000 );
      }
    }
  }

  ::GlobalUnlock( hCopy );

  delete [] pSrc;

  // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒvƒ“
  if( OpenClipboard () == 0 ) return; // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒvƒ“Ž¸”s
  // ƒNƒŠƒbƒvƒ{[ƒh‚ÌƒI[ƒi[‚É‚È‚é
  if( EmptyClipboard () == 0 ) return;  // ƒI[ƒi[‚É‚È‚ê‚¸
  
  // ƒNƒŠƒbƒvƒ{[ƒh‚Öƒf[ƒ^‚ðÝ’è
  if( ::SetClipboardData( CF_DIB, (HANDLE) hCopy ) == NULL )  {
    // Ž¸”s‚µ‚½‚Æ‚«
    CloseClipboard();
    return;
  }
  
  // ¬Œ÷
  CloseClipboard();
}


void CEasyToonView::_CurrentFrameToCopyBuffer(CRect rectCopySrc)
{
  m_dcCopySrc.RestoreDC( -1 );
  
  // ƒy[ƒXƒgƒoƒbƒtƒ@‚ÌƒNƒŠƒA
  if( m_pbmpCopySrc != NULL ) delete m_pbmpCopySrc;
  m_pbmpCopySrc = NULL;

  // bmp ì¬
  m_pbmpCopySrc = new CBitmap;
  m_pbmpCopySrc->CreateBitmap( rectCopySrc.Width(), rectCopySrc.Height(), 1,  1, NULL );

  m_dcCopySrc.SaveDC();

  if( m_pbmpCopySrc != NULL ) {
    m_dcCopySrc.SelectObject( m_pbmpCopySrc );
    m_dcCopySrc.BitBlt( 0, 0, rectCopySrc.Width(), rectCopySrc.Height(),
      GetDocument()->GetFrameDC( m_nCurrentFrame ), rectCopySrc.left, rectCopySrc.top, SRCCOPY );
  }
}


//--------------------------------------------------------------------------
//	ƒRƒs[
//--------------------------------------------------------------------------
void CEasyToonView::OnEditCopy() 
{

  if ( m_nTool == move ) {
    _CopyBufferToClipboard();
  } else {
    CRect rect;
    // ƒNƒŠƒbƒvƒ{[ƒh‚Ö“\‚è•t‚¯
    rect.SetRect( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy );
    _CurrentFrameToClipboard( rect );
  }
}

//--------------------------------------------------------------------------
//	ƒJƒbƒg
//--------------------------------------------------------------------------
void CEasyToonView::OnEditCut() 
{
  CRect rect;

  // ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
	UpdateUndoBuffer( restoreBitmap );

  // ƒJƒbƒg—Ìˆæ‚ÌÝ’è
  if( m_nTool == move ) {
    rect = m_rectCopySrc;
    _CopyBufferToClipboard();
    // ”ÍˆÍ‘I‘ðƒ‚[ƒh‚ÉˆÚs
    m_rectCopySrc.SetRectEmpty();
    m_rectCopyDst.SetRectEmpty();
  } else {
    rect.SetRect( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy );
    // ƒNƒŠƒbƒvƒ{[ƒh‚Ö“\‚è•t‚¯
    _CurrentFrameToClipboard( rect );
  }

	// ƒtƒŒ[ƒ€‚ÌƒNƒŠƒA
/*
	CDC* pdcFrame = GetDocument()->GetFrameDC( m_nCurrentFrame );
  pdcFrame->SaveDC();
	pdcFrame->SelectStockObject( WHITE_PEN );
	pdcFrame->SelectStockObject( WHITE_BRUSH );
	pdcFrame->Rectangle( &rect );
  pdcFrame->RestoreDC( -1 );
*/
  // XV
	DrawFrames();
	DrawCanvas(NULL);
	GetDocument()->SetModifiedFlag();
}

//--------------------------------------------------------------------------
//	ƒy[ƒXƒg
//--------------------------------------------------------------------------
void CEasyToonView::OnEditPaste() 
{
	// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
	UpdateUndoBuffer(restoreBitmap);

	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Å‚ ‚Á‚½‚È‚çA‘I‘ð“à—e‚ðƒtƒŒ[ƒ€‚Éƒy[ƒXƒg‚µ‚Ä‚¨‚­
	if ( m_nTool == move ) PasteSelectBuf();
  
  // ƒRƒs[ƒoƒbƒtƒ@‚ÉƒNƒŠƒbƒvƒ{[ƒh‚Ì’†g‚ð“ü‚ê‚é
  // ƒRƒs[æ‚ÆƒRƒs[Œ³‹éŒ`‚à“ü‚ê‚é
  _ClipboardToCopyBuffer();

	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚ÉˆÚs
	m_nTool = move;
	m_ptTo = CPoint(0,0);

  // o‚·
  m_nMoveSubStat = stretchview;

  DrawFrames();
	DrawCanvas(NULL);

  CDC *pDC = GetDC();
  OnPrepareDC( pDC );
  _DrawSelectionRect( pDC, m_rectCopyDst, 2 );
  ReleaseDC( pDC );
  
  GetDocument()->SetModifiedFlag();
}
// ƒJƒbƒgƒoƒbƒtƒ@‚ª–³Œø‚Ìê‡‚ÍAƒy[ƒXƒg‚Í–³Œø
void CEasyToonView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable( ! m_rectCutBuf.IsRectEmpty() );
}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	ƒAƒ“ƒhƒD
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ðXV
//--------------------------------------------------------------------------
void CEasyToonView::UpdateUndoBuffer( UNDOTYPE nType )
{
	m_blCanUndo = TRUE;
	m_blCanRedo = FALSE;
	m_nUndoType = nType;
	m_dcUndoBuf.SelectObject( m_pbmpUndoBuf );
	m_dcUndoBuf.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, GetDocument()->GetFrameDC(m_nCurrentFrame), 0, 0, SRCCOPY );
}

//--------------------------------------------------------------------------
// ƒAƒ“ƒhƒDƒnƒ“ƒhƒ‰
//--------------------------------------------------------------------------
void CEasyToonView::OnEditUndo() 
{
	CEasyToonDoc* pDoc = GetDocument();

	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move )
	{
    _EndCopy();
		//PasteSelectBuf();
		//m_rectSelect.SetRectEmpty();
	}

	// ƒAƒ“ƒhƒD‚ÌŽí—Þ‚É‚æ‚Á‚Ä•ªŠò
	switch ( m_nUndoType )
	{
	case restoreFrame:		// ƒtƒŒ[ƒ€•œ‹A
		// ƒtƒŒ[ƒ€‘}“ü
		pDoc->InsertNewFrame( m_nCurrentFrame );
		// •œ‹A
		m_dcUndoBuf.SelectObject( m_pbmpUndoBuf );
		GetDocument()->GetFrameDC(m_nCurrentFrame)->BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, &m_dcUndoBuf, 0, 0, SRCCOPY );
		break;
	case restoreBitmap:		// ƒrƒbƒgƒ}ƒbƒv•œ‹A
		// ƒŠƒhƒDƒf[ƒ^‚ÌXV
		m_dcUndoBuf.SelectObject( m_pbmpRedoBuf );
		m_dcUndoBuf.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, GetDocument()->GetFrameDC(m_nCurrentFrame), 0, 0, SRCCOPY );
		// •œ‹A
		m_dcUndoBuf.SelectObject( m_pbmpUndoBuf );
		GetDocument()->GetFrameDC(m_nCurrentFrame)->BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, &m_dcUndoBuf, 0, 0, SRCCOPY );
		break;
	case deleteFrame:		// ƒtƒŒ[ƒ€íœ
		m_dcUndoBuf.SelectObject( m_pbmpRedoBuf );
		m_dcUndoBuf.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, GetDocument()->GetFrameDC(m_nCurrentFrame), 0, 0, SRCCOPY );
		pDoc->DeleteFrame(m_nCurrentFrame);
		if ( pDoc->GetCount() == 0 ) pDoc->AppendNewFrame();
		if ( pDoc->GetCount() == m_nCurrentFrame ) m_nCurrentFrame--;
		break;
	}

	// ó‘ÔXV
	m_blCanUndo = FALSE;
	m_blCanRedo = TRUE;
	DrawFrames();
	DrawCanvas(NULL);
}

void CEasyToonView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_blCanUndo );
}

//--------------------------------------------------------------------------
// ƒŠƒhƒDƒnƒ“ƒhƒ‰
//--------------------------------------------------------------------------
void CEasyToonView::OnEditRedo() 
{
	CEasyToonDoc* pDoc = GetDocument();

	// ƒAƒ“ƒhƒD‚ÌŽí—Þ‚É‚æ‚Á‚Ä•ªŠò
	switch ( m_nUndoType )
	{
	case restoreFrame:		// ƒtƒŒ[ƒ€•œ‹AF‚Â‚Ü‚èƒtƒŒ[ƒ€íœ
		m_dcUndoBuf.SelectObject( m_pbmpUndoBuf );
		m_dcUndoBuf.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, GetDocument()->GetFrameDC(m_nCurrentFrame), 0, 0, SRCCOPY );
		pDoc->DeleteFrame(m_nCurrentFrame);
		if ( pDoc->GetCount() == 0 ) pDoc->AppendNewFrame();
		if ( pDoc->GetCount() == m_nCurrentFrame ) m_nCurrentFrame--;
		break;
	case restoreBitmap:		// ƒrƒbƒgƒ}ƒbƒv•œ‹A
		// •œ‹A
		m_dcUndoBuf.SelectObject( m_pbmpRedoBuf );
		GetDocument()->GetFrameDC(m_nCurrentFrame)->BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, &m_dcUndoBuf, 0, 0, SRCCOPY );
		break;
	case deleteFrame:		// ƒtƒŒ[ƒ€íœF‚Â‚Ü‚èƒtƒŒ[ƒ€•œ‹A
		// ƒtƒŒ[ƒ€‘}“ü
		pDoc->InsertNewFrame( m_nCurrentFrame );
		// •œ‹A
		m_dcUndoBuf.SelectObject( m_pbmpRedoBuf );
		GetDocument()->GetFrameDC(m_nCurrentFrame)->BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy, &m_dcUndoBuf, 0, 0, SRCCOPY );
		break;
	}

	// ó‘ÔXV
	m_blCanUndo = TRUE;
	m_blCanRedo = FALSE;
	DrawFrames();
	DrawCanvas(NULL);
}

void CEasyToonView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_blCanRedo );
}




//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	ƒc[ƒ‹ƒ{ƒ^ƒ“
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	”ÍˆÍ‘I‘ð
//--------------------------------------------------------------------------
void CEasyToonView::OnToolSelect() 
{
	m_nTool = select;
}
void CEasyToonView::OnUpdateToolSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_nTool == select || m_nTool == move ? 1 : 0 );
}

//--------------------------------------------------------------------------
//	‚»‚Ì‘¼‚Ìƒc[ƒ‹
//--------------------------------------------------------------------------
void CEasyToonView::OnToolBoxBtn( UINT nID )
{
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚¾‚Á‚½ê‡‚Í”ÍˆÍ‚ðƒy[ƒXƒg‚µ‚Ä‚¨‚­
	if ( m_nTool == move )
	{
    _EndCopy();
  }
	m_nTool = (TOOL)(nID - ID_TOOL_SELECT);

	if (m_nTool == paraline) 
	{
		m_bFirstUsePara=false;
	}
}
void CEasyToonView::OnUpdateToolBoxBtn ( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck( m_nTool == (TOOL)(pCmdUI->m_nID - ID_TOOL_SELECT) ? 1 : 0 );
}


void CEasyToonView::OnBrushBtn( UINT nID )
{
  m_nBrush = nID - ID_BRUSH_PT0;
  _CreatePatternBrush();
}

void CEasyToonView::OnUpdateBrushBtn( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( m_nBrush == (int) (pCmdUI->m_nID - ID_BRUSH_PT0) ? 1 : 0 );
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//	‚»‚Ì‘¼‚Ìƒƒjƒ…[
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	Ä¶
//--------------------------------------------------------------------------
void CEasyToonView::OnViewPlay() 
{
	CPlayDlg dlg( this, GetDocument(), m_nZoomMagnify, 0, GetDocument()->GetCount()-1 );
	dlg.DoModal();
}

void CEasyToonView::OnViewPlayseg() 
{
	CPlayDlg dlg( this, GetDocument(), m_nZoomMagnify, GetMainFrame()->GetStartFrame()-1, GetMainFrame()->GetEndFrame()-1 );
	dlg.DoModal();
}
//--------------------------------------------------------------------------
//	GIF‚ÌƒGƒNƒXƒ|[ƒg
//--------------------------------------------------------------------------
void CEasyToonView::OnFileExportgif() 
{

	static char szFilter[] = "GIF Files (*.gif)|*.gif|All Files (*.*)|*.*||";
	CFileDialog fileDlg ( true, "gif", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK, szFilter);
	
	
	if ( fileDlg.DoModal() == IDOK ) GetDocument()->ExportGif( fileDlg.GetPathName() );
}

void CEasyToonView::OnFileImportgif() 
{

	static char szFilter[] = "GIF Files (*.gif)|*.gif|All Files (*.*)|*.*||";
	CFileDialog fileDlg ( true, "gif", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK, szFilter);


	if ( fileDlg.DoModal() == IDOK ) GetDocument()->ImportGif( new CArchive(new CFile(fileDlg.GetPathName(),CFile::OpenFlags::modeRead),CArchive::Mode::load),true );
}
//--------------------------------------------------------------------------
// ˜A”Ô BMP ‚ÌƒGƒNƒXƒ|[ƒg
//--------------------------------------------------------------------------
void CEasyToonView::OnFileExportbmp() 
{
	static char szFilter[] = "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog fileDlg ( FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );
	if ( fileDlg.DoModal() == IDOK ) GetDocument()->ExportBmp( fileDlg.GetPathName() );
}

//--------------------------------------------------------------------------
// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌƒvƒƒpƒeƒB
//--------------------------------------------------------------------------
void CEasyToonView::OnViewProperty() 
{
	CAnimPropertyDlg dlg ( this );
	dlg.m_nDefaultDelay = GetDocument()->GetDefaultDelay();
	if ( dlg.DoModal() ) 
	{
		GetDocument()->SetDefaultDelay(dlg.m_nDefaultDelay);
		GetDocument()->SetModifiedFlag();
	}
}

//--------------------------------------------------------------------------
// •\Ž¦ƒIƒvƒVƒ‡ƒ“
//--------------------------------------------------------------------------
void CEasyToonView::OnViewOption() 
{
	CViewOptionDlg dlg ( this );
	dlg.m_nZoomMagnify = m_nZoomMagnify;
	if ( dlg.DoModal() ) 
	{
		m_nZoomMagnify = dlg.m_nZoomMagnify;
		SetScrollSizes ( MM_TEXT, CSize ( 2 * m_CanvasMarginW + m_nZoomMagnify * m_sizeFrame.cx, 
										  2 * m_CanvasMarginH + m_nZoomMagnify * m_sizeFrame.cy  ) );
		Invalidate();
	}
}


//--------------------------------------------------------------------------
// ezt ‚ÌƒCƒ“ƒ|[ƒg
//--------------------------------------------------------------------------

void CEasyToonView::OnFileImportezt() 
{

	static char szFilter[] = "ext Files (*.ezt)|*.ezt|All Files (*.*)|*.*||";
	CFileDialog fileDlg ( TRUE, "ezt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
  if ( fileDlg.DoModal() == IDOK ) {
    GetDocument()->ImportEzt( fileDlg.GetPathName(), m_nCurrentFrame );
    GetMainFrame()->SetPlayLimit( GetDocument()->GetCount() );
  }
  Invalidate( false );
}

//--------------------------------------------------------------------------
// ŽŸƒtƒŒ[ƒ€‚ð•`‚­‚©‚Ç‚¤‚©‚Ì§Œä
//--------------------------------------------------------------------------
void CEasyToonView::OnDrawNextframe() 
{
  m_bDrawNext = !m_bDrawNext;
  _DrawBackFrame();
  Invalidate( false );
}

//--------------------------------------------------------------------------
// ‘OƒtƒŒ[ƒ€‚ð•`‚­‚©‚Ç‚¤‚©‚Ì§Œä
//--------------------------------------------------------------------------
void CEasyToonView::OnDrawPrevframe() 
{
  m_bDrawPrev = !m_bDrawPrev;
  _DrawBackFrame();
  Invalidate( false );
}

//--------------------------------------------------------------------------
// ƒ{ƒ^ƒ“ó‘Ô•ÏX
//--------------------------------------------------------------------------
void CEasyToonView::OnUpdateDrawNextframe(CCmdUI* pCmdUI) 
{
  if( m_bDrawNext != false )
    pCmdUI->SetCheck( 1 );
  else
    pCmdUI->SetCheck( 0 );
}

//--------------------------------------------------------------------------
// ƒ{ƒ^ƒ“ó‘Ô•ÏX
//--------------------------------------------------------------------------
void CEasyToonView::OnUpdateDrawPrevframe(CCmdUI* pCmdUI) 
{
  if( m_bDrawPrev != false )
    pCmdUI->SetCheck( 1 );
  else
    pCmdUI->SetCheck( 0 );
}

//--------------------------------------------------------------------------
// ‹éŒ`‚ðƒLƒƒƒ“ƒoƒXÀ•W‚©‚çƒXƒNƒŠ[ƒ“(ƒNƒ‰ƒCƒAƒ“ƒg)À•W‚Ö
//--------------------------------------------------------------------------
int CEasyToonView::_CanvasToScreen(int pos)
{
  return (pos*m_nZoomMagnify);
}

CRect CEasyToonView::_CanvasToScreen(CRect rect)
{
  CRect rectResult;
  rectResult.top    = m_CanvasMarginH+_CanvasToScreen( rect.top );
  rectResult.bottom = m_CanvasMarginH+_CanvasToScreen( rect.bottom );
  rectResult.left   = m_CanvasMarginW+_CanvasToScreen( rect.left );
  rectResult.right  = m_CanvasMarginW+_CanvasToScreen( rect.right );
  return rectResult;
}


//--------------------------------------------------------------------------
// ŠgkƒsƒbƒJ[‚Â‚«‹éŒ`•`‰æ
//--------------------------------------------------------------------------
void CEasyToonView::_DrawSelectionRect( CDC *pDC, CRect rect, int nPick)
{
//  if( (pDC == NULL) || (rect.IsRectEmpty() != FALSE) ) return;
  if( (pDC == NULL) ) return;
  
  rect = _CanvasToScreen( rect );

  pDC->SaveDC();
	pDC->SetROP2( R2_XORPEN );
	pDC->SelectStockObject( NULL_BRUSH );

  // ƒyƒ“ì¬
  CPen pen( PS_SOLID, 1, (COLORREF)0xffffff), *ppenOld;
  ppenOld = pDC->SelectObject( &pen );

  // ˜g•`‰æ
  pDC->Rectangle( rect );

  // ƒsƒbƒJ[•`‰æ
  CRect rectPicker;
  CBrush brush( 0xffffff );
  pDC->SelectObject( &brush );

  // ã
  rectPicker.top    = rect.top - nPick;
  rectPicker.bottom = rect.top + nPick;
  rectPicker.left   = ((rect.left + rect.right)/2) - nPick;
  rectPicker.right  = ((rect.left + rect.right)/2) + nPick;
  if( (m_iCopyMode == SCALEFREE) || (m_iCopyMode == SCALEFIX) )
    pDC->Rectangle( &rectPicker );

  // ã‚Ì‚©‚Ç
  if( (m_iCopyMode == SCALEFREE) || (m_iCopyMode == ROTATE) || (m_iCopyMode == FREE) ) {
    rectPicker.left   = rect.left - nPick;
    rectPicker.right  = rect.left + nPick;
    pDC->Rectangle( &rectPicker );
    rectPicker.left   = rect.right - nPick;
    rectPicker.right  = rect.right + nPick;
    pDC->Rectangle( &rectPicker );
  }

  // ‰º
  rectPicker.top    = rect.bottom - nPick;
  rectPicker.bottom = rect.bottom + nPick;
  rectPicker.left   = ((rect.left + rect.right)/2) - nPick;
  rectPicker.right  = ((rect.left + rect.right)/2) + nPick;
  if( (m_iCopyMode == SCALEFREE) || (m_iCopyMode == SCALEFIX) )
    pDC->Rectangle( &rectPicker );
  // ‰º‚Ì‚©‚Ç
  if( (m_iCopyMode == SCALEFREE) || (m_iCopyMode == ROTATE) || (m_iCopyMode == FREE) ) {
    rectPicker.left   = rect.left - nPick;
    rectPicker.right  = rect.left + nPick;
    pDC->Rectangle( &rectPicker );
    rectPicker.left   = rect.right - nPick;
    rectPicker.right  = rect.right + nPick;
    pDC->Rectangle( &rectPicker );
  }

  // ¶
  rectPicker.top    = ((rect.top + rect.bottom)/2) - nPick;
  rectPicker.bottom = ((rect.top + rect.bottom)/2) + nPick;
  rectPicker.left   = rect.left - nPick;
  rectPicker.right  = rect.left + nPick;
  if( (m_iCopyMode == SCALEFREE) || (m_iCopyMode == SCALEFIX) )
    pDC->Rectangle( &rectPicker );

  // ¶
  rectPicker.top    = ((rect.top + rect.bottom)/2) - nPick;
  rectPicker.bottom = ((rect.top + rect.bottom)/2) + nPick;
  rectPicker.left   = rect.right - nPick;
  rectPicker.right  = rect.right + nPick;
  if( (m_iCopyMode == SCALEFREE) || (m_iCopyMode == SCALEFIX) )
    pDC->Rectangle( &rectPicker );

  pDC->SelectObject( ppenOld );

  // I—¹
	pDC->RestoreDC(-1);
}

void CEasyToonView::_DrawSelectionArea(CDC *pDC, CPoint pointsrc[], int nPick)
{
  if( (pDC == NULL) ) return;
  CPoint point[4];

  // ‚È‚ñ‚©À•W•ÏŠ·
  int i;
  for( i = 0 ; i < 4 ; i++ ) {
    point[i].x = m_CanvasMarginW+_CanvasToScreen( pointsrc[i].x );
    point[i].y = m_CanvasMarginH+_CanvasToScreen( pointsrc[i].y );
  }

  pDC->SaveDC();
	pDC->SetROP2( R2_XORPEN );
	pDC->SelectStockObject( NULL_BRUSH );

  // ƒyƒ“ì¬
  CPen pen( PS_SOLID, 1, (COLORREF)0xffffff), *ppenOld;
  ppenOld = pDC->SelectObject( &pen );

  // ˜g•`‰æ
  pDC->MoveTo( point[0] );
  pDC->LineTo( point[1] );
  pDC->LineTo( point[2] );
  pDC->LineTo( point[3] );
  pDC->LineTo( point[0] );

  // ƒsƒbƒJ[•`‰æ
  CRect rectPicker;
  CBrush brush( 0xffffff );
  pDC->SelectObject( &brush );

  for( i = 0 ; i < 4 ; i++ ) {
    rectPicker.top    = point[i].y - nPick;
    rectPicker.bottom = point[i].y + nPick;
    rectPicker.left   = point[i].x - nPick;
    rectPicker.right  = point[i].x + nPick;
    pDC->Rectangle( &rectPicker );
  }

  pDC->SelectObject( ppenOld );

  // I—¹
	pDC->RestoreDC(-1);
}

//--------------------------------------------------------------------------
// ŠgkƒsƒbƒJ[‚Æ‚Ì“–‚è”»’è
//--------------------------------------------------------------------------
int CEasyToonView::_PickSelectionRect(CRect rect, int nPick, CPoint point)
{
  CRect rectPicker;

  // ã
  rectPicker.top    = rect.top - nPick;
  rectPicker.bottom = rect.top + nPick;
  rectPicker.left   = ((rect.left + rect.right)/2) - nPick;
  rectPicker.right  = ((rect.left + rect.right)/2) + nPick;
  if( rectPicker.PtInRect( point ) ) return stretchtop;

  // ¶ã
  rectPicker.left   = rect.left - nPick;
  rectPicker.right  = rect.left + nPick;
  if( rectPicker.PtInRect( point ) && (m_iCopyMode == SCALEFREE) ) return stretchlefttop;

  // ‰Eã
  rectPicker.left   = rect.right - nPick;
  rectPicker.right  = rect.right + nPick;
  if( rectPicker.PtInRect( point ) && (m_iCopyMode == SCALEFREE) ) return stretchrighttop;

  // ‰º
  rectPicker.top    = rect.bottom - nPick;
  rectPicker.bottom = rect.bottom + nPick;
  rectPicker.left   = ((rect.left + rect.right)/2) - nPick;
  rectPicker.right  = ((rect.left + rect.right)/2) + nPick;
  if( rectPicker.PtInRect( point ) ) return stretchbottom;

  // ¶‰º
  rectPicker.left   = rect.left - nPick;
  rectPicker.right  = rect.left + nPick;
  if( rectPicker.PtInRect( point ) && (m_iCopyMode == SCALEFREE) ) return stretchleftbottom;

  // ‰E‰º
  rectPicker.left   = rect.right - nPick;
  rectPicker.right  = rect.right + nPick;
  if( rectPicker.PtInRect( point ) && (m_iCopyMode == SCALEFREE) ) return stretchrightbottom;

  // ¶
  rectPicker.top    = ((rect.top + rect.bottom)/2) - nPick;
  rectPicker.bottom = ((rect.top + rect.bottom)/2) + nPick;
  rectPicker.left   = rect.left - nPick;
  rectPicker.right  = rect.left + nPick;
  if( rectPicker.PtInRect( point ) ) return stretchleft;

  // ¶
  rectPicker.top    = ((rect.top + rect.bottom)/2) - nPick;
  rectPicker.bottom = ((rect.top + rect.bottom)/2) + nPick;
  rectPicker.left   = rect.right - nPick;
  rectPicker.right  = rect.right + nPick;
  if( rectPicker.PtInRect( point ) ) return stretchright;

  if( rect.PtInRect( point ) ) return stretchmove;

  return stretchout;

}

int CEasyToonView::_PickSelectionArea(CPoint ptSrc[], int nPick, CPoint ptTarget)
{
  if( (ptSrc[0] == ptSrc[1]) && (ptSrc[0] == ptSrc[2]) && (ptSrc[0] == ptSrc[3]) &&
      (ptSrc[1] == ptSrc[2]) && (ptSrc[1] == ptSrc[3]) &&
      (ptSrc[2] == ptSrc[3]) ) return stretchout;
  
  // ‚æ‚·‚Ý
  int i;
  for( i = 0 ; i < 4 ; i++ ) {
    if( ptTarget.x < (ptSrc[i].x - nPick) ) continue;
    if( (ptSrc[i].x + nPick) < ptTarget.x ) continue;
    if( ptTarget.y < (ptSrc[i].y - nPick) ) continue;
    if( (ptSrc[i].y + nPick) < ptTarget.y ) continue;
    switch( i ) {
    case 0: return stretchlefttop;
    case 1: return stretchrighttop;
    case 2: return stretchrightbottom;
    case 3: return stretchleftbottom;
    }
  }

  // 3 - 0 ‚Æ‚ÌŠOÏ
  int nCross;
  nCross = (ptTarget.x - ptSrc[3].x)*(ptSrc[0].y - ptSrc[3].y) - (ptTarget.y - ptSrc[3].y)*(ptSrc[0].x - ptSrc[3].x);

  for( i = 0 ; i < 3 ; i++ ) {
    if( nCross*((ptTarget.x - ptSrc[i].x)*(ptSrc[i+1].y - ptSrc[i].y) - (ptTarget.y - ptSrc[i].y)*(ptSrc[i+1].x - ptSrc[i].x)) <= 0 ) {
      break;
    }
  }
  
  if( i == 3 ) return stretchmove;

  return stretchout;
}

//--------------------------------------------------------------------------
// ”’•“ü‚ê‘Ö‚¦
//--------------------------------------------------------------------------
void CEasyToonView::OnSwapColor() 
{
  COLORREF crTemp = m_crLeftPen;
  m_crLeftPen = m_crRightPen;
  m_crRightPen = crTemp;
}

//--------------------------------------------------------------------------
// •\Ž¦Šg‘å—¦•ÏX
//--------------------------------------------------------------------------
void CEasyToonView::SetZoomMagnify(int nZoomMagnify)
{
	
	if (nZoomMagnify!=0 && nZoomMagnify<=30)
	{
		
		if (m_nZoomMax==false)
		{
			CPoint MousePoint;

			//Récupère la résolution de l'écran.
			POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};

			//Récupère la position du pointeur
			GetCursorPos(&MousePoint);

            m_nZoomMagnify = nZoomMagnify;


			SetScrollSizes ( MM_TEXT, CSize ( 2 * m_CanvasMarginW + m_nZoomMagnify * m_sizeFrame.cx, 
				2 * m_CanvasMarginH + m_nZoomMagnify * m_sizeFrame.cy  ),CSize(2 * m_CanvasMarginW+m_sizeFrame.cx,2 * m_CanvasMarginH + m_sizeFrame.cy) );

			MousePoint=TranslatePoint(MousePoint,false);

			//Détermine si le pointeur est à l'intérieur du document afin
			//d'avoir une valeur correct pour les scrollbars

			if (MousePoint.x>m_sizeFrame.cx)
			{
				MousePoint.x=0;
			}

			if (MousePoint.y>m_sizeFrame.cy)
			{
				MousePoint.y=0;
			}

			//Déplace la position de l'ascenseur des scollbars suivant 
			//la position de la souris
			//La condition if détermine si la taille du document est supérieur 
			//à celle de l'écran afin d"éviter certains bugs.

			if (m_sizeFrame.cx *m_nZoomMagnify>Ecran.x)
			{
				SetScrollPos(SB_HORZ,MousePoint.x*(GetScrollLimit(SB_HORZ)/m_sizeFrame.cx));
			}

			if (m_sizeFrame.cy * m_nZoomMagnify>Ecran.y)
			{
				SetScrollPos(SB_VERT,MousePoint.y*(GetScrollLimit(SB_VERT)/m_sizeFrame.cy));
			}

			Invalidate();
			
		}
	}

	if (nZoomMagnify==30)
	{
		m_nZoomMax=true;
	}
	else
	{
 		m_nZoomMax=false;
	}
	
	GetMainFrame()->m_spinViewZoom.SetPos(nZoomMagnify);

}


//--------------------------------------------------------------------------
// ƒtƒ@ƒCƒ‹‚©‚ç“\‚è•t‚¯
//--------------------------------------------------------------------------
void CEasyToonView::OnEditPastefromfile() 
{
	static char szFilter[] = "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog fileDlg ( TRUE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );

  if ( fileDlg.DoModal() != IDOK ) return;

//  GetDocument()->ImportBmp24( fileDlg.GetPathName(), m_nCurrentFrame );
//  GetMainFrame()->SetPlayLimit( GetDocument()->GetCount() );

  CFile *pFile;
  CArchive *pArc;

  TRY {
    pFile = new CFile ( fileDlg.GetPathName(), CFile::modeRead );
    pArc = new CArchive( pFile, CArchive::load );
  
    // ‚Æ‚è‚ ‚¦‚¸ƒtƒ@ƒCƒ‹ƒwƒbƒ_‚ð“Ç‚à‚¤
    BITMAPFILEHEADER bmpfilehead;
    pArc->Read( (void*) &bmpfilehead, sizeof(BITMAPFILEHEADER) );
    if( bmpfilehead.bfType == ('B' | ('M' << 8))) { // ƒrƒbƒgƒ}ƒbƒvƒtƒ@ƒCƒ‹‚¶‚á‚È‚¢‚Æƒ_ƒ

      // ƒCƒ“ƒtƒHƒwƒbƒ_“Ç‚Ýž‚Ý
      BITMAPINFOHEADER bmpinfohead;
      pArc->Read( (void*) &bmpinfohead, sizeof(BITMAPINFOHEADER) );
      if( bmpinfohead.biBitCount == 24 ) { // 24 bpp ‚¶‚á‚È‚¢‚Æƒ_ƒ

        // 24 ƒrƒbƒg‚Ì•û‚Ìƒoƒbƒtƒ@Šm•Û
        int nbSrcBytes = bmpinfohead.biWidth*3;
        nbSrcBytes += (4-(nbSrcBytes & 0x3)) & 0x3; // ƒ‰ƒXƒ^‚Ì DWORD ƒoƒEƒ“ƒ_ƒŠ
        nbSrcBytes *= bmpinfohead.biHeight;
        BYTE  *pSrc	= new BYTE[nbSrcBytes];

        // 1 ƒrƒbƒg‚Ì•û‚Ìƒoƒbƒtƒ@Šm•Û
        int nbDstBytes = (bmpinfohead.biWidth >> 3) + (bmpinfohead.biWidth & 0x7 ? 1 : 0); // ƒ‰ƒXƒ^‚ÌƒoƒCƒg”
        // WORD ƒoƒEƒ“ƒ_ƒŠ
        nbDstBytes += (2-(nbDstBytes & 0x1)) & 0x1;
        nbDstBytes *= bmpinfohead.biHeight;
        BYTE *pDst = new BYTE[nbDstBytes];
      
        // 24 ƒrƒbƒg‚Ì•û‚Éƒtƒ@ƒCƒ‹‚©‚ç“Ç‚Ýž‚Ý
        pArc->Read( pSrc, nbSrcBytes );

        // 24 ¨ 1 •ÏŠ·
        int x, y;
        for( y = 0 ; y < bmpinfohead.biHeight ; y++ ) {
          for( x = 0 ; x < bmpinfohead.biWidth ; x++ ) {
            if( CEasyToonDoc::_GetPixelColor24( pSrc, 
              bmpinfohead.biWidth, bmpinfohead.biHeight, x, y ) != 0 ) {
              CEasyToonDoc::_SetPixelColor1( pDst, bmpinfohead.biWidth, bmpinfohead.biHeight,
                           x, bmpinfohead.biHeight - y - 1, 1 );
            } else {
              CEasyToonDoc::_SetPixelColor1( pDst, bmpinfohead.biWidth, bmpinfohead.biHeight,
                           x, bmpinfohead.biHeight - y - 1, 0 );
            }
          }
        }

      	// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
      	UpdateUndoBuffer(restoreBitmap);

      	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Å‚ ‚Á‚½‚È‚çA‘I‘ð“à—e‚ðƒtƒŒ[ƒ€‚Éƒy[ƒXƒg‚µ‚Ä‚¨‚­
      	if ( m_nTool == move ) PasteSelectBuf();
  
        // bmp ì¬
        m_dcCopySrc.RestoreDC( -1 );
        if( m_pbmpCopySrc != NULL ) delete m_pbmpCopySrc; // ƒy[ƒXƒgƒoƒbƒtƒ@‚ÌƒNƒŠƒA
        m_pbmpCopySrc = NULL;
        m_pbmpCopySrc = new CBitmap;
  	    m_pbmpCopySrc->CreateBitmap( bmpinfohead.biWidth, bmpinfohead.biHeight, 1,  1, pDst );
        m_rectCopySrc.SetRect( 0, 0, bmpinfohead.biWidth, bmpinfohead.biHeight );
        m_rectCopyDst = m_rectCopySrc;
        m_dcCopySrc.SaveDC();

        if( m_pbmpCopySrc != NULL )
          m_dcCopySrc.SelectObject( m_pbmpCopySrc );
  
	      // ”ÍˆÍˆÚ“®ƒ‚[ƒh‚ÉˆÚs
	      m_nTool = move;
      	m_ptTo = CPoint(0,0);

        DrawFrames();
	      DrawCanvas(NULL);

        CDC *pDC = GetDC();
        OnPrepareDC( pDC );
        _DrawSelectionRect( pDC, m_rectCopyDst, 2 );
        ReleaseDC( pDC );
  
        GetDocument()->SetModifiedFlag();
        delete [] pDst;
        delete [] pSrc;

      }
    }

    pArc->Close();
    delete pArc;
    delete pFile;
  }
  CATCH ( CFileException, e )
  {
  }
  END_CATCH
  
  
  
  
  Invalidate( false );
}


//--------------------------------------------------------------------------
// ŽŸƒtƒŒ[ƒ€‚ðƒJƒŒƒ“ƒgƒtƒŒ[ƒ€‚Ìƒ}ƒXƒN‚É‚µ‚Ä‘OƒtƒŒ[ƒ€‚Æ‡¬
//--------------------------------------------------------------------------
void CEasyToonView::OnMaskcopy() 
{
  CEasyToonDoc *pDoc = (CEasyToonDoc*) GetDocument();
  if( m_nCurrentFrame < 1 ) return;
  if( m_nCurrentFrame >= (pDoc->GetCount()-1) ) return;

  if ( m_nTool == move ) {
    _EndCopy();
	}

	// ƒAƒ“ƒhƒDƒoƒbƒtƒ@‚ÌXV
	UpdateUndoBuffer(restoreBitmap);

  // ƒ[ƒN€”õ
  CBitmap bmpWork;
  CDC dcWork;
  dcWork.CreateCompatibleDC( NULL );
  bmpWork.CreateBitmap( m_sizeFrame.cx, m_sizeFrame.cy, 1, 1, NULL );
  dcWork.SaveDC();
  dcWork.SelectObject( &bmpWork );

  // ”wŒiƒRƒs[
  dcWork.SetTextColor ( RGB ( 0, 0, 0 ) );
  dcWork.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy,
                 pDoc->GetFrameDC( m_nCurrentFrame - 1 ), 0, 0, SRCCOPY );
  // ƒ}ƒXƒN‚ÅŒŠ‚ð‚ ‚¯‚é
  dcWork.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy,
                 pDoc->GetFrameDC( m_nCurrentFrame + 1 ), 0, 0, MERGEPAINT );

  // ƒJƒŒƒ“ƒgƒtƒŒ[ƒ€‚ðƒRƒs[
  dcWork.BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy,
                 pDoc->GetFrameDC( m_nCurrentFrame ), 0, 0, SRCAND );

  // ƒ[ƒN‚ðƒJƒŒƒ“ƒgƒtƒŒ[ƒ€‚ÖƒRƒs[
  pDoc->GetFrameDC( m_nCurrentFrame )->BitBlt( 0, 0, m_sizeFrame.cx, m_sizeFrame.cy,
                 &dcWork, 0, 0, SRCCOPY );

  dcWork.RestoreDC( -1 );

  Invalidate( false );
}

//--------------------------------------------------------------------------
// ”ÍˆÍŽw’èƒ‚[ƒh
//--------------------------------------------------------------------------
void CEasyToonView::OnTbselectScalefree() 
{
  m_iCopyMode = SCALEFREE; 
}
void CEasyToonView::OnTbselectScalefix() 
{
  m_iCopyMode = SCALEFIX; 
}
void CEasyToonView::OnTbselectRotate() 
{
  m_iCopyMode = ROTATE; 
}
void CEasyToonView::OnTbselectFree() 
{
  m_iCopyMode = FREE; 
}

//--------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------
void CEasyToonView::OnUpdateTbselectScalefree(CCmdUI* pCmdUI) 
{
  if( m_nTool == move )
    pCmdUI->Enable( FALSE );
  else {
    pCmdUI->Enable( TRUE );
    if( m_iCopyMode == SCALEFREE )
      pCmdUI->SetCheck( 1 );
    else
      pCmdUI->SetCheck( 0 );
  }
}


void CEasyToonView::OnUpdateTbselectScalefix(CCmdUI* pCmdUI) 
{
  if( m_nTool == move )
    pCmdUI->Enable( FALSE );
  else {
    pCmdUI->Enable( TRUE );
    if( m_iCopyMode == SCALEFIX )
      pCmdUI->SetCheck( 1 );
    else
      pCmdUI->SetCheck( 0 );
  }
}


void CEasyToonView::OnUpdateTbselectRotate(CCmdUI* pCmdUI) 
{
  if( m_nTool == move )
    pCmdUI->Enable( FALSE );
  else {
    pCmdUI->Enable( TRUE );
    if( m_iCopyMode == ROTATE )
      pCmdUI->SetCheck( 1 );
    else
      pCmdUI->SetCheck( 0 );
  }
}


void CEasyToonView::OnUpdateTbselectFree(CCmdUI* pCmdUI) 
{
  if( m_nTool == move )
    pCmdUI->Enable( FALSE );
  else {
    pCmdUI->Enable( TRUE );
    if( m_iCopyMode == FREE )
      pCmdUI->SetCheck( 1 );
    else
      pCmdUI->SetCheck( 0 );
  }
}

//--------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------
void CEasyToonView::OnMoveStart()
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
    _EndCopy();
  // ƒtƒŒ[ƒ€‚ðæ“ª‚É
	m_nCurrentFrame = 0;
  // ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );
  // ”wŒiXV
  _DrawBackFrame();
  // •`‰æ
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}

void CEasyToonView::OnMoveEnd() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
    _EndCopy();
  // ƒtƒŒ[ƒ€‚ð––”ö‚É
	m_nCurrentFrame = pDoc->GetCount() - 1;;
  // ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );
  // ”wŒiXV
  _DrawBackFrame();
  // •`‰æ
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}

void CEasyToonView::OnMoveNext1() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
    _EndCopy();
  // ŽŸƒtƒŒ[ƒ€‚Ö
	m_nCurrentFrame++;
  if( m_nCurrentFrame >= pDoc->GetCount() )
    m_nCurrentFrame -= pDoc->GetCount();
  
  // ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );
  // ”wŒiXV
  _DrawBackFrame();
  // •`‰æ
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}

void CEasyToonView::OnMoveNext10() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
    _EndCopy();
  // 10 ƒRæ‚Ö
	m_nCurrentFrame += 10;
  while( m_nCurrentFrame >= pDoc->GetCount() )
    m_nCurrentFrame -= pDoc->GetCount();
  
  // ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );
  // ”wŒiXV
  _DrawBackFrame();
  // •`‰æ
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}

void CEasyToonView::OnMovePrev1() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
    _EndCopy();
  // ‘OƒtƒŒ[ƒ€‚Ö
	m_nCurrentFrame--;
  if( m_nCurrentFrame < 0 )
    m_nCurrentFrame = pDoc->GetCount() + m_nCurrentFrame;
  
  // ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );
  // ”wŒiXV
  _DrawBackFrame();
  // •`‰æ
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}

void CEasyToonView::OnMovePrev10() 
{
	CEasyToonDoc* pDoc = GetDocument();
	// ”ÍˆÍˆÚ“®ƒ‚[ƒh‚Ìê‡‚ÍA‘I‘ð”ÍˆÍ‚ðƒy[ƒXƒg
	if ( m_nTool == move ) 
    _EndCopy();
  // 10 ƒR‘O‚Ö
	m_nCurrentFrame -= 10;
  while( m_nCurrentFrame < 0 )
    m_nCurrentFrame = pDoc->GetCount() + m_nCurrentFrame;
  
  // ‰æ–ÊXV
	GetMainFrame()->SetFrameDelay( pDoc->GetFrameDelay(m_nCurrentFrame) );
  // ”wŒiXV
  _DrawBackFrame();
  // •`‰æ
  DrawFrames();
	DrawCanvas(NULL);
	// ƒAƒ“ƒhƒD•s‰Â‚É
	m_blCanUndo = m_blCanRedo = FALSE;
}


void CEasyToonView::_CreatePatternBrush()
{
  if( m_pbmpPattern ) delete m_pbmpPattern;
  m_pbmpPattern = new CBitmap;
  m_pbmpPattern->CreateBitmap( 8, 8, 1, 1, NULL );  
  
  CDC dc;
  dc.CreateCompatibleDC( NULL );
  dc.SaveDC();
  dc.SelectStockObject( NULL_PEN );
  dc.SelectObject( m_pbmpPattern );
  CBrush brush( RGB(m_nBrush*15, m_nBrush*15, m_nBrush*15) );
  dc.SelectObject( &brush );
  dc.Rectangle( 0,0,8,8);
  dc.RestoreDC(-1);
}



BOOL CEasyToonView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	if (zDelta==120)
		
	{
		SetZoomMagnify( m_nZoomMagnify+1 );
	}
	else
	{
		SetZoomMagnify( m_nZoomMagnify-1 );
	}
	//return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}

int CEasyToonView::GetZoomMagnify(void)
{
	return m_nZoomMagnify;
}


void CEasyToonView::OnFlipVertical()
{
	Flip(Vertical);
}

void CEasyToonView::OnFlipHorizontal()
{
	Flip(Horizontal);
}

void CEasyToonView::Flip(int type)
{

	///////////////////////////////////
	CBitmap bmpTemp;
	CDC dcTemp;

	dcTemp.CreateCompatibleDC( NULL );

	bmpTemp.CreateCompatibleBitmap(GetDocument()->GetFrameDC(m_nCurrentFrame),			m_sizeFrame.cx,m_sizeFrame.cy);

	dcTemp.SelectObject( &bmpTemp );
	
	m_dcUndoBuf.SelectObject(m_pbmpUndoBuf);
	///////////////////////////////////
	
	int cols=0,rows =0;

	if (!m_rectCopySrc.IsRectEmpty())
	{
		
		//m_dcUndoBuf.BitBlt(m_rectCopyDst.left,m_rectCopyDst.top,m_rectCopyDst.Width(), 
		//m_rectCopyDst.Height(),&m_dcInterface,
		//m_rectCopySrc.left, m_rectCopySrc.top,SRCCOPY);

		switch(type)
		{

		case Horizontal:
				
			for (cols=0;cols<m_rectCopyDst.Width()+1;cols++) 
			{
				for (rows=0;rows<m_rectCopyDst.Height()+1;rows++) 							{
						dcTemp.SetPixel(m_rectCopyDst.left+cols,m_rectCopyDst.bottom						-rows,m_dcInterface.GetPixel(m_rectCopyDst.left+cols,								m_rectCopyDst.top+rows));
				}
			}
			break;

		case Vertical:

			for (cols=0;cols<m_rectCopyDst.Width()+1;cols++) 
				{
				for (rows=0;rows<m_rectCopyDst.Height()+1;rows++) 
					{
						dcTemp.SetPixel(m_rectCopyDst.right-cols,m_rectCopyDst.top+							rows,m_dcInterface.GetPixel(m_rectCopyDst.left+cols,								m_rectCopyDst.top+rows));
					}
				}
				break;
		}

		GetDocument()->GetFrameDC( m_nCurrentFrame )->BitBlt( m_rectCopyDst.left, m_rectCopyDst.top, m_rectCopyDst.Width(), m_rectCopyDst.Height(),&dcTemp, m_rectCopyDst.left, m_rectCopyDst.top, SRCCOPY);

		m_rectCopyDst.SetRectEmpty();
		m_rectCopySrc.SetRectEmpty();
		
	}
	else
	{
	
		
		m_dcUndoBuf.BitBlt(0,0,m_sizeFrame.cx,m_sizeFrame.cy,GetDocument()->GetFrameDC( m_nCurrentFrame ),0,0,SRCCOPY);

	switch(type)
	{
	case Horizontal:
		
			for (cols = 0; cols < m_sizeFrame.cx+1; cols++) 
			{
				for (rows = 0; rows < m_sizeFrame.cy+1; rows++) 
				{
					dcTemp.SetPixel(cols,m_sizeFrame.cy-rows-1,							GetDocument()->GetFrameDC(m_nCurrentFrame)->						GetPixel(cols,rows));
				}
			}
		break;
		
	case Vertical:
				
			for (cols = 0; cols < m_sizeFrame.cx+1; cols++) 
			{
				for (rows = 0; rows < m_sizeFrame.cy+1; rows++) 
				{
					dcTemp.SetPixel(m_sizeFrame.cx-cols-1,rows,											GetDocument()->GetFrameDC(m_nCurrentFrame)->GetPixel(cols,rows));
				}
			}
		break;
	}

	//Actualise le bitmap et le dc d'annulation des changements.

		GetDocument()->GetFrameDC( m_nCurrentFrame )->BitBlt( 0, 0,							m_sizeFrame.cx, m_sizeFrame.cy,&dcTemp, 0, 0, SRCCOPY);
	}

	//Actualise le bitmap et le dc de rétablissement des changements
	m_dcUndoBuf.SelectObject(m_pbmpRedoBuf);
	m_dcUndoBuf.BitBlt(0,0,m_sizeFrame.cx,m_sizeFrame.cy,GetDocument()->GetFrameDC(		m_nCurrentFrame ),0,0,SRCCOPY);

	dcTemp.RestoreDC( -1 );

	Invalidate(false);

	m_blCanUndo=true;

}
	
void CEasyToonView::OnNegatif()
{

	//m_dcUndoBuf.SelectObject(m_pbmpUndoBuf);
	

	if (!m_rectCopySrc.IsRectEmpty())
	{
		//m_dcUndoBuf.BitBlt(m_rectCopyDst.left,m_rectCopyDst.top,							m_rectCopyDst.Width(), m_rectCopyDst.Height(),&m_dcInterface,						m_rectCopySrc.left, m_rectCopySrc.top,SRCCOPY);
		GetDocument()->GetFrameDC(m_nCurrentFrame)->BitBlt									(m_rectCopyDst.left,m_rectCopyDst.top, m_rectCopyDst.Width(),						m_rectCopyDst.Height(),&m_dcInterface, m_rectCopyDst.left,m_rectCopyDst.top,		NOTSRCCOPY);
		m_rectCopyDst.SetRectEmpty();
		m_rectCopySrc.SetRectEmpty();
		DrawCanvas(NULL);
	}
	else
	{
			
		m_dcUndoBuf.BitBlt(0,0,m_sizeFrame.cx, m_sizeFrame.cy,&								m_dcInterface,0, 0,SRCCOPY);

		GetDocument()->GetFrameDC( m_nCurrentFrame )->BitBlt( 0, 0,							m_sizeFrame.cx, m_sizeFrame.cy,GetDocument()->GetFrameDC(							m_nCurrentFrame ), 0, 0, DSTINVERT);
	}

	m_dcUndoBuf.SelectObject(m_pbmpRedoBuf);
	m_dcUndoBuf.BitBlt(0,0,m_sizeFrame.cx,m_sizeFrame.cy,GetDocument()->GetFrameDC(		m_nCurrentFrame ),0,0,SRCCOPY);

	Invalidate(true);

	m_blCanUndo=true;
}

void CEasyToonView::OnText()
{
	//CFontDlgEt DlgFont(NULL,CF_EFFECTS | CF_SCREENFONTS);
	CFontDlgEt DlgFont(NULL,CF_EFFECTS | CF_SCREENFONTS);
	if((DlgFont.DoModal())==IDCANCEL)
	{
		return;
	}

	CFont font;
	LOGFONT lf;
	
	memset(&lf, 0, sizeof(LOGFONT));  
	lf.lfStrikeOut= DlgFont.IsStrikeOut();
	lf.lfItalic=DlgFont.IsItalic();
	lf.lfUnderline=DlgFont.IsUnderline();
	lf.lfWidth=0;
	lf.lfHeight = DlgFont.GetSize()/10;
	lf.lfWeight=DlgFont.GetWeight()/100;
	strcpy(lf.lfFaceName, DlgFont.GetFaceName());        // request a face name "Arial"
	VERIFY(font.CreateFontIndirect(&lf));  // create the font
	
	if (!m_rectCopySrc.IsRectEmpty())
	{
		m_rectCopySrc.SetRectEmpty();
		m_rectCopyDst.SetRectEmpty();
	}

	int RectHeight=lf.lfHeight*DlgFont.GetTextNbline();
	int RectWidth=1;
	int width;

	CString DlgText=DlgFont.GetText();

	m_dcCopySrc.SelectObject(&m_pbmpCopySrc);
	m_dcCopySrc.SetBkMode(TRANSPARENT);
	m_dcCopySrc.SetTextColor(RGB(0,0,0));
	m_dcCopySrc.SelectObject(font);
	
	m_dcCopySrc.DrawText(DlgText,DlgText.GetLength(),&m_rectCopySrc,DT_CALCRECT);

	m_rectCopySrc.SetRect(CPoint(0,0), CPoint(m_rectCopySrc.Width(),m_rectCopySrc.Height()));
	m_rectCopyDst.SetRect(CPoint(0,0), CPoint(m_rectCopySrc.Width(),m_rectCopySrc.Height()));

	m_pbmpCopySrc = new CBitmap;
	m_pbmpCopySrc->CreateBitmap( m_rectCopySrc.Width(), m_rectCopySrc.Height(), 1,  1, NULL );

	//m_iCopyMode = FREE;
	m_nTool=move;
	CDC* pdc;
    OnPrepareDC( pdc = GetDC() );
	_DrawSelectionRect( pdc, m_rectCopySrc, 2 );
	//_DrawInterfaceMove();
    //rect.SetRect( CPoint(m_CanvasMarginW,m_CanvasMarginH), CPoint(m_CanvasMarginW+DTextSize.cx,m_CanvasMarginH+DTextSize.cy));

	m_dcCopySrc.SelectObject( m_pbmpCopySrc );
	m_dcCopySrc.FillSolidRect(0 , 0, m_rectCopySrc.Width(), m_rectCopySrc.Height(), RGB(255,255,255)); 
	m_dcCopySrc.DrawText(DlgText,DlgText.GetLength(),&m_rectCopySrc,0);
	
	delete m_pbmpCopySrc;
	m_pbmpCopySrc=NULL;

	ReleaseDC(pdc);
    DrawFrames();
    DrawCanvas(NULL);
}

void CEasyToonView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	if (nChar==16 && m_shift==false)
	{
		m_shift=true;
	}
	else
	{
		m_shift=false;
	}
		
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEasyToonView::SquareRect(CRect Rect, CPoint & PtFrom, CPoint & PtTo)
{

	if (Rect.Width()>=Rect.Height())
			{

				if (PtTo.x>=PtFrom.x)
				{
					if (PtTo.y>=PtFrom.y)
					{
						PtTo.x=PtFrom.x+(PtTo.y-PtFrom.y);
					}
					else
					{
						PtTo.x=PtFrom.x+(PtFrom.y-PtTo.y);
					}
					
				}
				else
				{
					if (PtTo.y>=PtFrom.y)
					{
						PtTo.x=PtFrom.x-(PtTo.y-PtFrom.y);
					}
					else
					{
						PtTo.x=PtFrom.x-(PtFrom.y-PtTo.y);
					}
				}
			}
			else
			{
				if (PtTo.y>=PtFrom.y)
				{
					if (PtTo.x>PtFrom.x)
					{
						PtTo.y=PtFrom.y+(PtTo.x-PtFrom.x);
					}
					else
					{
						PtTo.y=PtFrom.y+(PtFrom.x-PtTo.x);
					}
					
				}
				else
				{
					if (PtTo.x>PtFrom.x)
					{
						PtTo.y=PtFrom.y-(PtTo.x-PtFrom.x);
					}
					else
					{
						PtTo.y=PtFrom.y-(PtFrom.x-PtTo.x);
					}
				}
			}
}
void CEasyToonView::OnMButtonDown(UINT nFlags, CPoint point)
{
	OnEditUndo();
	CScrollView::OnMButtonDown(nFlags, point);
}

