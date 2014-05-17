#include "stdafx.h"
#include "EasyToon.h"
#include "EasyToonDoc.h"
#include "NewOptionDlg.h"
#include "lzw.h"
#include <complex>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CEasyToonDoc, CDocument)

BEGIN_MESSAGE_MAP(CEasyToonDoc, CDocument)
	//{{AFX_MSG_MAP(CEasyToonDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _DEBUG
void CEasyToonDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEasyToonDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


////////////////////////////////////////////////////////////////////////////////
//
//	CFrameクラス
//
CFrame :: CFrame ( SIZE size )
{
	int nReq = size.cy * ( size.cx / 8 + (size.cx & 15 ? 2 : 0) );
	BYTE* pBmpSrc = new BYTE [ nReq ];
	memset ( pBmpSrc, 0xff, nReq );
	m_bmp.CreateBitmap ( size.cx, size.cy,1, 1, pBmpSrc );
	m_dc.CreateCompatibleDC ( NULL );
	m_dc.SelectObject ( &m_bmp );
	m_nDelay = 0;
	delete pBmpSrc;
}
CFrame :: ~CFrame ()
{
	m_dc.RestoreDC ( -1 );
}



////////////////////////////////////////////////////////////////////////////////
//
//	CEastToonDoc クラス
//

//
// コンストラクタ・デストラクタ
//
CEasyToonDoc::CEasyToonDoc()
{
	m_sizeFrame = CSize ( FRAMEDOC_DEFAULTWIDTH, FRAMEDOC_DEFAULTHEIGHT );
//	CEasyToonApp(AfxGetApp()).SetDem(false);
}
CEasyToonDoc::~CEasyToonDoc()
{
}

//
// ドキュメント・基本動作
//
// 「新規作成」
BOOL CEasyToonDoc::OnNewDocument()
{
	CEasyToonApp * a_Dem=((CEasyToonApp*)AfxGetApp());

	if (!CDocument::OnNewDocument()) return FALSE;

	if (a_Dem->GetDem()==FALSE)
	{
		CNewOptionDlg dlg;
		dlg.m_nFrameWidth  = FRAMEDOC_DEFAULTWIDTH;
		dlg.m_nFrameHeight = FRAMEDOC_DEFAULTHEIGHT;
		dlg.DoModal();
		m_sizeFrame = CSize ( dlg.m_nFrameWidth, dlg.m_nFrameHeight );
	}
	a_Dem->SetDem(false);

	AppendNewFrame();
	SetModifiedFlag ( FALSE );
	m_nDefaultDelay = FRAMEDOC_DEFAULTDELAY;

	return TRUE;
}
// ドキュメント再初期化
void CEasyToonDoc::DeleteContents() 
{
	POSITION pos = m_frameList.GetHeadPosition();
	while ( pos ) delete m_frameList.GetNext(pos);
	m_frameList.RemoveAll();
	CDocument::DeleteContents();
}

//
// アクセスメソッド
//
// フレームの取得（ビットマップ）
CBitmap* CEasyToonDoc :: GetFrameBmp ( int nFrame )
{
	POSITION pos = m_frameList.FindIndex ( nFrame );
	return pos != NULL ? ((CFrame*) m_frameList.GetAt ( pos )) -> GetBitmap() : NULL;
}
// フレームの取得（メモリＤＣ）
CDC* CEasyToonDoc :: GetFrameDC ( int nFrame )
{
	POSITION pos = m_frameList.FindIndex ( nFrame );
	return pos != NULL ? ((CFrame*) m_frameList.GetAt ( pos )) -> GetDC() : NULL;
}
// 遅延時間の取得
int CEasyToonDoc :: GetFrameDelay ( int nFrame )
{
	POSITION pos = m_frameList.FindIndex ( nFrame );
	return pos != NULL ? ((CFrame*) m_frameList.GetAt ( pos )) -> GetDelay() : NULL;
}
// 遅延時間のディフォルトの取得
int CEasyToonDoc :: GetDefaultDelay ()
{
	return m_nDefaultDelay;
}
// 全フレーム数の取得
int	CEasyToonDoc :: GetCount ()
{
	return m_frameList.GetCount();
}
// 遅延時間の設定
void CEasyToonDoc :: SetFrameDelay ( int nFrame, int nDelay )
{
	POSITION pos = m_frameList.FindIndex ( nFrame );
	if (pos != NULL)
	{
		CFrame* pFrame = (CFrame*) m_frameList.GetAt( pos );
		SetModifiedFlag ( pFrame->GetDelay() != nDelay );
		pFrame->SetDelay ( nDelay );
	}
}
// 遅延時間のディフォルトの設定
void CEasyToonDoc :: SetDefaultDelay ( int nDelay )
{
	m_nDefaultDelay = nDelay;
	SetModifiedFlag();
}
// フレームサイズの取得
SIZE CEasyToonDoc :: GetFrameSize ()
{
	return m_sizeFrame;
}
// フレームサイズの設定
void CEasyToonDoc :: SetFrameSize ( SIZE size )
{
	m_sizeFrame = size;
}

