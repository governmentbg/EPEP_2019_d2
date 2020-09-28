DEFINE_RESPONSE_TABLE1(TListWindowEx, TListWindow)
//{{TListWindowExRSP_TBL_BEGIN}}
  EV_WM_KILLFOCUS,
  EV_WM_PAINT,
  EV_MESSAGE(LVM_SETBKCOLOR, EvSetBkColor),
  EV_MESSAGE(LVM_SETTEXTBKCOLOR, EvSetTextBkColor),
  EV_MESSAGE(LVM_SETTEXTCOLOR, EvSetTextColor),
  EV_MESSAGE(LVM_SETIMAGELIST, EvSetImageList),
  EV_WM_SETFOCUS,
  EV_WM_SIZE,
//{{TListWindowExRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TListWindowEx Implementation}}


TListWindowEx::TListWindowEx(TWindow* parent, int resourceId)
:
    TListWindow(parent, resourceId)
{
  InitializeData();
}


TListWindowEx::~TListWindowEx()
{
}


void TListWindowEx::InitializeData()
{
	m_bFullRowSel = false;
	m_bClientWidthSel = true;

	m_cxClient = 0;
	m_cxStateImageOffset = 0;

	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk = ::GetSysColor(COLOR_WINDOW);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);

  m_bHorzLines = false;  // 01.oct.2004
  m_bVertLines = false;  // 01.oct.2004

  // From PreCreateWindow
	m_bFullRowSel = true;
}  // TListWindowEx::InitializeData

// offsets for first and other columns
#define OFFSET_FIRST    2
#define OFFSET_OTHER    6

