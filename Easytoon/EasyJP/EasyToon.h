// EasyToon.h : EASYTOON アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_EASYTOON_H__EE57B9C9_A60C_11D1_AB14_444553540000__INCLUDED_)
#define AFX_EASYTOON_H__EE57B9C9_A60C_11D1_AB14_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

#define FRAMEDOC_DEFAULTWIDTH		160
#define FRAMEDOC_DEFAULTHEIGHT		120
#define FRAMEVIEW_DEFAULTMAGNIFY	2
#define FRAMEVIEW_CANVASMARGIN		2

/////////////////////////////////////////////////////////////////////////////
// CEasyToonApp:
// このクラスの動作の定義に関しては EasyToon.cpp ファイルを参照してください。
//

class CEasyToonApp : public CWinApp
{
public:
	CEasyToonApp();
	bool GetDem ();
	void SetDem (bool);
	
private:
	bool m_demarrage;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEasyToonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CEasyToonApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EASYTOON_H__EE57B9C9_A60C_11D1_AB14_444553540000__INCLUDED_)
