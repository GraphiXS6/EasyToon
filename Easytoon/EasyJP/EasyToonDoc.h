#if !defined(AFX_EASYTOONDOC_H__EE57B9CF_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_EASYTOONDOC_H__EE57B9CF_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



// 定数
#define FRAMEDOC_DEFAULTDELAY 20

//
// フレーム・クラス
//
class CFrame : public CObject
{
protected:
	CBitmap	m_bmp;			// ビットマップ
	CDC		m_dc;			// ビットマップのメモリＤＣ
	int		m_nDelay;		// 遅延時間
public:
	CFrame ( SIZE size );
	~CFrame ();
	// 各種アクセスメソッド
	CBitmap* GetBitmap () { return &m_bmp; }
	CDC*     GetDC ()     { return &m_dc; }
	int		 GetDelay ()  { return m_nDelay; }
	void	 SetDelay ( int nDelay ) { m_nDelay = nDelay; }
};

//
// ドキュメント・クラス
//
class CEasyToonDoc : public CDocument
{
protected:
	CObList	m_frameList;		// フレームのリスト
	CObList m_memoryList;
	int		m_nDefaultDelay;	// 遅延時間のデフォルト値
	CSize	m_sizeFrame;		// フレームのサイズ

public:
	// get
	CBitmap*	GetFrameBmp ( int nFrame );			// フレームの取得（ビットマップ）
	CDC*		GetFrameDC  ( int nFrame );			// フレームの取得（メモリＤＣ）
	int			GetFrameDelay ( int nFrame );		// 遅延時間の取得
	int			GetDefaultDelay ();					// 遅延時間のディフォルトの取得
	SIZE		GetFrameSize ();					// フレームサイズの取得
	int			GetCount ();						// 全フレーム数の取得
	// set
	void		SetFrameDelay ( int nFrame, int nDelay );	// 遅延時間の設定
	void		SetDefaultDelay ( int nDelay );				// 遅延時間のディフォルトの設定
	void		SetFrameSize ( SIZE size );					// フレームサイズの設定
	// フレーム操作
	int			AppendNewFrame ();					// 新規フレームの追加
	void		InsertNewFrame ( int nFrame );		// 新規フレームの挿入
	void		DeleteFrame ( int nFrame );			// フレームの削除
	// 特殊
	void		ExportGif ( LPCTSTR lpszFileName );	// GIF形式でのエクスポート
	void		ImportGif ( CArchive* ar, BOOL IsOpening );
	void    ExportBmp( LPCTSTR lpszFilename ); // BMP形式でのエクスポート

	static void _SetPixelColor1( BYTE *pPixel, int nWidth, int nHeight, int x, int y, int color );
	static int _GetPixelColor1( BYTE *pPixel, int nWidth, int nHeight, int x, int y );
	static void _SetPixelColor24( BYTE *pPixel, int nWidth, int nHeight, int x, int y, COLORREF color );
	static COLORREF _GetPixelColor24( BYTE *pPixel, int nWidth, int nHeight, int x, int y );

protected:
	CEasyToonDoc();
public:
	DECLARE_DYNCREATE(CEasyToonDoc)
	//{{AFX_VIRTUAL(CEasyToonDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL
public:
	void ImportEzt( LPCTSTR lpszFilename, int nInsertPos );
	virtual ~CEasyToonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	//{{AFX_MSG(CEasyToonDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif
