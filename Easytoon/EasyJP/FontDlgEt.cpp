// FontDlgEt.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "EasyToon.h"
#include "FontDlgEt.h"


// CFontDlgEt

IMPLEMENT_DYNAMIC(CFontDlgEt, CFontDialog)
CFontDlgEt::CFontDlgEt(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) : 
	CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
}

CFontDlgEt::~CFontDlgEt()
{
}

BOOL CFontDlgEt::OnInitDialog()
{
	CRect rect;
	CRect WindowRect;
	CFont TxtLbl;
	LOGFONT TxtLbllog;
	
	this->GetCurrentFont(&TxtLbllog);
	strcpy_s(TxtLbllog.lfFaceName,"Arial");
	TxtLbl.CreateFontIndirect(&TxtLbllog);

    this->GetWindowRect(WindowRect);

	m_txtEditLbl.Create("Texte à insérer :",WS_VISIBLE,CRect(10,285,120,300),this);
	m_TxtEdit.Create(WS_EX_CLIENTEDGE | ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_WANTRETURN | ES_AUTOVSCROLL, CRect(10,310,420,410), this, IDC_EDIT_FONT );

	this->SetWindowPos(NULL,0,0,WindowRect.Width(),WindowRect.Height()+100,SWP_NOMOVE | SWP_NOZORDER);
	
	return CFontDialog::OnInitDialog();
}

void CFontDlgEt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_FONT,m_TxtEdit);
}

BEGIN_MESSAGE_MAP(CFontDlgEt, CFontDialog)
	ON_WM_CHANGEUISTATE()
	ON_WM_DRAWITEM()
//	ON_WM_NCLBUTTONDOWN()
ON_WM_UPDATEUISTATE()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CFontDlgEt::OnOK()
{
		CWnd * Cat=NULL;
		CString Essai;
		int i=1;
		
			while (Cat!=NULL)
			{
				Cat->GetWindowText(Essai);
				//MessageBox(Essai,"Texte du Controle");
				i++;
				Cat=GetDlgItem(i);
			}
	m_TxtEdit.GetWindowText(m_DlgFtEditC);
	m_NbLine=m_TxtEdit.GetLineCount();
	m_MaxChar=0;
	char Line[255]={0};
	for (int i=0;i<m_NbLine;i++)
	{
		m_TxtEdit.GetLine(i,Line,255);
		if (strlen(Line)>m_MaxChar)
		{
			m_MaxChar=strlen(Line);
		}
	}
	
	CFontDialog::OnOK();
}

//void CFontDlgEt::OnNcLButtonDown(UINT nHitTest, CPoint point)
//{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	//TxtFont.DeleteObject();

	//TxtFont.CreateFontIndirect(&m_lf);

	////if(TxtFont!=m_TxtEdit.GetFont())
	////{
	//m_TxtEdit.SetFont(&TxtFont);
//	CFontDialog::OnNcLButtonDown(nHitTest, point);
//}


BOOL CFontDlgEt::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD A_ID=HIWORD(wParam);
	
	switch(A_ID)
	{
	case 4:case 9: case 15704:
		//CHARFORMAT CF;
		//m_TxtEdit.SetWindowText(GetFaceName());
		//GetCharFormat(CF);
		//m_TxtEdit.SetWindowText(CF.szFaceName);
		/*
			DoModal Displays the dialog and allows the user to make a selection. 
GetCharFormat Retrieves the character formatting of the selected font. 
GetColor Returns the color of the selected font. 
GetCurrentFont Retrieves the name of the currently selected font. 
GetFaceName Returns the face name of the selected font. 
GetSize Returns the point size of the selected font. 
GetStyleName Returns the style name of the selected font. 
GetWeight */
			
			
			//m_TxtEdit.SetWindowText(Essai);
			TxtFont.DeleteObject();
			LOGFONT a;
			CFontDialog::GetCurrentFont(&a);
			TxtFont.CreateFontIndirect(&a);
			m_TxtEdit.SetFont(&TxtFont);
			m_TxtEdit.SetFocus();
		
		break;
	}
	return CFontDialog::OnCommand(wParam,lParam);
}

void CFontDlgEt::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	CFontDialog::OnLButtonDown(nFlags, point);
}

