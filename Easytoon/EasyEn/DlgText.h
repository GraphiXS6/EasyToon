#pragma once
#include "afxcmn.h"
#include "afxwin.h"



class CDlgText : public CDialog
{
	DECLARE_DYNAMIC(CDlgText)

public:
	CDlgText(CWnd* pParent = NULL);   
	int GetTextNbline(){return Nbline;};
	int GetMaxLenght(){return MaxChar;};
	void setfont(CFont & Font);
	CString GetText(){return m_Text;};
	virtual ~CDlgText();
	virtual  BOOL OnInitDialog( );

	enum { IDD = IDD_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()

private:
public:
	CEdit m_CtrlEditText;
	CString m_Text;
	CFont m_font;
	int Nbline;
	int MaxChar;

	afx_msg void OnBnClickedOk();
};
