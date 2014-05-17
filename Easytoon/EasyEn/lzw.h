////////////////////////////////////////////////////////////////////////////
//
//	LZW圧縮 エンコーダ＆デコーダ (GIF形式対応)
//
//	1998 (C) Keijiro Takahashi (keijiro@din.or.jp)
//
#ifndef __LZW_H_
#define __LZW_H_

#define LZW_MAXBITWIDTH		12
#define LZW_NULL			0xffff

//
//	エンコーダ
//
class CLzwEncoder
{
protected:
	int		m_nCodeSize;		// コード・ビット幅
	int		m_nBitWidth;		// ビット幅

	int		m_nImageWidth;		// イメージ幅
	int		m_nWidthCtr;		// 幅カウンタ

	WORD	m_wCC;				// Clear Code
	WORD	m_wEOI;				// End Of Input

	WORD*	m_pwStrTbl;			// ストリングテーブル
	WORD	m_nStrTblCtr;		// ストリングテーブル・カウンタ
	WORD*	m_pwSearchTbl;		// サーチ用ハッシュ・テーブル

	BYTE*	m_pSrcBuf;			// ソースデータ・バッファ
	BYTE*	m_pRead;			// リード・ポインタ
	int		m_nReadBit;			// リード・ポインタ（ビット）

	BYTE*	m_pOutBuf;			// 出力バッファ
	BYTE*	m_pWrite;			// ライト・ポインタ
	int		m_nWriteBit;		// ライト・ポインタ（ビット）

	void	InitStrTbl ();							// ストリング・テーブルの初期化
	void	AddString ( WORD w1, WORD w2 );			// ストリングの登録
	WORD	SearchString ( WORD w1, WORD w2 );		// ストリングの検索
	
	void	InitBuffer( BYTE* pSrc ,BYTE* pOut );	// バッファの初期化
	BYTE	Read();									// 読み込み
	void	Write ( WORD w );						// 書き出し

public:
	CLzwEncoder ();									// コンストラクタ
	~CLzwEncoder ();								// デストラクタ
	int	Encode ( BYTE* pSrc, int nLength, int nImageWidth,  int nBitWidth ,BYTE* pOut );	// エンコーディング
};


//
//	デコーダ
//
class CLzwDecoder
{
protected:
	int		m_nCodeSize;		// コード・ビット幅
	int		m_nBitWidth;		// ビット幅

	int		m_nImageWidth;		// イメージ幅
	int		m_nWidthCtr;		// 幅カウンタ

	WORD	m_wCC;				// Clear Code
	WORD	m_wEOI;				// End Of Input

	WORD*	m_pwStrTblPrefix;		// ストリングテーブル（プレフィクス）
	BYTE*	m_pbStrTblSuffix;		// ストリングテーブル（サフィクス）
	WORD	m_nStrTblCtr;			// ストリングテーブル・カウンタ

	BYTE*	m_pOutBuf;				// 出力バッファ
	BYTE*	m_pWrite;				// ライト・ポインタ
	int		m_nWriteBit;			// ライト・ポインタ（ビット）

	BYTE*	m_pSrcBuf;				// ソースデータ
	BYTE*	m_pRead;				// リード・ポインタ
	int		m_nReadBit;				// リード・ポインタ（ビット）

	void	InitStrTbl ();									// ストリング・テーブルの初期化
	void	AddString ( WORD w, BYTE b );					// ストリング追加
	BYTE	GetString ( WORD wIndex, WORD* w );				// ストリング取得

	void	InitBuffer( BYTE* pSrc ,BYTE* pOut );			// バッファの初期化
	WORD	Read();											// 読み込み
	void	Write ( BYTE b );								// 書き出し

public:
	CLzwDecoder();							// コンストラクタ
	~CLzwDecoder();							// デストラクタ
	int	Decode  ( BYTE* pSrc, int nImageWidth, int nCodeSize, BYTE* pOut );	// デコーディング
};


#endif