//
// フレーム操作
//
// 新規フレームの追加
int CEasyToonDoc :: AppendNewFrame ()
{
	m_frameList.AddTail ( new CFrame ( m_sizeFrame ) );
	SetModifiedFlag();
	return m_frameList.GetCount() - 1;
}
// 新規フレームの挿入
void CEasyToonDoc :: InsertNewFrame ( int nFrame )
{
	m_frameList.InsertBefore ( m_frameList.FindIndex ( nFrame ), new CFrame ( m_sizeFrame ) );
	SetModifiedFlag();
}
// フレームの削除
void CEasyToonDoc :: DeleteFrame ( int nFrame )
{
	POSITION pos = m_frameList.FindIndex ( nFrame );
	delete m_frameList.GetAt ( pos );
	m_frameList.RemoveAt ( pos );
	SetModifiedFlag();
}

//
// シリアライズ
//
void CEasyToonDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 保存
		// ヘッダ
    /*
		WORD wFrameCount   = m_frameList.GetCount();
		WORD wFrameWidth   = (WORD)m_sizeFrame.cx;
		WORD wFrameHeight  = (WORD)m_sizeFrame.cy;
		WORD wDefaultDelay = m_nDefaultDelay;
		ar << "EASYTooN000";	// ID
		ar << wFrameCount;		// フレーム数
		ar << wFrameWidth;		// フレーム幅
		ar << wFrameHeight;		// フレーム高さ
		ar << wDefaultDelay;	// 遅延時間のディフォルト
		// フレームの書き込み
		CLzwEncoder encoder;
		POSITION pos		= m_frameList.GetHeadPosition();
		WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
		BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
		BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4 ]; // セーブ時にコケるようならここを大きくするといいカモ
		for ( int i = 0 ; i < wFrameCount ; i++ )
		{
			CFrame*	 frame	 = (CFrame*) m_frameList.GetNext( pos );
			CBitmap* pBmp	 = frame->GetBitmap();
			WORD	 wDelay  = frame->GetDelay();
			pBmp->GetBitmapBits ( wFrameSize, pFrameBuf );
			DWORD	 dwLength = encoder.Encode ( pFrameBuf, wFrameHeight * wFrameWidth , wFrameWidth, 1, pCompBuf );

			ar << wDelay;					// 遅延時間
			ar << dwLength;					// ブロックサイズ
			ar.Write ( pCompBuf, dwLength );// ブロック書き込み
		}
		delete pFrameBuf;
		delete pCompBuf;
    */
		CString Version="EASYTooN001";
		WORD wFrameCount   = m_frameList.GetCount();
		WORD wFrameWidth   = (WORD)m_sizeFrame.cx;
		WORD wFrameHeight  = (WORD)m_sizeFrame.cy;
		WORD wDefaultDelay = m_nDefaultDelay;
		ar << Version;	// ID 001 は無圧縮にしよー
		ar << wFrameCount;		// フレーム数
		ar << wFrameWidth;		// フレーム幅
		ar << wFrameHeight;		// フレーム高さ
		ar << wDefaultDelay;	// 遅延時間のディフォルト
		// フレームの書き込み
		//CLzwEncoder encoder;
		POSITION pos		= m_frameList.GetHeadPosition();
		WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
		BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
		for ( int i = 0 ; i < wFrameCount ; i++ )
		{
			CFrame*	 frame	 = (CFrame*) m_frameList.GetNext( pos );
			CBitmap* pBmp	 = frame->GetBitmap();
			WORD	 wDelay  = frame->GetDelay();
			pBmp->GetBitmapBits ( wFrameSize, pFrameBuf );
	  	DWORD dwLength = wFrameSize;
			ar << wDelay;					// 遅延時間
			ar << dwLength;					// ブロックサイズ
      ar.Write( pFrameBuf, wFrameSize );
		}
		delete pFrameBuf;
  } else {
		// 読み込み
		// ヘッダ
		CString	strID;
		WORD	wFrameCount;
		WORD	wFrameWidth;
		WORD	wFrameHeight;
		WORD	wDefaultDelay;
		ar >> strID;			// ID
		ar >> wFrameCount;		// フレーム数
		ar >> wFrameWidth;		// フレーム幅
		ar >> wFrameHeight;		// フレーム高さ
		ar >> wDefaultDelay;	// 遅延時間のディフォルト
    if ( strID == CString("EASYTooN000") ) {
      // EASYTooN000 だと圧縮形式(旧形式)
  		m_nDefaultDelay = wDefaultDelay;
	  	m_sizeFrame = CSize ( wFrameWidth, wFrameHeight );
		  // フレームの読み込み
		  CLzwDecoder decoder;
		  WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
		  BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
		  BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4 ]; // ロード時にコケるようならここを大きくするといいカモ
		  for ( int i = 0 ; i < wFrameCount ; i++ )
  		{
	  		WORD  wDelay;
		  	DWORD dwLength;
  			ar >> wDelay;					// 遅延時間
	  		ar >> dwLength;					// ブロックサイズ
		  	ar.Read ( pCompBuf, dwLength ); // ブロック読み込み
			  
  			decoder.Decode ( pCompBuf, wFrameWidth, 1, pFrameBuf );
	  		CFrame*	 pFrame	= new CFrame ( m_sizeFrame );
		  	pFrame->SetDelay ( wDelay );
			  CBitmap* pBmp	= pFrame->GetBitmap();
  			pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
	  		m_frameList.AddTail ( pFrame );
  		}
	  	delete pFrameBuf;
		  delete pCompBuf;
    } else if( strID == CString("EASYTooN001") ){
      // EASYTooN001 だと無圧縮形式(新形式)
  		m_nDefaultDelay = wDefaultDelay;
	  	m_sizeFrame = CSize ( wFrameWidth, wFrameHeight );
		  // フレームの読み込み
		  CLzwDecoder decoder;
		  WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
		  BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
      for ( int i = 0 ; i < wFrameCount ; i++ )
  		{
	  		WORD  wDelay;
		  	DWORD dwLength;
  			ar >> wDelay;					// 遅延時間
	  		ar >> dwLength;					// ブロックサイズ
		  	ar.Read ( pFrameBuf, dwLength ); // ブロック読み込み

        CFrame*	 pFrame	= new CFrame ( m_sizeFrame );
		  	pFrame->SetDelay ( wDelay );
			  CBitmap* pBmp	= pFrame->GetBitmap();
  			pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
	  		m_frameList.AddTail ( pFrame );
  		}
	  	delete pFrameBuf;
    }
  }
}

