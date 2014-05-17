#include "stdafx.h"
#include "EasyToon.h"
#include "EasyToonDoc.h"
#include "NewOptionDlg.h"
#include "lzw.h"
#include "Utils.h"
#include <complex>

#include <gdiplus.h>
#include <Gdiplusimaging.h>

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
	m_bmp.CreateBitmap ( size.cx, size.cy, 1, 1, pBmpSrc );
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
	m_nTracePrevFrameNb = 1;
	m_nTraceNextFrameNb = 1;

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
	m_MemoryList.RemoveAll();
	
	return TRUE;
}
// ドキュメント再初期化
void CEasyToonDoc::DeleteContents() 
{
	POSITION pos = m_frameList.GetHeadPosition();
	while ( pos ) delete m_frameList.GetNext(pos);

	pos = m_MemoryList.GetHeadPosition();
	while ( pos ) delete m_MemoryList.GetNext(pos);

	m_frameList.RemoveAll();
	m_MemoryList.RemoveAll();
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
	CString Ext = ar.GetFile()->GetFileName().Right(3);

	if (ar.IsStoring())
	{
	
		CString Version="EASYTooN003";
		WORD wFrameCount   = m_frameList.GetCount();
		WORD wMemoryCount = m_MemoryList.GetCount();
		WORD wFrameWidth   = (WORD)m_sizeFrame.cx;
		WORD wFrameHeight  = (WORD)m_sizeFrame.cy;
		WORD wDefaultDelay = m_nDefaultDelay;
		ar << Version;	// ID 001 は無圧縮にしよー
		ar << wFrameCount;		// フレーム数
		ar << wFrameWidth;		// フレーム幅
		ar << wFrameHeight;		// フレーム高さ
		ar << wDefaultDelay;	// 遅延時間のディフォルト
		ar << wMemoryCount;
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

	

		pos = m_MemoryList.GetHeadPosition();

		for ( int i = 0 ; i < wMemoryCount ; i++ )
		{
			CBitmap* pBmp	 = (CBitmap*) m_MemoryList.GetNext( pos );
			pBmp->GetBitmapBits ( wFrameSize, pFrameBuf );
			DWORD dwLength = wFrameSize;
			ar << dwLength;					// ブロックサイズ
			ar.Write( pFrameBuf, wFrameSize );
		}

		delete pFrameBuf;
	} 
	else {


		if(Ext == "gif")
		{
			ImportGif(&ar, true,0);
		}
		else
		{
			// 読み込み
			// ヘッダ
			CString	strID;
			WORD	wFrameCount;
			WORD	wFrameWidth;
			WORD	wFrameHeight;
			WORD	wDefaultDelay;
			BYTE tmp[30];
			ar >> strID;			// ID
			ar >> wFrameCount;		// フレーム数

			ar >> wFrameWidth;		// フレーム幅
			ar >> wFrameHeight;		// フレーム高さ
			ar >> wDefaultDelay;	// 遅延時間のディフォルト


			if ( strID == CString("EASYTooN000")  ) {
				// EASYTooN000 だと圧縮形式(旧形式)
				m_nDefaultDelay = wDefaultDelay;
				m_sizeFrame = CSize ( wFrameWidth, wFrameHeight );
				// フレームの読み込み
				CLzwDecoder decoder;
				WORD	 wFrameSize = wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) );
				BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
				BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4 ]; // ロード時にコケるようならここを大きくするといいカモ

				ar.Read(&tmp,28);
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
			}
			else if( strID == CString("EASYTooN001")  ){
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
			} else if( strID == CString("EASYTooN002" )  ){
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
					ar.Read(&tmp,28);
					ar >> wDelay;					// 遅延時間
					ar >> dwLength;	
					// ブロックサイズ
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
			}
			else if( strID == CString("EASYTooN003" )  )
			{
				WORD	wMemoryCount;
				ar >> wMemoryCount;
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

				BYTE*	 pFrame	= new BYTE[ wFrameSize ];

				for ( int i = 0 ; i < wMemoryCount ; i++ )
				{
					DWORD dwLength;

					ar >> dwLength;					// ブロックサイズ
					ar.Read ( pFrameBuf, dwLength ); // ブロック読み込み

					
					CBitmap* pBmp = new CBitmap();
					pBmp->CreateBitmap(wFrameWidth,wFrameHeight,1,1,pFrame);
					
					pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
					pBmp->SetBitmapDimension(wFrameWidth,wFrameHeight);
				
					m_MemoryList.AddTail ( pBmp );
					
				}

				delete pFrameBuf;
				delete pFrame;
			}
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
		ar << (WORD)0;		// number of iterations
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

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}
void CEasyToonDoc::ImportGif(CArchive* ar, BOOL IsOpening, int m_nCurrentFrame )
{
	
	char FileType[3];
	char GifVersion[3];
	BYTE Ext;
	WORD wFrameHeight;
	WORD wFrameWidth;
	WORD wFrameSize;
	BYTE GLOBALCOLOR;
	char NbBitsPerPixel;
	bool GlbExist;
	int globalnb;


	char Buffer[1024];

	ar->ReadString(FileType,3);
	ar->ReadString(GifVersion,3);
	*ar >> wFrameWidth;
	*ar >> wFrameHeight;
	*ar >> GLOBALCOLOR;

	NbBitsPerPixel = ((GLOBALCOLOR&0x70)>>4)+1;

	if(NbBitsPerPixel>1)
	{
		MessageBox(NULL,"Cannot load or import the picture.\nTotal number of colors is greater than 2. Warning !","Error",MB_OK);

		if (IsOpening)
		{
			CFrame*	 frame	 = new CFrame(CSize(FRAMEDOC_DEFAULTWIDTH,FRAMEDOC_DEFAULTHEIGHT));
			m_frameList.RemoveAll();
			m_frameList.AddHead(frame);

			MessageBox(NULL,"Warning ! Don't save the Gif file or it will be replaced by an Easytoon File !","Error",MB_OK);
		}

		return;
	}

	wFrameSize	   = (WORD)(wFrameHeight * ( wFrameWidth / 8 + (wFrameWidth & 15 ? 2 : 0) ));

	if (IsOpening)
	{
		m_sizeFrame.cx = wFrameWidth;
		m_sizeFrame.cy = wFrameHeight;
	}

	GlbExist=GLOBALCOLOR&128;

	ar->Read(Buffer,1);
	ar->Read(Buffer,1);


	if (GlbExist)
	{
		globalnb=pow(2,(float)(GLOBALCOLOR&7)+1)*3;
		for (int i=0;i<globalnb;i++)
		{
			ar->Read(Buffer,1);
		}
	}
	ar->Read(&Ext,1);

	if(Ext=='!')
	{
		ar->ReadString(Buffer,2);	// Block Size
		ar->ReadString(Buffer,11);	// Application ID
		ar->Read(Buffer,5);
		Ext = 0;
	}



	// フレームの書き込み
	CLzwDecoder encoder;
	//POSITION pos		= m_frameList.GetHeadPosition();
	BYTE*	 pFrameBuf	= new BYTE[ wFrameSize ];
	BYTE*	 pCompBuf	= new BYTE[ wFrameSize*4]; // セーブ時にコケるようならここを大きくするといいカモ
	int nInsertPos=1;

	int i=0;

	WORD wDelay;  //= frame->GetDelay() != 0 ? frame->GetDelay() : m_nDefaultDelay;
	WORD Left;
	WORD Top;
	WORD dwLength;
	bool LocalMap;
	BYTE Local;

	int NbColorLocalMap;


	do
	{

		switch(Ext)
		{

		case '!':

			ar->Read(&Ext,1);
			ar->Read(&Ext,1);
			ar->Read(&Ext,1);
			*ar>>wDelay;

			ar->Read(&Ext,1);// transparency
			ar->Read(&Ext,1);// terminator
			break;
		case',':

			*ar >> Left;
			*ar >> Top;
			*ar >> wFrameWidth;
			*ar >> wFrameHeight;
			*ar >> Local;

			LocalMap=Local&128;

			if (LocalMap)
			{
				NbColorLocalMap=pow(2,(float)(Local&7)+1)*3;
				for(int i=0;i<NbColorLocalMap;i++)
				{
					ar->Read(&Ext,1);
				}
			}


			unsigned int ClearCode, EOICode;

			unsigned int        BlockSize =0;
			//For loading the code
			unsigned int        CodeSize = 0;

			ar->Read(&CodeSize,1);
			ar->Read(&BlockSize,1);

			ClearCode = 1 << CodeSize;
			EOICode = ClearCode + 1;

			int j=0;

			char Read;
			UINT ByteRead=0;

			while(BlockSize!=0)
			{
				ar->Read(&pCompBuf[j],BlockSize);
				j+=BlockSize;
				ar->Read(&BlockSize,1);
			};
			//pCompBuf[j] = '\0';
			encoder.Decode(pCompBuf, wFrameWidth,1,pFrameBuf);

			CFrame*	 frame	 = new CFrame(CSize(wFrameWidth,wFrameHeight));
			CBitmap* pBmp	 = frame->GetBitmap();

			pBmp->SetBitmapBits(wFrameSize,pFrameBuf );


			frame->SetDelay(wDelay);

			if(IsOpening)
			{
				m_frameList.AddTail(frame);

			}
			else
			{
				m_frameList.InsertBefore( m_frameList.FindIndex( m_nCurrentFrame ), frame );
			}

			m_nCurrentFrame++;
			break;

		}

		ar->Read(&Ext,1);
	}
	while (Ext!=0x3B);

	delete []pFrameBuf;
	delete [] pCompBuf;

	/*ar->Close();

	ar=NULL;*/
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
		::strcpy_s( szBasename, lpszFilename );

		// 後ろから拡張子までをつぶす
		// 後ろからやらないとディレクトリ名に'.'があったときに死亡しちゃうヨ
		int nbStrings = ::strlen( szBasename );
		for( i = 0 ; i < nbStrings ; i++ ) {
			if( szBasename[nbStrings-1-i] == '.' ) {
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
		for( i = 0 ; i < nbBitmaps ; i++ ) {
			// ファイル名捏造
			::strcpy_s( szFilename, szBasename );
			::sprintf_s( szTemp, "%04d", i );
			::strcat_s( szFilename, szTemp );
			::strcat_s( szFilename, ".bmp" );

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
		BYTE tmp[30];

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
			} else if( strID == CString("EASYTooN002" )  ){
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
					ar.Read(&tmp,28);
					ar >> wDelay;					// 遅延時間
					ar >> dwLength;	
					// ブロックサイズ
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
			} else if( strID == CString("EASYTooN003" )  )
			{
				WORD	wMemoryCount;
				ar >> wMemoryCount;
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

				BYTE*	 pFrame	= new BYTE[ wFrameSize ];

				for ( int i = 0 ; i < wMemoryCount ; i++ )
				{
					DWORD dwLength;

					ar >> dwLength;					// ブロックサイズ
					ar.Read ( pFrameBuf, dwLength ); // ブロック読み込み

					
					CBitmap* pBmp = new CBitmap();
					pBmp->CreateBitmap(wFrameWidth,wFrameHeight,1,1,pFrame);
					
					pBmp->SetBitmapBits ( wFrameSize, pFrameBuf );
					pBmp->SetBitmapDimension(wFrameWidth,wFrameHeight);
					
					m_MemoryList.AddTail ( pBmp );
					
				}

				delete pFrameBuf;
				delete pFrame;
			}
		
		}
			ar.Close();
	}
	CATCH ( CException, e )
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

void CEasyToonDoc::SetNextFrameNb(int NbFrame)
{
	m_nTraceNextFrameNb = NbFrame;
}

void CEasyToonDoc::SetPrevFrameNb(int NbFrame)
{
	m_nTracePrevFrameNb = NbFrame;
}

void CEasyToonDoc::DeleteFrameFromMemory(int nFrame)
{
	m_MemoryList.RemoveAt(m_MemoryList.FindIndex(nFrame));
}

void CEasyToonDoc::CopyFrameToMemory(int nFrame)
{
	CBitmap * bmp = GetFrameBmp(nFrame);
	bmp->SetBitmapDimension(m_sizeFrame.cx,m_sizeFrame.cy);

	m_MemoryList.AddTail(Utils::CopyBitmap(bmp));

}

CObList * CEasyToonDoc::GetMemoryList()
{
		return &m_MemoryList;
}

CBitmap*	CEasyToonDoc::GetBitmapFromMemoryList(int mFrame)
{
	if(m_MemoryList.GetCount()>=mFrame+1)
	{
		CBitmap* tmpBmp = (CBitmap*)m_MemoryList.GetAt(m_MemoryList.FindIndex( mFrame ));
	
		return tmpBmp;
	}

	return NULL;
}
