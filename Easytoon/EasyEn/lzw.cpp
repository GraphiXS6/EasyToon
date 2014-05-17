////////////////////////////////////////////////////////////////////////////
//
//	LZW圧縮 エンコーダ＆デコーダ (GIF形式対応)
//
//	1998 (C) Keijiro Takahashi (keijiro@din.or.jp)
//
#include "stdafx.h"
#include "lzw.h"


////////////////////////////////////////////////////////////////////////////
//
//	エンコーダ
//

// コンストラクタ
CLzwEncoder::CLzwEncoder()
{
	m_pwStrTbl		= new WORD[ (1 << LZW_MAXBITWIDTH) * 2 ];
	m_pwSearchTbl	= new WORD[ (1 << LZW_MAXBITWIDTH) * 2 ];
}
// デストラクタ
CLzwEncoder::~CLzwEncoder()
{
	delete m_pwStrTbl;
	delete m_pwSearchTbl;
}

//
//	ストリング・テーブル関連
//
//	ストリング・テーブル初期化
void CLzwEncoder::InitStrTbl ()
{
	for ( int i=0 ; i < (1 << LZW_MAXBITWIDTH) * 2 ; i++ ) m_pwSearchTbl[i] = LZW_NULL;
	for ( int i=0 ; i < (m_wEOI + 1) * 2 ; i+=2 )
	{
		m_pwStrTbl [i  ] = i;
		m_pwStrTbl [i+1] = LZW_NULL;
	}
	m_nStrTblCtr = m_wEOI + 1;
}
// ストリングの登録
void CLzwEncoder::AddString ( WORD w1, WORD w2 )
{
	m_pwStrTbl [ m_nStrTblCtr*2     ] = w1;
	m_pwStrTbl [ m_nStrTblCtr*2 + 1 ] = w2;
	m_pwSearchTbl [ w1*2 + w2 ] = m_nStrTblCtr;
	if ( ++m_nStrTblCtr > 1 << m_nBitWidth ) 
	{
		if ( m_nBitWidth < LZW_MAXBITWIDTH )
		{
			m_nBitWidth++;
		}
		else
		{
			Write ( m_wCC );
			m_nStrTblCtr = m_wEOI + 1;
			m_nBitWidth  = m_nCodeSize + 1;
			for ( int i=0 ; i < (1 << LZW_MAXBITWIDTH) * 2 ; i++ ) m_pwSearchTbl[i] = LZW_NULL;
		}
	}
}
//	ストリングの検索
WORD CLzwEncoder::SearchString ( WORD w1, WORD w2 )
{
	return m_pwSearchTbl [ w1*2 + w2 ];
}

//
//	バッファ操作
//
// 初期化
void CLzwEncoder::InitBuffer( BYTE* pSrc ,BYTE* pOut )
{
	m_pSrcBuf	= pSrc;
	m_pRead		= pSrc;
	m_pOutBuf	= pOut;
	m_pWrite	= pOut;
	m_nReadBit	= 0;
	m_nWriteBit	= 8;
	*m_pWrite	= 0;
}
// 読み込み
BYTE CLzwEncoder::Read ()
{
	BYTE rval = (*m_pRead >> (7 - m_nReadBit)) & 1;
	if ( --m_nWidthCtr == 0 )
	{
		m_nReadBit = 0;
		m_pRead += (m_nImageWidth & 15) > 8 ? 1 : (m_nImageWidth & 15) ? 2 : 1;
		m_nWidthCtr = m_nImageWidth;
	}
	else if ( ++m_nReadBit == 8 )
	{
		m_nReadBit = 0;
		m_pRead++;
	}
	return rval;
}
// 書き出し
void CLzwEncoder::Write ( WORD w )
{
	int nBit = m_nBitWidth;
	while ( nBit > m_nWriteBit )
	{
		*m_pWrite |=  w << (8 - m_nWriteBit);
		w >>= m_nWriteBit;
		*++m_pWrite = 0;
		nBit -= m_nWriteBit;
		m_nWriteBit = 8;
	}
	*m_pWrite |=  w << (8 - m_nWriteBit);
	m_nWriteBit -= nBit;
}

//
// lzwエンコーディング
//
int CLzwEncoder::Encode ( BYTE* pSrc, int nLength, int nImageWidth, int nCodeSize, BYTE* pOut )
{
	// 初期化
	m_nCodeSize		= (nCodeSize == 1) ? 2 : nCodeSize;
	m_nBitWidth		= m_nCodeSize + 1;
	m_wCC			= 1 << m_nCodeSize;
	m_wEOI			= m_wCC + 1;
	m_nImageWidth	= nImageWidth;
	m_nWidthCtr		= nImageWidth;
	// バッファ初期化
	InitBuffer ( pSrc, pOut );
	// テーブル初期化
	InitStrTbl ();

	// lzwメイン
	WORD wPrev = Read();
	Write ( m_wCC );
	for ( int i=0 ; i < nLength-1 ; i++ )
	{
		WORD wInp  = Read();
		WORD wCode = SearchString ( wPrev, wInp );
		if ( wCode != LZW_NULL )
		{
			wPrev = wCode;
		}
		else
		{
			Write ( wPrev );
			AddString ( wPrev, wInp );
			wPrev = wInp;
		}
	}
	Write ( wPrev );
	Write ( m_wEOI );
	return m_pWrite - m_pOutBuf + 1;
}