void TListWindowEx::DrawItem(DRAWITEMSTRUCT far& drawInfo)
{
	//CListCtrl& ListCtrl=GetListCtrl();  // I'll try to do without it.
  TDC dc(drawInfo.hDC);
	TRect rcItem(drawInfo.rcItem);
	UINT uiFlags = ILD_TRANSPARENT;
	//CImageList* pImageList;  // I'll try to replace it with the following.
  HIMAGELIST hImgList;
	int nItem = drawInfo.itemID;
	BOOL bFocus = (GetFocus() == HWindow);
	COLORREF clrTextSave, clrBkSave;
	COLORREF clrImage = m_clrBkgnd;
//	static _TCHAR szBuff[MAX_PATH];
	static char szBuff[MAX_PATH];
	LPCTSTR pszText;
//	char* pszText;

// get item data

	//LV_ITEM lvi;
	TListWindItem lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.pszText = szBuff;
	lvi.cchTextMax = sizeof(szBuff);
	lvi.stateMask = 0xFFFF;     // get all state flags
	// ListCtrl.GetItem(&lvi);
	GetItem(lvi);

	bool bSelected =
    (bFocus || (GetStyle() & LVS_SHOWSELALWAYS)) && lvi.state & LVIS_SELECTED;
	bSelected = bSelected || (lvi.state & LVIS_DROPHILITED);

// set colors if item is selected

	TRect rcAllLabels;
	//ListCtrl.GetItemRect(nItem, rcAllLabels, LVIR_BOUNDS);
	GetItemRect(nItem, &rcAllLabels, Bounds);

	TRect rcLabel;
	//ListCtrl.GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, &rcLabel, Label);

	rcAllLabels.left = rcLabel.left;
	if (m_bClientWidthSel && rcAllLabels.right<m_cxClient)
		rcAllLabels.right = m_cxClient;

	if (bSelected)
	{
		clrTextSave = dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clrBkSave = dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		dc.FillRect(rcAllLabels, TBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
		dc.FillRect(rcAllLabels, TBrush(m_clrTextBk));

// set color and mask for the icon

	if (lvi.state & LVIS_CUT)
	{
		clrImage = m_clrBkgnd;
		uiFlags |= ILD_BLEND50;
	}
	else if (bSelected)
	{
		clrImage = ::GetSysColor(COLOR_HIGHLIGHT);
		uiFlags |= ILD_BLEND50;
	}

// draw state icon

	UINT nStateImageMask = lvi.state & LVIS_STATEIMAGEMASK;
	if (nStateImageMask)
	{
		int nImage = (nStateImageMask>>12) - 1;
		//pImageList = ListCtrl.GetImageList(LVSIL_STATE);
    hImgList = GetImageList(State);

    if(hImgList)
    {
  		TImageList(hImgList).Draw(nImage, dc, rcItem.left, rcItem.top,
        ILD_TRANSPARENT);
    }
	}

// draw normal and overlay icon

	TRect rcIcon;
	//ListCtrl.GetItemRect(nItem, rcIcon, LVIR_ICON);
	GetItemRect(nItem, &rcIcon, Icon);

	//pImageList = ListCtrl.GetImageList(LVSIL_SMALL);
  hImgList = GetImageList(Small);
  if(hImgList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		if (rcItem.left<rcItem.right-1)
		{
			::ImageList_DrawEx(hImgList, lvi.iImage,
					dc.GetHDC(), rcIcon.left, rcIcon.top, 16, 16,
					m_clrBkgnd, clrImage, uiFlags | nOvlImageMask);
		}
	}

  TPen grayPen(TColor(RGB(224, 224, 224)));  // 01.oct.2004

// draw item label

	//ListCtrl.GetItemRect(nItem, rcItem, LVIR_LABEL);
	GetItemRect(nItem, &rcItem, Label);
	rcItem.right -= m_cxStateImageOffset;

	pszText = MakeShortString(dc, szBuff,
				rcItem.right-rcItem.left, 2*OFFSET_FIRST);

	rcLabel = rcItem;
	rcLabel.left += OFFSET_FIRST;
	rcLabel.right -= OFFSET_FIRST;

	dc.DrawText(pszText, -1, rcLabel,
    DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

  if(m_bVertLines)  // 01.oct.2004 - begin
  {
    dc.SelectObject(grayPen);

    dc.MoveTo(rcLabel.right, rcLabel.top);
    dc.LineTo(rcLabel.right, rcLabel.bottom);
    dc.RestorePen();
  }                 // 01.oct.2004 - end

// draw labels for extra columns

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	for(int nColumn = 1;
    //ListCtrl.GetColumn(nColumn, &lvc);
    GetColumn(nColumn, &lvc);
    nColumn++)
	{
		rcItem.left = rcItem.right;
		rcItem.right += lvc.cx;


    if(m_bVertLines)  // 01.oct.2004 - begin
    {
      dc.SelectObject(grayPen);

      dc.MoveTo(rcItem.right, rcItem.top);
      dc.LineTo(rcItem.right, rcItem.bottom);
      dc.RestorePen();
    }                 // 01.oct.2004 - end

		//int nRetLen = ListCtrl.GetItemText(nItem, nColumn,
		//				szBuff, sizeof(szBuff));
		int nRetLen = GetItemText(nItem, nColumn, szBuff, sizeof(szBuff));
		if (nRetLen == 0)
			continue;

		pszText = MakeShortString(dc, szBuff,
			rcItem.right - rcItem.left, 2*OFFSET_OTHER);

		UINT nJustify = DT_LEFT;

		if(pszText == szBuff)
		{
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_RIGHT:
				nJustify = DT_RIGHT;
				break;
			case LVCFMT_CENTER:
				nJustify = DT_CENTER;
				break;
			default:
				break;
			}
		}

		rcLabel = rcItem;
		rcLabel.left += OFFSET_OTHER;
		rcLabel.right -= OFFSET_OTHER;

		dc.DrawText(pszText, -1, rcLabel,
			(uint16)(nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP |
      DT_VCENTER));
	}

// draw  a horizontal line at the top of the item

  if(m_bHorzLines)  // 01.oct.2004 - begin
  {
    dc.SelectObject(grayPen);
    dc.MoveTo(rcAllLabels.left, rcAllLabels.top);
    dc.LineTo(rcAllLabels.right, rcAllLabels.top);
    if((nItem + 1) == GetItemCount())
    {
      dc.MoveTo(rcAllLabels.left, rcAllLabels.bottom);
      dc.LineTo(rcAllLabels.right, rcAllLabels.bottom);
    }
    dc.RestorePen();
  }                 // 01.oct.2004 - end

// draw focus rectangle if item has focus

	if (lvi.state & LVIS_FOCUSED && bFocus)
		dc.DrawFocusRect(rcAllLabels);

// set original colors if item was selected

	if (bSelected)
	{
		dc.SetTextColor(clrTextSave);
		dc.SetBkColor(clrBkSave);
	}
}  // TListWindowEx::DrawItem


bool TListWindowEx::GetFullRowSel()
{
	return(m_bFullRowSel);
}


LPCTSTR TListWindowEx::MakeShortString(TDC& dc, LPCTSTR lpszLong,
  int nColumnLen, int nOffset)
{
	//static const _TCHAR szThreeDots[] = _T("...");
	static const char szThreeDots[] = "...";  // Must not be longer than 15 chars!

	int nStringLen = lstrlen(lpszLong);

	if(nStringLen == 0 ||
		(dc.GetTextExtent(lpszLong, nStringLen).cx + nOffset) <= nColumnLen)
	{
		return(lpszLong);
	}

	//static _TCHAR szShort[MAX_PATH];
	static char szShort[MAX_PATH + 16];

	lstrcpy(szShort,lpszLong);
	int nAddLen = dc.GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i = nStringLen-1; i > 0; i--)
	{
		szShort[i] = 0;
		if((dc.GetTextExtent(szShort, i).cx + nOffset + nAddLen)
			<= nColumnLen)
		{
			break;
		}
	}

	lstrcat(szShort, szThreeDots);
	return(szShort);
}  // TListWindowEx::MakeShortString


void TListWindowEx::EvKillFocus(THandle hWndGetFocus)
{
	TListWindow::EvKillFocus(hWndGetFocus);

	// check if we are losing focus to label edit box
	if(hWndGetFocus != NULL && ::GetParent(hWndGetFocus) == HWindow)
		return;

	// repaint items that should change appearance
	if(m_bFullRowSel && (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
		RepaintSelectedItems();
}  // TListWindowEx::OnKillFocus


void TListWindowEx::EvPaint()
{
	// in full row select mode, we need to extend the clipping region
	// so we can paint a selection all the way to the right
	if (m_bClientWidthSel &&
		(GetStyle() & LVS_TYPEMASK) == LVS_REPORT &&
		GetFullRowSel())
	{
		TRect rcAllLabels;
		//GetListCtrl().GetItemRect(0, rcAllLabels, LVIR_BOUNDS);
		GetItemRect(0, &rcAllLabels, Bounds);

		if(rcAllLabels.right < m_cxClient)
		{
			// need to call BeginPaint (in CPaintDC c-tor)
			// to get correct clipping rect
			TPaintDC dc(HWindow);

			TRect rcClip;
			dc.GetClipBox(rcClip);

			rcClip.left = min(rcAllLabels.right-1, rcClip.left);
			rcClip.right = m_cxClient;

			InvalidateRect(rcClip, FALSE);
			// EndPaint will be called in CPaintDC d-tor
		}
	}

  TListWindow::EvPaint();

  // INSERT>> Your code here.

}  // TListWindowEx::EvPaint


LRESULT TListWindowEx::EvSetBkColor(WPARAM, LPARAM lParam)  // wParam
{
	m_clrBkgnd = (COLORREF)lParam;
	return(DefaultProcessing());
}


LRESULT TListWindowEx::EvSetTextBkColor(WPARAM, LPARAM lParam)  // wParam
{
	m_clrTextBk = (COLORREF)lParam;
	return(DefaultProcessing());
}


void TListWindowEx::EvSetFocus(THandle hWndLostFocus)
{
    TListWindow::EvSetFocus(hWndLostFocus);

    // check if we are getting focus from label edit box
    if(hWndLostFocus != NULL && ::GetParent(hWndLostFocus) == HWindow)
      return;

    // repaint items that should change appearance
    if(m_bFullRowSel && (GetStyle() & LVS_TYPEMASK)==LVS_REPORT)
      RepaintSelectedItems();
}  // TListWindowEx::EvSetFocus


LRESULT TListWindowEx::EvSetImageList(WPARAM wParam, LPARAM lParam)
{
	// if we're running Windows 4, there's no need to offset the
	// item text location

	OSVERSIONINFO info;
	info.dwOSVersionInfoSize = sizeof(info);
  m_cxStateImageOffset = 0;

	if(::GetVersionEx(&info))
  {
    if((int)wParam == LVSIL_STATE && info.dwMajorVersion < 4)
    {
      int cx, cy;

      if(::ImageList_GetIconSize((HIMAGELIST)lParam, &cx, &cy))
        m_cxStateImageOffset = cx;
      else
        m_cxStateImageOffset = 0;
    }
  }

	return(DefaultProcessing());
}  // TListWindowEx::EvSetImageList


LRESULT TListWindowEx::EvSetTextColor(WPARAM, LPARAM lParam)  // wParam
{
	m_clrText = (COLORREF)lParam;
	return(DefaultProcessing());
}




void TListWindowEx::EvSize(uint sizeType, TSize& size)
{
	m_cxClient = size.cx;

  TListWindow::EvSize(sizeType, size);

  // INSERT>> Your code here.

}


//BOOL CListViewEx::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// default is report view and full row selection
//	cs.style &= ~LVS_TYPEMASK;
//	cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED;
//	m_bFullRowSel = TRUE;
//
//	return(CListView::PreCreateWindow(cs));
//}


void TListWindowEx::RepaintSelectedItems()
{
	//CListCtrl& ListCtrl = GetListCtrl();
	TRect rcItem, rcLabel;

// invalidate focused item so it can repaint properly

	int nItem = GetNextItem(-1, Focused);

	if(nItem != -1)
	{
		GetItemRect(nItem, &rcItem, Bounds);
		GetItemRect(nItem, &rcLabel, Label);
		rcItem.left = rcLabel.left;

		InvalidateRect(rcItem, FALSE);
	}

// if selected items should not be preserved, invalidate them

	if(!(GetStyle() & LVS_SHOWSELALWAYS))
	{
		for(nItem = GetNextItem(-1, Selected);
			nItem != -1; nItem = GetNextItem(nItem, Selected))
		{
			GetItemRect(nItem, &rcItem, Bounds);
			GetItemRect(nItem, &rcLabel, Label);
			rcItem.left = rcLabel.left;

			InvalidateRect(rcItem, FALSE);
		}
	}

// update changes

	UpdateWindow();
}  // TListWindowEx::RepaintSelectedItems
