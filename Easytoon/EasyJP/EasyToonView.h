#if !defined(AFX_EASYTOONVIEW_H__EE57B9D1_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_EASYTOONVIEW_H__EE57B9D1_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#define VIEW_MARGIN		2

//
// CFrameViewクラス
//
class CEasyToonView : public CScrollView
{
public:
	// アクセスメソッド
	// 関連付けられたドキュメントの取得
	CEasyToonDoc* GetDocument();
	// フレームウィンドウの取得
	CMainFrame* GetMainFrame()					{ return (CMainFrame*)GetParentFrame(); }
	// カレントフレーム番号の取得
	int			GetCurrentFrame()				{ return m_nCurrentFrame; }
	// フレーム・インターバルの設定
	void		SetFrameDelay ( int nDelay )	{ GetDocument()->SetFrameDelay ( m_nCurrentFrame, nDelay ); }
	// ペンサイズの変更
	void		SetPenSize ( int nSize )		{ m_nPenSize = nSize; }

	// 消しゴムサイズ変更
	void		SetEraseSize ( int nSize )		{ m_nRPenSize = nSize; }


protected:
	// オブジェクト
enum TOOL { select, pen, fill, line, linestrip, rect, fillRect, ellipse, fillEllipse,paraline,move,Text } m_nTool;	// 描画モード
enum MOVESUBSTAT { stretchout, stretchmove, stretchlefttop, stretchtop, stretchrighttop, stretchleft, stretchright, stretchleftbottom, stretchbottom, stretchrightbottom, stretchview } m_nMoveSubStat;

  enum {Horizontal,Vertical};

  int			m_nCurrentFrame;		// カレントフレーム番号
	// キャンバス
	CBitmap*	m_pbmpCanvas;			// キャンバスの仮想画面
	CDC			m_dcCanvas;				// キャンバスのメモリＤＣ
  bool m_bDrawPrev;
  bool m_bDrawNext;
  // フレーム淡色表示用
  CBitmap *m_pbmpBackFrame;
  CDC m_dcBackFrame;

  // ラインや拡大縮小などのインターフェイス表示用
  CBitmap *m_pbmpInterface;
  CDC m_dcInterface;
  int m_nInterfacePenSize;
  COLORREF m_crInterfacePenColor;

  // アンドゥ
	BOOL		m_blCanUndo;			// アンドゥ可能か？
	BOOL		m_blCanRedo;			// リドゥ可能か？
	enum UNDOTYPE { restoreBitmap, restoreFrame, deleteFrame } m_nUndoType;	// アンドゥの種類
	CBitmap*	m_pbmpUndoBuf;			// アンドゥ用バッファ
	CBitmap*	m_pbmpRedoBuf;			// リドゥ用バッファ
	CDC			m_dcUndoBuf;			// アンドゥ用バッファへのメモリＤＣ
	// カット＆ペースト
//	CRect		m_rectSelect;			// 選択範囲
//	CBitmap*	m_pbmpSelectBuf;		// 選択用バッファ
//	CDC			m_dcSelectBuf;			// 選択用バッファへのメモリＤＣ
//	CRect		m_rectCutBuf;			// カットバッファの矩形情報
//	CBitmap*	m_pbmpCutBuf;			// カットバッファ
//	CDC			m_dcCutBuf;				// カットバッファのメモリＤＣ
	// アトリビュート
	CSize		m_sizeFrame;			// フレームのサイズ
	int			m_nZoomMagnify;			// ズーム率
	int			m_nTraceDepth;			// トレスする深さ
	double		m_dTraceFade;			// トレスのライト強度
	int			m_nPenSize;				// ペンサイズ
	int     m_nRPenSize;
	COLORREF	m_crLeftPen;			// 左ペンの色
	COLORREF	m_crRightPen;			// 右ペンの色
  bool    m_bLine;          // ラインストリップのときにライン描画中
  bool    m_bButton;        // ラインストリップのときのクリック判定
  int     m_nLastButton;    // ラインストリップ時の最後に押されてたボタン
  // 雑用オブジェクト
	CPoint		m_ptFrom;				// ポイント指定・雑用
	CPoint		m_ptTo;					// ポイント指定・雑用
	CRect		m_rectTmp;				// 矩形指定・雑用
	CPen		m_penDraw;				// 描画用ペン

  // カット＆ペースト
  CRect m_rectCopyDst; // コピー先矩形保存
  CRect m_rectCopySrc; // コピー元矩形保存
  CBitmap *m_pbmpCopySrc;
  CDC m_dcCopySrc;

  CPoint m_ptCopyDst[4]; // 自由変形コピー先保存
  float m_fRotate; // いちおう回転角を保存しとく
  CPoint m_ptCopyCenterSrc; // いちおう中心も保存しとく
  CPoint m_ptCopyCenterDst; 

  //Variables pour l'utilisation de paraline
  double m_paraline_coefdir;
  double m_paraline_B;
  CRect m_paraline_rect;

  bool m_bFirstUsePara;
  bool consty;
  bool constx;

  bool m_shift;

  int m_CanvasMarginH,m_CanvasMarginW;

  enum COPYMODE { SCALEFREE, SCALEFIX, ROTATE, FREE };

  int m_iCopyMode; // コピーモード

  // パターン
  CBitmap *m_pbmpPattern;
  int m_nBrush;