//
// GIF形式でのエクスポート
//
void CEasyToonDoc::ExportGif ( LPCTSTR lpszFileName )
{
	CFile* file;
	
	TRY
	{
		file = new CFile ( lpszFileName, CFile::modeCreate | CFile::modeWrite );

		CArchive ar ( file, CArchive::store );
	
		WORD wFrameWidth   = (WORD)m_sizeFrame.cx;
		WORD wFrameHeight  = (WORD)m_sizeFrame.cy;
		WORD wFrameSize	   = (WORD)(wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) ));

		// ID
		ar.Write ( "GIF89a",6 );
		// logical screen descriptor
		ar << wFrameWidth;	// width
		ar << wFrameHeight;	// height
		ar << (BYTE)0x80;	// Packed Fields
		ar << (BYTE)0;		// BG
		ar << (BYTE)0;		// Aspect Ratio
		// global color map
		ar << (BYTE)0;	
		ar << (BYTE)0;
		ar << (BYTE)0;
		ar << (BYTE)0xff;
		ar << (BYTE)0xff;
		ar << (BYTE)0xff;
		// Netscape Extension
		ar << '!';			// Separator
		ar << (BYTE)0xFF;	// Control Label
		ar << (BYTE)11;		// Block Size
		ar.Write ( "NETSCAPE2.0",11 );	// Application ID
		ar << (BYTE)3;		// Block Size
		ar << (BYTE)1;		
		ar << (WORD)0;		// number of ite rations
		ar << (BYTE)0;		// terminator

		// フレームの書き込み
		CLzwEncoder encoder;
		POSITION pos		= m_frameList.GetHeadPosition();
		BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
		BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4]; // セーブ時にコケるようならここを大きくするといいカモ
		for ( int i = 0 ; i < m_frameList.GetCount() ; i++ )
		{
			CFrame*	 frame	 = (CFrame*) m_frameList.GetNext( pos );
			CBitmap* pBmp	 = frame->GetBitmap();
			WORD	 wDelay  = frame->GetDelay() != 0 ? frame->GetDelay() : m_nDefaultDelay;
			pBmp->GetBitmapBits ( wFrameSize, pFrameBuf );
			DWORD	 dwLength = encoder.Encode ( pFrameBuf, wFrameHeight * wFrameWidth, wFrameWidth, 1, pCompBuf );
			// Graphic Control Extension
			ar << '!';			// Separator
			ar << (BYTE)0xF9;	// Control Label
			ar << (BYTE)4;		// Block Size
			ar << (BYTE)0x08;	// Packed Fields
			ar << wDelay;		// Delay
			ar << (BYTE)0;		// transperency
			ar << (BYTE)0;		// terminator
			// image descriptor
			ar << ',';			// Separator
			ar << (WORD)0;		// Top-Left
			ar << (WORD)0;		
			ar << wFrameWidth;	// Bottom-Right
			ar << wFrameHeight;
			ar << (BYTE)0;		// Packed Fields
			// image data
			ar << (BYTE)2;		// Code Size
			// raster data
			DWORD ptr = 0;
			for ( DWORD j=0 ; ptr + 0xff < dwLength ; j++, ptr+=0xff )
			{
				ar << (BYTE)0xff;
				ar.Write ( pCompBuf + ptr, 0xff );
			}
			ar << (BYTE)(dwLength - ptr);
			ar.Write ( pCompBuf + ptr, dwLength - ptr );
			ar << (BYTE)0;
		}
		delete pFrameBuf;
		delete pCompBuf;

		// Comment Extension
		ar << '!';			// Separator
		ar << (BYTE)(0xFE);	// Control Label
		ar << '-';
		ar.WriteString("This GIF animation was generated by EASYTooN.");
		ar << (BYTE)0;		// terminator
		ar << ';';			// terminator
	}
	CATCH( CFileException, e )
	{
	}
	END_CATCH

	delete file;
}
void CEasyToonDoc::ImportGif(CArchive* ar, BOOL IsOpening )
{
	//CFile* file;

	//TRY
	//{
	//file = new CFile ( lpszFileName, CFile::modeRead);


	//CArchive ar ( file, CArchive::load);
	//char FileType[3];
	//char GifVersion[3];
	//char Ext;
	//WORD Height;
	//WORD WIDTH;
	//WORD wFrameSize;
	//BYTE GLOBALCOLOR;
	//bool GlbExist;
	//int globalnb;

	//char Buffer[1024]={0};

	//ar.ReadString(FileType,3);
	//ar.ReadString(GifVersion,3);
	//ar >> WIDTH;
	//ar >> Height;
	//ar >> GLOBALCOLOR;
	//ar.Read(Buffer,1);
	//GlbExist=GLOBALCOLOR&128;
	//
	//ar.Read(Buffer,1);
	//ar.Read(Buffer,1);
	//
	//if (GlbExist)
	//{
	//	globalnb=pow(2,(float)(GLOBALCOLOR&7)+1);
	//	for (int i=0;i<globalnb;i++)
	//	{
	//		ar.Read(Buffer,1);
	//		ar.Read(Buffer,1);
	//		ar.Read(Buffer,1);
	//	}
	//}
	//ar.ReadString(&Ext,1);

	//if(Ext=='!')
	//{
	//	ar.ReadString(Buffer,2);	// Block Size
	//	ar.ReadString(Buffer,11);	// Application ID
	//	ar.Read(Buffer,5);
	//}

	//
	//
	//	 フレームの書き込み
	//	CLzwDecoder encoder;
	//	POSITION pos		= m_frameList.GetHeadPosition();
	//	BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
	//	BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4]; // セーブ時にコケるようならここを大きくするといいカモ
	//	int nInsertPos=1;

	//	ar.ReadString(&Ext,1);

	//	do
	//	{
	//		CFrame*	 frame	 = new CFrame(CSize(wFrameSize));
	//		CBitmap* pBmp	 = frame->GetBitmap();
	//		WORD wDelay;  //= frame->GetDelay() != 0 ? frame->GetDelay() : m_nDefaultDelay;
	//		WORD Left;
	//		WORD Top;
	//		WORD dwLength;
	//		bool LocalMap;
	//		BYTE Local;
	//		BYTE Lenghtt;
	//		int NbColorLocalMap;

	//		if (Ext=='!')
	//		{			
	//			ar.Read(&Ext,3);
	//			ar>>wDelay;
	//			
	//			ar.Read(&Ext,1);// transparency
	//			ar.Read(&Ext,1);// terminator
	//			ar.ReadString(&Ext,1);
	//		}
	//		

	//		ar >> Left;
	//		ar >> Top;
	//		ar >> WIDTH;
	//		ar >> Height;
	//		ar >> Local;

	//		LocalMap=Local&128;

	//		if (LocalMap)
	//		{
	//			NbColorLocalMap=pow(2,(float)(Local&7)+1);
	//			for(int i=0;i<NbColorLocalMap;i++)
	//			{
	//				ar.Read(&Ext,1);
	//				ar.Read(&Ext,1);
	//				ar.Read(&Ext,1);
	//			}
	//		}
	//		ar.Read(&Ext,1);
	//		ar >> Lenghtt;
	//		
	//		int i=0;

	//		do 
	//		{
	//			ar.Read(&Ext,1);
	//			pCompBuf[i]=Ext;
	//			i++;
	//		} 
	//		while(pCompBuf[i]!=0x00);
	//		pCompBuf[i]='\0';
	//			
	//		encoder.Decode( pCompBuf, wFrameSize,2,pFrameBuf);
	//		
	//		pBmp->SetBitmapBits(wFrameSize,pFrameBuf );
	//		
	//		m_frameList.InsertBefore( m_frameList.FindIndex( nInsertPos ), frame );
	//		nInsertPos++;

	//		ar.ReadString(&Ext,1);
 //
	//		
	//	}
	//	while (Ext=='!');

	//	delete pFrameBuf;
	//	delete [] pCompBuf;

		/*
		for ( int i = 0 ; i < 40 ; i++ )
		{
			//pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
			DWORD	 dwLength = encoder.Decode( pFrameBuf, Height * WIDTH, 1, pCompBuf );
			// Graphic Control Extension
			ar.ReadString(ID,1);			// Separator
			ar.Read(pb,4);		// Block Size
			ar.Read(pb,1);	// Packed Fields
			ar >> wDelay;		// Delay
			ar.Read(pb,1);		// transperency
			ar.Read(pb,1);		// terminator
			// image descriptor
			ar.ReadString(ID,1);			// Separator
			ar.Read(pb,1);		// Top-Left
			ar.Read(pb,1);		
			ar >> wFrameWidth;	// Bottom-Right
			ar >> wFrameHeight;
			ar.Read(pb,1);		// Packed Fields
			// image data
			ar.Read(pb,2);		// Code Size
			// raster data
			DWORD ptr = 0;
			for ( DWORD j=0 ; ptr + 0xff < dwLength ; j++, ptr+=0xff )
			{
				ar << (BYTE)0xff;
				ar.Write ( pCompBuf + ptr, 0xff );
			}
			ar << (BYTE)(dwLength - ptr);
			ar.Write ( pCompBuf + ptr, dwLength - ptr );
			ar << (BYTE)0;
		}
		delete pFrameBuf;
		delete pCompBuf;

		// Comment Extension
		ar << '!';			// Separator
		ar << (BYTE)0xFE;	// Control Label
		ar << "This GIF animation was generated by EASYTooN.";
		ar << (BYTE)0;		// terminator
		ar << ';';			// terminator*/
	/*}
	CATCH( CFileException, e )
	{
	}
	END_CATCH

		delete file;*/
}
//
// 連番BMP形式でのエクスポート
//
void CEasyToonDoc::ExportBmp(LPCTSTR lpszFilename)
{
	CFile* file;
	
	TRY
	{
    int i;
    char szBasename[1024]; // うわーてきとーでーす
    char szFilename[1024];
    char szTemp[5];
     
    // 連番ファイルのベースになる名前を取得
    ::strcpy( szBasename, lpszFilename );

    // 後ろから拡張子までをつぶす
    // 後ろからやらないとディレクトリ名に'.'があったときに死亡しちゃうヨ
    int nbStrings = ::strlen( szBasename );
    for( i = 0 ; i < nbStrings ; i++ ) {
      if( szBasename[nbStrings-1-i] == '.' ) 
	  {
        szBasename[nbStrings-1-i] = '\0';
        break;
      }
      szBasename[nbStrings-1-i] = '\0';
    }

    // フレーム数分くりかえす。
    int nbBitmaps = m_frameList.GetCount();
		POSITION pos		= m_frameList.GetHeadPosition();
    BITMAPFILEHEADER bmpfilehead;
    BITMAPINFOHEADER bmpinfohead;
    DWORD dwFileSize;
    DWORD dwPixels = m_sizeFrame.cx*m_sizeFrame.cy;
    DWORD dwBlack, dwWhite;
    dwBlack = 0x00;
    dwWhite = 0x00ffffff;

    // 24 ビットの方のバッファ確保
    int nbDstBytes = m_sizeFrame.cx*3;
    nbDstBytes += (4-(nbDstBytes & 0x3)) & 0x3; // ラスタの DWORD バウンダリ
    nbDstBytes *= m_sizeFrame.cy;
    BYTE  *pDst	= new BYTE[nbDstBytes];

    // 1 ビットの方のバッファ確保
    int nbSrcBytes = (m_sizeFrame.cx >> 3) + (m_sizeFrame.cy & 0x7 ? 1 : 0); // ラスタのバイト数
    // WORD バウンダリ
    nbSrcBytes += (2-(nbSrcBytes & 0x1)) & 0x1;
    nbSrcBytes *= m_sizeFrame.cy;
    BYTE *pSrc = new BYTE[nbSrcBytes];

    CArchive *pArc;
    for( i = 0 ; i < nbBitmaps ; i++ ) 
	{
      // ファイル名捏造
      ::strcpy( szFilename, szBasename );
      ::sprintf_s( szTemp, "%04d", i );
      ::strcat( szFilename, szTemp );
      ::strcat( szFilename, ".bmp" );

      // BITMAPFILEHEADER 捏造
      bmpfilehead.bfSize = sizeof(BITMAPFILEHEADER);
      bmpfilehead.bfType = 'B' | ('M' << 8);
      bmpfilehead.bfSize = dwFileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nbDstBytes;
      bmpfilehead.bfReserved1 = bmpfilehead.bfReserved2 = 0;
      bmpfilehead.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

      // BITMAPINFOHEADER 捏造
      bmpinfohead.biSize = sizeof(BITMAPINFOHEADER);
      bmpinfohead.biWidth = m_sizeFrame.cx;
      bmpinfohead.biHeight = m_sizeFrame.cy;
      bmpinfohead.biPlanes = 1;
      bmpinfohead.biBitCount = 24;
      bmpinfohead.biCompression = BI_RGB;
      bmpinfohead.biSizeImage = nbDstBytes;
      bmpinfohead.biXPelsPerMeter = 0xec4; // なんかペイントで出力したらこんなんなってた。
      bmpinfohead.biYPelsPerMeter = 0xec4;
      bmpinfohead.biClrUsed = 0;
      bmpinfohead.biClrImportant = 0;

      file = new CFile ( szFilename, CFile::modeCreate | CFile::modeWrite );
      pArc = new CArchive( file, CArchive::store );

      CFrame*	 frame	 = (CFrame*) m_frameList.GetNext( pos );
      CBitmap* pBmp	 = frame->GetBitmap();

      // まずファイルヘッダ
      pArc->Write( (void*) &bmpfilehead, sizeof(BITMAPFILEHEADER) );
      // 次にインフォヘッダ
      pArc->Write( (void*) &bmpinfohead, sizeof(BITMAPINFOHEADER) );
      
			pBmp->GetBitmapBits ( nbSrcBytes, pSrc );

      // 1 → 24 変換
      int x, y;
      for( y = 0 ; y < m_sizeFrame.cy ; y++ ) {
        for( x = 0 ; x < m_sizeFrame.cx ; x++ ) {
          if( _GetPixelColor1( pSrc, m_sizeFrame.cx, m_sizeFrame.cy, x, y ) != 0 ) {
            _SetPixelColor24( pDst, m_sizeFrame.cx, m_sizeFrame.cy, x, m_sizeFrame.cy - y - 1, 0xffffff );
          } else {
            _SetPixelColor24( pDst, m_sizeFrame.cx, m_sizeFrame.cy, x, m_sizeFrame.cy - y - 1, 0x000000 );
          }
        }
      }
      pArc->Write( pDst, nbDstBytes );
      pArc->Close();
      delete pArc;
      delete file;
		}
    delete [] pSrc;
    delete [] pDst;
	}
	CATCH( CFileException, e )
	{
	}
	END_CATCH


}


