// MemoryViewer.cpp : implementation file
//

#include "stdafx.h"
#include "MemoryViewer.h"
#include "Utils.h"


// CMemoryViewer dialog

IMPLEMENT_DYNAMIC(CMemoryViewer, CDialogBar)

CMemoryViewer::CMemoryViewer(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
 
}

CMemoryViewer::~CMemoryViewer()
{
}

BOOL CMemoryViewer::OnInitDialogBar()
{
	UpdateData(false);
	_imgLst = new CImageList();
	//_imgLst->Create(1,1,ILC_COLOR ,0,1);
	
	_lvMemory.SetImageList(_imgLst,LVSIL_NORMAL);
	btnAddMemory.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ADDBMPMEM),IMAGE_ICON,16,16,LR_SHARED));
	btnDeleteMemory.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_DELBMPMEM),IMAGE_ICON,16,16,LR_SHARED));
	btnRestoreMemory.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_RESTBMPMEM),IMAGE_ICON,16,16,LR_SHARED));
	return TRUE;
}

void CMemoryViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, _lvMemory);
	DDX_Control(pDX, IDC_BTNADDMEMORY, btnAddMemory);
	DDX_Control(pDX, IDC_BTNRESTOREMEMORY, btnRestoreMemory);
	DDX_Control(pDX, IDC_BTNDELETEMEMORY, btnDeleteMemory);
}

BOOL CMemoryViewer::Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName,
	UINT nStyle, UINT nID)
{
	// Let MFC Create the control
	if(!CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID))
		return FALSE;

	// Since there is no WM_INITDIALOG message we have to call
	// our own InitDialog function ourselves after m_hWnd is valid
	if(!OnInitDialogBar())
		return FALSE;

	return TRUE;
}

BOOL CMemoryViewer::Create(CWnd * pParentWnd, UINT nIDTemplate,
	UINT nStyle, UINT nID)
{
	if(!Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID))
		return FALSE;

	// Since there is no WM_INITDIALOG message we have to call
	// our own InitDialog function ourselves after m_hWnd is valid
	if(!OnInitDialogBar())
		return FALSE;
	return TRUE;
}

BEGIN_MESSAGE_MAP(CMemoryViewer, CDialogBar)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST1, &CMemoryViewer::OnLvnItemActivateList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMemoryViewer::OnNMRClickList1)
	ON_BN_CLICKED(IDC_BTNADDMEMORY, &CMemoryViewer::OnBnClickedBtnaddmemory)
	ON_BN_CLICKED(IDC_BTNRESTOREMEMORY, &CMemoryViewer::OnBnClickedBtnrestorememory)
	ON_BN_CLICKED(IDC_BTNDELETEMEMORY, &CMemoryViewer::OnBnClickedBtndeletememory)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMemoryViewer::OnNMClickList1)
	ON_WM_SIZE()
	ON_NOTIFY(HDN_FILTERCHANGE, 0, &CMemoryViewer::OnHdnFilterChangeList1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, &CMemoryViewer::OnLvnEndlabeleditList1)
END_MESSAGE_MAP()


// CMemoryViewer message handlers


void CMemoryViewer::setMemoryList(CObList* imgList)
{
	_objList = imgList;
	
	CalculateThumbnailsSize();
	_lvMemory.SetIconSpacing(_thumbSize.cx+5,_thumbSize.cy);

	Update();
}

void CMemoryViewer::Update()
{
	_lvMemory.DeleteAllItems();
	
	_imgLst->DeleteImageList();
	_imgLst->Create(_thumbSize.cx,_thumbSize.cy,ILC_COLOR ,0,10);
	_lvMemory.SetImageList(_imgLst,LVSIL_NORMAL);
	
    _lvMemory.SetRedraw(false);

	for(int i = 0; i<_objList->GetCount();i++)
	{
		POSITION pos = _objList->FindIndex(i);
		CBitmap * cob = Utils::CopyBitmap((CBitmap*)_objList->GetAt(pos));
		
		cob = Utils::ResizeBitmap(cob,_thumbSize.cx,_thumbSize.cy);
		
		int img = _imgLst->Add(cob,cob);
		
		CString strImgNb;
		strImgNb.Format("%d",i+1);

		_lvMemory.InsertItem(i,strImgNb,img);
        
		_lvMemory.SetItem(i,0, LVIF_IMAGE, NULL, img,0, 0, 0);
		
		 pos = NULL;
		 delete cob;
	}
	_lvMemory.SetRedraw(true);
}

