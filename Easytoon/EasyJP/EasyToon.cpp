// EasyToon.cpp : アプリケーション用クラスの機能定義を行います。
//

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

/////////////////////////////////////////////////////////////////////////////
// CEasyToonApp

BEGIN_MESSAGE_MAP(CEasyToonApp, CWinApp)
	//{{AFX_MSG_MAP(CEasyToonApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyToonApp クラスの構築

CEasyToonApp::CEasyToonApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	m_demarrage=true;
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CEasyToonApp オブジェクト

CEasyToonApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEasyToonApp クラスの初期化

BOOL CEasyToonApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	//  したければ以下の特定の初期化ルーチンの中から不必要なもの
	//  を削除してください。
#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	// 設定が保存される下のレジストリ キーを変更します。
	// 会社名または所属など、適切な文字列に
	// 変更してください。
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEasyToonDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CEasyToonView));
	AddDocTemplate(pDocTemplate);

	// DDE Execute open を使用可能にします。
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	// ドラッグ/ドロップ オープンを許可します
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnWebbutton();
	afx_msg void OnWebbutton2();
	afx_msg void OnWebbutton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_WEBBUTTON, OnWebbutton)
	ON_BN_CLICKED(IDC_WEBBUTTON2, OnWebbutton2)
	ON_BN_CLICKED(IDC_WEBBUTTON3, OnWebbutton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CEasyToonApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CEasyToonApp コマンド

void CAboutDlg::OnWebbutton() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(),NULL,"http://www.radiumsoftware.com",NULL,NULL,0);
}

void CAboutDlg::OnWebbutton2() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(),NULL,"http://www.F-Team.org/setagaya/index.html",NULL,NULL,0);
}

void CAboutDlg::OnWebbutton3() 
{
	ShellExecute(AfxGetMainWnd()->GetSafeHwnd(),NULL,"http://www.flatd2d.info",NULL,NULL,0);
}

bool CEasyToonApp::GetDem()
{
	return m_demarrage;
}

void CEasyToonApp::SetDem(bool Demarrage)
{
	m_demarrage=Demarrage;
}


