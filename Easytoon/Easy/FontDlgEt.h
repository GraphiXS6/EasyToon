#pragma once


// CFontDlgEt

class CFontDlgEt : public CFontDialog
{
	DECLARE_DYNAMIC(CFontDlgEt)

public:
	CFontDlgEt(LPLOGFONT lplfInitial = NULL,
			DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
			CDC* pdcPrinter = NULL,
			CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CFontDlgEt(const CHARFORMAT& charformat,
			DWORD dwFlags = CF_SCREENFONTS,
			CDC* pdcPrinter = NULL,
			CWnd* pParentWnd = NULL);
#endif

	virtual ~CFontDlgEt();
	CString GetText(){return m_DlgFtEditC;}
	int GetMaxLenght(){return m_MaxChar;}
	int GetTextNbline(){return m_NbLine;}

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	
	virtual	void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL CFontDlgEt::OnCommand(WPARAM wParam, LPARAM lParam);
private:
	CEdit m_TxtEdit;
	CStatic m_txtEditLbl;
	CString m_DlgFtEditC;
	CFont TxtFont;
	int m_NbLine;
	int m_MaxChar;
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