void CEasyToonDoc::ImportEzt(LPCTSTR lpszFilename, int nInsertPos)
{

  TRY {
    CFile file( lpszFilename, CFile::modeRead );
    CArchive ar( &file, CArchive::load );

    // ヘッダ読み込み
		CString	strID;
		WORD	wFrameCount;
		WORD	wFrameWidth;
		WORD	wFrameHeight;
		WORD	wDefaultDelay;

    ar >> strID;			// ID
		ar >> wFrameCount;		// フレーム数
		ar >> wFrameWidth;		// フレーム幅
		ar >> wFrameHeight;		// フレーム高さ
		ar >> wDefaultDelay;	// 遅延時間のディフォルト

    // 解像度が一緒じゃないとダメ
    if( (wFrameWidth == m_sizeFrame.cx) && (wFrameHeight == m_sizeFrame.cy) ) {
      if ( strID == CString("EASYTooN000") ) {
        // EASYTooN000 だと圧縮形式(旧形式)
        // フレームの読み込み
  		  CLzwDecoder decoder;
	  	  WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
		    BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
		    BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4 ]; // ロード時にコケるようならここを大きくするといいカモ
  		  for ( int i = 0 ; i < wFrameCount ; i++ ) {
  	  		WORD  wDelay;
	  	  	DWORD dwLength;
  	  		ar >> wDelay;					// 遅延時間
	  	  	ar >> dwLength;					// ブロックサイズ
		  	  ar.Read ( pCompBuf, dwLength ); // ブロック読み込み
			  
    			decoder.Decode ( pCompBuf, wFrameWidth, 1, pFrameBuf );
	    		CFrame*	 pFrame	= new CFrame ( m_sizeFrame );
          pFrame->SetDelay ( wDelay );
			    CBitmap* pBmp	= pFrame->GetBitmap();
  			  pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
  	  		m_frameList.InsertBefore( m_frameList.FindIndex( nInsertPos ), pFrame );
          nInsertPos++;
    		}
  	  	delete pFrameBuf;
	  	  delete pCompBuf;
      } else if( strID == CString("EASYTooN001") ){
        // EASYTooN001 だと無圧縮形式(新形式)
  		  // フレームの読み込み
		    WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
  		  BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
        for ( int i = 0 ; i < wFrameCount ; i++ ) {
  	  		WORD  wDelay;
	  	  	DWORD dwLength;
  	  		ar >> wDelay;					// 遅延時間
	  	  	ar >> dwLength;					// ブロックサイズ
		  	  ar.Read ( pFrameBuf, dwLength ); // ブロック読み込み
          CFrame*	 pFrame	= new CFrame ( m_sizeFrame );
	  	  	pFrame->SetDelay ( wDelay );
		  	  CBitmap* pBmp	= pFrame->GetBitmap();
  		  	pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
	  		  m_frameList.InsertBefore( m_frameList.FindIndex( nInsertPos ), pFrame );
          nInsertPos++;
    		}
	    	delete pFrameBuf;
      }
    }
  }
  CATCH ( CFileException, e )
  {
  }
  END_CATCH

}

