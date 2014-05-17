// DlgText.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "EasyToon.h"
#include "DlgText.h"


// Boîte de dialogue CDlgText

IMPLEMENT_DYNAMIC(CDlgText, CDialog)
CDlgText::CDlgText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgText::IDD, pParent)
{
}

CDlgText::~CDlgText()
{
}

void CDlgText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_CtrlEditText);
}


BEGIN_MESSAGE_MAP(CDlgText, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgText

void CDlgText::OnBnClickedOk()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	m_CtrlEditText.GetWindowText(m_Text);
    Nbline=m_CtrlEditText.GetLineCount();
	MaxChar=0;
	char Line[255]={0};
	for (int i=0;i<Nbline;i++)
	{
		m_CtrlEditText.GetLine(i,Line,255);
		if (strlen(Line)>MaxChar)
		{
			MaxChar=strlen(Line);
		}
	}

	OnOK();
}

BOOL CDlgText::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_CtrlEditText.SetFont(&m_font,true);
	return true;
}

void CDlgText::setfont(CFont & Font)
{
LOGFONT TempFont;
Font.GetLogFont(&TempFont);
m_font.CreateFontIndirect(&TempFont);
}