void CMemoryViewer::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	bDisableIfNoHndler = FALSE;
	CDialogBar::OnUpdateCmdUI(pTarget,bDisableIfNoHndler);
}

void CMemoryViewer::OnLvnItemActivateList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CMemoryViewer::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	selectedItem = pNMIA->iItem;
	*pResult = 0;
}

void CMemoryViewer::setParentDocument(CEasyToonView* pView)
{
	this->pView = pView;
}

void CMemoryViewer::OnBnClickedBtnaddmemory()
{
	pView->OnMemoryBtn(true,0);
}

void CMemoryViewer::OnBnClickedBtnrestorememory()
{
	if(selectedItem<0)
		return;
	pView->OnMemoryBtn(false,selectedItem);
}

void CMemoryViewer::OnBnClickedBtndeletememory()
{
	if(selectedItem<0 && _lvMemory.GetItemCount()>=selectedItem)
		return;


	pView->GetDocument()->DeleteFrameFromMemory(selectedItem);
	selectedItem--;
	Update();
	
}

void CMemoryViewer::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	selectedItem = pNMItemActivate->iItem;
	*pResult = 0;
}

void CMemoryViewer::CalculateThumbnailsSize()
{
	CSize tmpSize = pView->GetDocument()->GetFrameSize();
	
	if(tmpSize.cx == 0 || tmpSize.cy == 0)
	{
		_thumbSize.cx = DEFTHUMBX;
		_thumbSize.cy = DEFTHUMBY;
		return;
	}

	CRect lvMemortRect;
	
	_lvMemory.GetWindowRect(&lvMemortRect);
		
	_thumbSize.cy = (int)(tmpSize.cy * ((double)(lvMemortRect.Size().cy-40)/(double)tmpSize.cy));
	_thumbSize.cx = (int)(tmpSize.cx * ((double)(lvMemortRect.Size().cy-40)/(double)tmpSize.cy));
}

void CMemoryViewer::OnSize(UINT nType, int cx, int cy)
{
	
	CDialogBar::OnSize(nType, cx, cy);


	CListCtrl tmpComparator;
	CRect lvMem;
	CRect mfRect;

	if(_lvMemory!= tmpComparator)
	{
		CDockState mvDockState;
		if (!this->IsFloating())
		{
			_lvMemory.GetClientRect(&lvMem);

			_lvMemory.SetWindowPos(NULL,5,35,_memViewWndSize.cx-15,130,SWP_FRAMECHANGED);
			_lvMemory.UpdateWindow();

			this->RedrawWindow(CRect(0,0,_memViewWndSize.cx,_memViewWndSize.cy));
		}
	}

	 //_memViewWndSize.cx = cx;
	 //_memViewWndSize.cy = cy;

	
}

CSize CMemoryViewer::CalcDynamicLayout(int nLength, DWORD nMode)
{
	CalculateDefaultBarSize();
	return _memViewWndSize;
}


void CMemoryViewer::CalculateDefaultBarSize()
{
	CRect mfRect;
	
	if (!IsFloating())
	{
	this->GetDockingFrame()->GetWindowRect(&mfRect);

	//GetParent()->GetWindowRect(&mfRect);

	_memViewWndSize.cx = mfRect.Size().cx ;
	_memViewWndSize.cy = 180 ;
	}
	else
	{
		_memViewWndSize.cy = 180;
	}
}


void CMemoryViewer::OnHdnFilterChangeList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Internet Explorer 5 or greater.
	// The symbol _WIN32_IE must be >= 0x0500.
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CMemoryViewer::OnLvnEndlabeleditList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