int CEasyToonDoc::_GetPixelColor1(BYTE *pPixel, int nWidth, int nHeight, int x, int y)
{
  // 1 ラスタのバイト数
  int nbRasterBytes = (nWidth >> 3) + (nWidth & 0x7 ? 1 : 0);
  // WORD バウンダリ
  nbRasterBytes += (2-(nbRasterBytes & 0x1)) & 0x1;

  // マスク作成
  BYTE mask = 1 << (7 - (x & 0x7));

  // そのドットがあるバイトをマスク
  if( (mask & *(pPixel+nbRasterBytes*y+(x/8))) )
    return 1;

  return 0;
}

void CEasyToonDoc::_SetPixelColor1(BYTE *pPixel, int nWidth, int nHeight, int x, int y, int color)
{
  // 1 ラスタのバイト数
  int nbRasterBytes = (nWidth >> 3) + (nWidth & 0x7 ? 1 : 0);
  // WORD バウンダリ
  nbRasterBytes += (2-(nbRasterBytes & 0x1)) & 0x1;
  
  // マスク(?)作成
  BYTE mask = 1 << (7 - (x & 0x7));

  // 書き込み
  if( color )
    *(pPixel+nbRasterBytes*y+(x/8)) |= mask;
  else
    *(pPixel+nbRasterBytes*y+(x/8)) &= ~mask;
}

void CEasyToonDoc::_SetPixelColor24(BYTE *pPixel, int nWidth, int nHeight, int x, int y, COLORREF color)
{
  // 1 ラスタのバイト数
  int nbRasterBytes = nWidth*3;
  // DWORD バウンダリ
  nbRasterBytes += (4-(nbRasterBytes & 0x3)) & 0x3;

  // 書き込み
  pPixel += ((nbRasterBytes*y) + (x*3));
  *pPixel = (BYTE) (color & 0xff);
  *(pPixel+1) = (BYTE)((color >> 8) & 0xff);
  *(pPixel+2) = (BYTE)((color >> 16) & 0xff);
}



COLORREF CEasyToonDoc::_GetPixelColor24(BYTE *pPixel, int nWidth, int nHeight, int x, int y)
{
  // 1 ラスタのバイト数
  int nbRasterBytes = nWidth*3;
  // DWORD バウンダリ
  nbRasterBytes += (4-(nbRasterBytes & 0x3)) & 0x3;

  pPixel += ((nbRasterBytes*y) + (x*3));
  COLORREF color;
  color = *(pPixel+2);
  color = (color << 8) | *(pPixel+1);
  color = (color << 8) | *pPixel;
  return color;
}

	