////////////////////////////////////////////////////////////////////////////
//
//	デコーダ
//

// コンストラクタ
CLzwDecoder::CLzwDecoder()
{
	m_pwStrTblPrefix	= new WORD [ 1 << LZW_MAXBITWIDTH ];
	m_pbStrTblSuffix	= new BYTE [ 1 << LZW_MAXBITWIDTH ];
}
// デストラクタ
CLzwDecoder::~CLzwDecoder()
{
	delete m_pwStrTblPrefix;
	delete m_pbStrTblSuffix;
}

//
//	ストリング・テーブル関連
//
//	ストリング・テーブル初期化
void CLzwDecoder::InitStrTbl ()
{
	for ( int i=0 ; i <= m_wEOI ; i++ )
	{
		m_pwStrTblPrefix [i] = LZW_NULL;
		m_pbStrTblSuffix [i] = i;
	}
	m_nStrTblCtr = m_wEOI + 1;
}
// ストリングの登録
void CLzwDecoder::AddString ( WORD w, BYTE b )
{
	m_pwStrTblPrefix [m_nStrTblCtr] = w;
	m_pbStrTblSuffix [m_nStrTblCtr] = b;
	m_nStrTblCtr++;
}
// ストリングの取得
BYTE CLzwDecoder::GetString ( WORD wIndex, WORD* w )
{
	*w = m_pwStrTblPrefix [wIndex];
	return m_pbStrTblSuffix [wIndex];
}

//
//	バッファ操作
//
// 初期化
void CLzwDecoder::InitBuffer( BYTE* pSrc ,BYTE* pOut )
{
	m_pSrcBuf	= pSrc;
	m_pRead		= pSrc;
	m_pOutBuf	= pOut;
	m_pWrite	= pOut;
	m_nReadBit	= 8;
	m_nWriteBit	= 0;
	*m_pWrite	= 0;
}
// 読み込み
WORD CLzwDecoder::Read()
{
	WORD rval = 0;
	int nBit = m_nBitWidth;
	while ( nBit > m_nReadBit )
	{
		rval |= ( *m_pRead >> (8 - m_nReadBit) ) << (m_nBitWidth - nBit);
		nBit -= m_nReadBit;
		m_pRead ++;
		m_nReadBit = 8;
	}
	rval |= ( (*m_pRead >> (8 - m_nReadBit) ) & ((1 << nBit) - 1) ) << (m_nBitWidth - nBit);
	m_nReadBit -= nBit;
	return rval;
}
// 書き出し
void CLzwDecoder::Write ( BYTE b )
{
	if ( m_nWidthCtr == 0 )
	{
		m_nWriteBit = 0;
		*( m_pWrite += (m_nImageWidth & 15) > 8 ? 1 : (m_nImageWidth & 15) ? 2 : 1 ) = 0;
		m_nWidthCtr = m_nImageWidth;
	}
	else if ( m_nWriteBit == 8 )
	{
		m_nWriteBit = 0;
		*++m_pWrite = 0;
	}
	*m_pWrite |= b << (7 - m_nWriteBit++);
	m_nWidthCtr--;
}

//
// Lzwデコーディング
//
int	CLzwDecoder::Decode ( BYTE* pSrc, int nImageWidth, int nCodeSize, BYTE* pOut )
{
	// 初期化
	m_nCodeSize		= (nCodeSize == 1) ? 2 : nCodeSize;
	m_nBitWidth		= m_nCodeSize + 1;
	m_wCC			= 1 << m_nCodeSize;
	m_wEOI			= m_wCC + 1;
	m_nImageWidth	= nImageWidth;
	m_nWidthCtr		= nImageWidth;
	// バッファ初期化
	InitBuffer ( pSrc, pOut );
	// テーブル初期化
	InitStrTbl ();

	// メイン
	WORD	wPrev = LZW_NULL;
	WORD	wInp;
	while ( ( wInp = Read() ) != m_wEOI )
	{
		if ( wInp == m_wCC )
		{
			m_nStrTblCtr = m_wEOI + 1;
			m_nBitWidth	 = m_nCodeSize + 1;
			wPrev		 = LZW_NULL;
		}
		else
		{
			BYTE	abTmp[1024]; // ロード時に落ちるようならココを大きくすればなんとかなるかも
			int		nTmpCtr = 0;
			WORD	wPrefix;

			if ( wInp < m_nStrTblCtr )
			{
				wPrefix = wInp;
				while ( wPrefix != LZW_NULL ) abTmp[nTmpCtr++] = GetString ( wPrefix, &wPrefix );
				for ( int i=nTmpCtr-1 ; i>=0 ; i-- ) Write ( abTmp[i] );
				if ( wPrev != LZW_NULL ) AddString ( wPrev, abTmp[nTmpCtr-1] );
			}
			else
			{
				wPrefix = wPrev;
				while ( wPrefix != LZW_NULL ) abTmp[nTmpCtr++] = GetString ( wPrefix, &wPrefix );
				for ( int i=nTmpCtr-1 ; i>=0 ; i-- ) Write ( abTmp[i] );
				Write ( abTmp[nTmpCtr-1] );
				AddString ( wPrev, abTmp[nTmpCtr-1] );
			}
			wPrev = wInp;
			if ( m_nStrTblCtr == 1 << m_nBitWidth && m_nBitWidth < LZW_MAXBITWIDTH ) m_nBitWidth++;
		}
	}
	return m_pWrite - m_pOutBuf;
}