  bool m_nZoomMax;
	// 描画メソッド
	void	DrawFrames( CRect* pRect = NULL, BOOL blSelReg = TRUE );	// フレームをキャンバスに描画
	void	DrawCanvas( CDC* pDC, CRect* pRect = NULL );		// キャンバスをウィンドウに描画
	// マウスハンドラ補助
	void	ScreenToCanvas( POINT* pt, BOOL blClip = TRUE );	// 座標をキャンバスのローカル座標に変換
	void	QuantizeForCanvas( POINT* pt, BOOL blClip = TRUE );	// キャンバスの解像度に合わせて量子化
	void	PasteSelectBuf();									// 範囲指定バッファを吐き出す
	void	BeginDraw( CPoint point, int nBtn );				// 描画開始（ButtonDown）
	void	KeepDraw( CPoint point, int nBtn );					// 描画持続 (ButtonMove)
	void	EndDraw( CPoint point, int nBtn );					// 描画終了 (ButtonUp)
	void	StopDraw( );										// 描画中止 (二重ButtonDown)
	// その他
	void	UpdateUndoBuffer ( UNDOTYPE nType );			// アンドゥバッファを更新
	void	SquareRect(CRect Rect, CPoint & PtFrom, CPoint & PtTo);
	POINT	TranslatePoint( POINT pt, BOOL blClip );		// 座標をキャンバスのローカル座標に変換
	


protected:
	CEasyToonView();
	DECLARE_DYNCREATE(CEasyToonView)
public:
	//{{AFX_VIRTUAL(CEasyToonView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL
public:
	void _DrawSelectionArea( CDC *pDC, CPoint point[], int nPick);
	void _CopyBufferToClipboard();

	void SetZoomMagnify( int nZoomMagnify );
	void _EndLineStrip();
	virtual ~CEasyToonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
protected:
	//{{AFX_MSG(CEasyToonView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFramePrev();
	afx_msg void OnFrameNext();
	afx_msg void OnFrameInsert();
	afx_msg void OnFrameDelete();
	afx_msg void OnFrameClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnToolSelect();
	afx_msg void OnUpdateToolSelect(CCmdUI* pCmdUI);
	afx_msg void OnFileExportgif();
	afx_msg void OnViewOption();
	afx_msg void OnViewPlay();
	afx_msg void OnViewProperty();
	afx_msg void OnUpdateFramePrev(CCmdUI* pCmdUI);
	afx_msg void OnFileExportbmp();
	afx_msg void OnFileImportgif();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDrawNextframe();
	afx_msg void OnUpdateDrawNextframe(CCmdUI* pCmdUI);
	afx_msg void OnDrawPrevframe();
	afx_msg void OnUpdateDrawPrevframe(CCmdUI* pCmdUI);
	afx_msg void OnSwapColor();
	afx_msg void OnViewPlayseg();
	afx_msg void OnFileImportezt();
	afx_msg void OnEditPastefromfile();
	afx_msg void OnMaskcopy();
	afx_msg void OnTbselectScalefree();
	afx_msg void OnUpdateTbselectScalefree(CCmdUI* pCmdUI);
	afx_msg void OnTbselectScalefix();
	afx_msg void OnUpdateTbselectScalefix(CCmdUI* pCmdUI);
	afx_msg void OnTbselectRotate();
	afx_msg void OnUpdateTbselectRotate(CCmdUI* pCmdUI);
	afx_msg void OnTbselectFree();
	afx_msg void OnUpdateTbselectFree(CCmdUI* pCmdUI);
	afx_msg void OnMoveEnd();
	afx_msg void OnMoveNext1();
	afx_msg void OnMoveNext10();
	afx_msg void OnMovePrev1();
	afx_msg void OnMovePrev10();
	afx_msg void OnMoveStart();
	//}}AFX_MSG
	afx_msg void OnBrushBtn( UINT nID );
	afx_msg void OnUpdateBrushBtn( CCmdUI* pCmdUI );
	afx_msg void OnToolBoxBtn( UINT nID );
	afx_msg void OnUpdateToolBoxBtn( CCmdUI* pCmdUI );
	afx_msg void Flip(int type);
	afx_msg void OnFlipVertical();
	afx_msg void OnFlipHorizontal();
	afx_msg void OnNegatif();
	afx_msg void OnText();
	DECLARE_MESSAGE_MAP()
private:
	void _DrawInterfaceEllipse( void );
	void _DrawInterfaceRectangle();
	void _CreatePatternBrush();
	int _PickSelectionArea( CPoint ptSrc[], int nPick, CPoint ptTarget );
	void _CurrentFrameToCopyBuffer( CRect rectCopySrc );
	void _ClipboardToCopyBuffer();
	void _CurrentFrameToClipboard( CRect rect );
	void _EndCopy();
	void _DrawInterfaceMove();
	void _DrawInterface();
	void _DrawBackFrame();
	void _DrawInterfaceLine();
	int _PickSelectionRect( CRect rect, int nPick, CPoint point );
	int _ClipCanvasH( int pos );
	int _ClipCanvasV( int pos );
	int _CanvasToScreen( int pos );
  int _QuantizeForCanvas(int pos);
	CRect _QuantizeForCanvas( CRect rect, bool bClip = true );
	void _DrawSelectionRect( CDC *pDC, CRect rect, int nPick );
	CRect _CanvasToScreen( CRect rect );
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	int GetZoomMagnify(void);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
};


#ifndef _DEBUG
inline CEasyToonDoc* CEasyToonView::GetDocument()
   { return (CEasyToonDoc*)m_pDocument; }
#endif

//{{AFX_INSERT_LOCATION}}

#endif
