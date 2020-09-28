// --------- TColumnRowListWindow - begin ---------

TColumnRowListWindow::TColumnRowListWindow(TWindow* parent, int resourceId):
  TListWindowEx(parent, resourceId)
{
}  // TColumnRowListWindow::TColumnRowListWindow

TColumnRowListWindow::~TColumnRowListWindow()
{
}  // TColumnRowListWindow::~TColumnRowListWindow

int TColumnRowListWindow::GetColumnCount()
{
  int count = 0;
  bool found = true;
  LV_COLUMN tmp;
  char fake[8];

  tmp.mask = 0;
  tmp.mask |= LVCF_SUBITEM;
  tmp.mask |= LVCF_FMT;
  tmp.mask |= LVCF_WIDTH;
  tmp.pszText = fake;
  tmp.cchTextMax = 2;
  for(int i = 0; found == true; i++)
  {
    if(GetColumn(i, &tmp) == true)
	count ++;
    else
	found = false;
  }
  return count;
}  // TColumnRowListWindow::GetColumnCount

int TColumnRowListWindow::GetColumnAtX(const int x, int* const pX1,
  int* const pX2)
{
  int column = -1;
  const int count = GetColumnCount();
  int leftSideX = 0;

  if(pX1)
    *pX1 = 0;
  if(pX2)
    *pX2 = 0;
  if(0 < x && x < Attr.W)
  {
    bool rT = true;
    LV_COLUMN tmp;
    char fake[8];

    tmp.pszText = fake;
    tmp.cchTextMax = 0;
    tmp.mask = 0;
    tmp.mask |= LVCF_WIDTH;
    for(int i = 0; rT == true && column == -1 && i < count; i++)
	if(GetColumn(i, &tmp))
	{
	  if((tmp.mask & LVCF_WIDTH) == LVCF_WIDTH)
	  {
	    if(leftSideX <= x && x <= leftSideX + tmp.cx)
	    {
		column = i;
		if(pX1)
		  *pX1 = leftSideX + 1;
		if(pX2)
		  *pX2 = leftSideX + tmp.cx - 1;
		if(pX1 && pX2 && *pX1 > *pX2)
		  *pX2 = *pX1;
	    }
	    else
		leftSideX += tmp.cx;
	  }
	  else
	    rT = false;
	}
	else
	  rT = false;
  }
  return column;
}  // TColumnRowListWindow::GetColumnAtX

int TColumnRowListWindow::GetRowAtY(const int y, uint32* const pItData,
  int* const pY1, int* const pY2)
{
  int item = -1;

  if(pItData)
    *pItData = -1;
  if(pY1)
    *pY1 = 0;
  if(pY2)
    *pY2 = 0;
  if(item == -1)
  {
    // suppress a warning
  }
  {
    TPoint pt(Attr.X + 1, y);
    TLwHitTestInfo hitTestInfo(pt);

    item = HitTest(hitTestInfo);
    if(item != -1)
    {
      {
        TListWindItem lwi;

        lwi.iItem = item;
        lwi.iSubItem = 0;
        if(GetItem(lwi))
          if(pItData)
            *pItData = lwi.GetItemData();
      }
      {
        RECT rect;

        if(GetItemRect(item, & rect, Bounds))
        {
          if(pY1)
            *pY1 = rect.top;
          if(pY2)
            *pY2 = rect.bottom;
        }
      }
    }
  }
  return item;
}  // TColumnRowListWindow::GetRowAtY

int TColumnRowListWindow::GetFirstRowWithMatchingData(const uint32 data,
  bool focusIt, bool selectIt)
{
  int item = -1;
  TListWindItem lwi;
  bool rT = true;

  for(int i = 0; rT == true && item == -1 && i < GetItemCount(); i++)
  {
    lwi.iItem = i;
    lwi.iSubItem = 0;
    if((rT = GetItem(lwi)) == true)
      if(data == lwi.GetItemData())
      {
        item = i;
        if(selectIt)
          SetItemState(item, TListWindItem::Selected, TListWindItem::Selected);
        if(focusIt)
        {
          // This doesn't work (at least with WinXP).
          //SetItemState(item, TListWindItem::Focus, TListWindItem::Focus);
          // This doesn't work, too (at least with WinXP).
          //SetItemState(item, LVIS_FOCUSED, LVIF_STATE);
        }
      }
  }
  return item;
}  // TColumnRowListWindow::GetFirstRowWithMatchingData

// --------- TColumnRowListWindow - end ---------

// --------- TItemDataComparer - begin ---------

int TItemDataComparer::Compare(uint32 item1, uint32 item2, uint32 lParam) const
{
  int res = 0;

  if(lParam)
  {
    // Nothing
  }
  if(item1 < item2)
    res = -1;
  else
    if(item1 > item2)
      res = 1;
  return res;
}  // TItemDataComparer::Compare

// --------- TItemDataComparer - end ---------

// --------- TLawDistribListWindow - begin ---------

DEFINE_RESPONSE_TABLE1(TLawDistribListWindow, TColumnRowListWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_PAINT,
  EV_WM_VSCROLL,
  EV_WM_SIZE,
END_RESPONSE_TABLE;

TLawDistribListWindow::TLawDistribListWindow(TWindow* parent, int resourceId,
  const TJudgeArray& jSet, ArrayOfJudgeLawStatsArrays& data,
  const SubjectRoll& sRoll, TStatic& ln_0, TStatic& ln_1):
  TColumnRowListWindow(parent, resourceId),
  judgSet(jSet), dataArr(data), subjRoll(sRoll), ln0(ln_0), ln1(ln_1)
{
	m_bFullRowSel = true;
	m_bClientWidthSel = false;
}  // TLawDistribListWindow::TLawDistribListWindow

TLawDistribListWindow::~TLawDistribListWindow()
{
}

void TLawDistribListWindow::SetupWindow()
{
  TColumnRowListWindow::SetupWindow();
  Refresh();
}

void TLawDistribListWindow::WriteOneCell(const JudgeLawStats& s,
  char* const dest, const size_t destSz)
{
  const char* const methodName = "WriteOneCell";

  if(dest && destSz)
  {
    {
      ostrstream oss (dest, destSz);

      if(s.laws)
        oss << s.laws;
      if(s.complexLaws > 0)
        oss << " / " << setw(3) << s.complexLaws;
      oss << " |" << ends;
    }
    dest[destSz - 1] = '\0';
    if(strlen(dest) + 1 == destSz)
      error("%s::%s\nThe buffer is too short.",
        typeid(*this).name(), methodName);
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
}  // TLawDistribListWindow::WriteOneCell

bool TLawDistribListWindow::InsertOneRow(const JudgeLawStatsArray& jLSA,
  const bool shortNames, const bool withSubjCode)
{
  bool rT = true;
  const char* const methodName = "InsertOneRow";
  const uint32 identification = jLSA.info.Identification();
  const int item = PlaceForIdentification(identification);

  if(item <= GetItemCount())
  {
    if(GetColumnCount() == 1 + jLSA.Count())
    {
      int subItem = 0;
      char tmp[256];

      {  // The item  (subject)
        TListWindItem lwi;

        lwi.iItem = item;
        lwi.iSubItem = subItem;  // Not necessary, but let it be.
        tmp[0] = '\0';
        if(jLSA.info.subjectLo > 0)
        {
          ostrstream oss(tmp, sizeof(tmp));

          if(withSubjCode)
            oss << jLSA.info.subjectLo << " ";
          if(shortNames)
            oss << subjRoll.SubjectToAlias(jLSA.info.subjectLo);
          else
            oss << subjRoll.SubjectToName(jLSA.info.subjectLo);
          oss << ends;
        }
        else
          if(jLSA.info.subjectLo == 0)
            ostrstream(tmp, sizeof(tmp)) << "ÎÁÙÎ" << ends;
        ENDING_ZERO(tmp);
        lwi.SetText(tmp);
        lwi.SetItemData(identification);
        if(InsertItem(lwi) != item)
          rT = false;
      }
	for(int i = 0; rT && i < jLSA.Count(); i++)
      {
	  JudgeLawStats s = *jLSA[i];
        LV_ITEM lvi;

        lvi.mask = 0;
        lvi.mask |= LVIF_TEXT;
        lvi.iItem = item;
        lvi.iSubItem = i + 1;
        WriteOneCell(s, tmp, sizeof(tmp));
        lvi.pszText = tmp;
        rT = SetItem(TListWindItem(lvi));
      }
    }
    else
      error("%s::%s\n%s (%d != %d)",
        typeid(*this).name(), methodName, WRONG_ARR_DIMENSION,
        GetColumnCount() - 1, jLSA.Count());
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TLawDistribListWindow::InsertOneRow

bool TLawDistribListWindow::InsertEmptyRow(const JLawStatsArrInfo& info)
{
  bool rT = true;
  const char* const methodName = "InsertEmptyRow";
  const uint32 identification = info.Identification();
  const int item = PlaceForIdentification(identification);

  if(item <= GetItemCount())
  {
    if(GetColumnCount() == 1 + judgSet.Count())
    {
      int subItem = 0;
      char tmp[256];

      {  // The item  (subject)
        TListWindItem lwi;

        lwi.iItem = item;
        lwi.iSubItem = subItem;  // Not necessary, but let it be.
        tmp[0] = '\0';
        ENDING_ZERO(tmp);
        lwi.SetText(tmp);
        lwi.SetItemData(identification);
        if(InsertItem(lwi) != item)
          rT = false;
      }
	for(int i = 0; rT && i < judgSet.Count(); i++)
      {
        JudgeLawStats s;
        LV_ITEM lvi;

        s.Clear();
        lvi.mask = 0;
        lvi.mask |= LVIF_TEXT;
        lvi.iItem = item;
        lvi.iSubItem = i + 1;
        WriteOneCell(s, tmp, sizeof(tmp));
        lvi.pszText = tmp;
        rT = SetItem(TListWindItem(lvi));
      }
    }
    else
      error("%s::%s\n%s (%d != %d)",
        typeid(*this).name(), methodName, WRONG_ARR_DIMENSION,
	  GetColumnCount() - 1, judgSet.Count());
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TLawDistribListWindow::InsertEmptyRow

bool TLawDistribListWindow::DeleteOneRow(const JLawStatsArrInfo& info)
{
  const char* const methodName = "DeleteOneRow";
  const int item = PlaceForIdentification(info.Identification()) - 1;
  bool res = false;

  if(item >= 0)
  {
    TListWindItem lwi;

    lwi.iItem = item;
    if(GetItem(lwi))
      if(lwi.GetItemData() == info.Identification())
        res = DeleteAnItem(item);
  }
  if(res == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return res;
}  // TLawDistribListWindow::DeleteOneRow

bool TLawDistribListWindow::Refresh()
{
  const char* const methodName = "Refresh";
  bool rT = true;

  // Delete the old content
  for(int i = GetColumnCount() - 1; rT == true && i >= 0; i++)
    rT = DeleteColumn(i);
  if(rT)
    if(GetItemCount() > 0)
      rT = DeleteAllItems();
  // Insert the new content
  if(rT)
    rT = InsertColumns();
  if(rT)
    rT = InsertRows();
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TLawDistribListWindow::Refresh

bool TLawDistribListWindow::InsertColumns()
{
  const char* const methodName = "InsertColumns";
  TListWindColumn lwc;
  char tmp[SIZE_OF_NAME_FOR_BOOK];

  tmp[0] = '\0';
  lwc.SetText(tmp);
  lwc.SetWidth(120);
  strConcat(tmp, "Ïðåäìåò", sizeof(tmp));
  InsertColumn(0, lwc);
  lwc.SetWidth(80);
  lwc.mask |= LVCF_FMT;
  lwc.fmt = LVCFMT_RIGHT;
  for(int i = 0; i < judgSet.Count(); i++)
  {
    const TJudge& j = *(judgSet[i]);

    tmp[0] = '\0';
    if(ucnPresent(j.ucn))
    {
      citizenUCNAndUCNTypeToName (j.ucn, j.ucnType, tmp, sizeof(tmp),
        false, true, false);
    }
    else
      error("Judge column %d:\nInvalid ucn - '%.*s'", i,
        sizeof(j.ucn), j.ucn);
    InsertColumn(i + 1, lwc);
  }
  {
    const bool res = (GetColumnCount() == 1 + judgSet.Count());

    if(res == false)
      error("%s::%s\n%s", typeid(*this).name(), methodName,
        UNSUCCESSFUL_OPERATION);
    return res;
  }
}  // TLawDistribListWindow::InsertColumns

bool TLawDistribListWindow::InsertRows()
{
  const char* const methodName = "InsertRows";
  bool rT = true;

  for(int i = 0; rT == true && i < dataArr.Count(); i++)
    if(dataArr[i]->AllStatsEmpty() == false)  // 17.jun.2003
      rT = InsertOneRow(*(dataArr[i]),
        true  // 28.jul.2003
        );
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TLawDistribListWindow::InsertRows

int TLawDistribListWindow::
PlaceForIdentification(const uint32 identification) const
{
  const char* const methodName = "PlaceForIdentification";
  const int items = GetItemCount();
  int place = items;  // Immediately after the last item
  TListWindItem lwi;
  bool fail = false;

  for(int i = 0; fail == false && place == items && i < items; i++)
  {
    lwi.iItem = i;
    if(GetItem(lwi))
    {
      if(lwi.GetItemData() > identification)
        place = i;
    }
    else
      fail = true;
  }
  if(fail)
  {
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
    place = items;
  }
  return place;
}  // TLawDistribListWindow::PlaceForIdentification

void TLawDistribListWindow::HideLines()
{
  ln0.Show(SW_HIDE);
  ln1.Show(SW_HIDE);
}  // TLawDistribListWindow::HideLines

void TLawDistribListWindow::ShowLines(const TPoint& point)
{
  int y0 = point.Y();
  int y1 = y0;
  const int item = GetRowAtY(point.Y(), NULL, &y0, &y1);
  const int sbThickness = (GetCountPerPage() < GetItemCount()) ?
    ::GetSystemMetrics(SM_CXVSCROLL) : 0;


  HideLines();
  Invalidate();
  if(0 <= item && item < GetItemCount() && Attr.W >= 4 + sbThickness)
  {
    if(0 < y0 && y0 + 1 < Attr.H)
    {
      ln0.Show(SW_SHOW);
      ln0.MoveWindow(Attr.X + 2, y0 + Attr.Y + 1, Attr.W - 4 - sbThickness, 1);
      ln0.BringWindowToTop();
    }
    if(0 < y1 && y1 + 1 < Attr.H)
    {
      ln1.Show(SW_SHOW);
      ln1.MoveWindow(Attr.X + 2, y1 + Attr.Y + 1, Attr.W - 4 - sbThickness, 1);
      ln1.BringWindowToTop();
    }
    Sleep(50);
    if(ln0.IsWindowVisible())
      ln0.Invalidate();
    if(ln1.IsWindowVisible())
      ln1.Invalidate();
  }
}  // TLawDistribListWindow::ShowLines

void TLawDistribListWindow::EvLButtonDown(uint modKeys, TPoint& point)
{
  TColumnRowListWindow::EvLButtonDown(modKeys, point);
  ShowLines(point);
}  // TLawDistribListWindow::EvLButtonDown

void TLawDistribListWindow::EvPaint()
{
  TColumnRowListWindow::EvPaint();
  if(ln0.IsWindowVisible())
    ln0.Invalidate();
  if(ln1.IsWindowVisible())
    ln1.Invalidate();
}  // TLawDistribListWindow::EvPaint

void TLawDistribListWindow::EvVScroll(uint scrollCode, uint thumbPos,
  THandle hWndCtl)
{
  TColumnRowListWindow::EvVScroll(scrollCode, thumbPos, hWndCtl);
  HideLines();
}  // TLawDistribListWindow::EvVScroll

void TLawDistribListWindow::EvSize(uint sizeType, TSize& size)
{
  TColumnRowListWindow::EvSize(sizeType, size);
  HideLines();
}  // TLawDistribListWindow::EvSize

// --------- TLawDistribListWindow - end ---------


// --------- TLawDistribListWindow2 - begin ---------

TLawDistribListWindow2::TLawDistribListWindow2(TWindow* parent, int resourceId,
  const TJudgeArray& jSet, ArrayOfJudgeLawStatsArrays& data,
  const SubjectRoll& sRoll, TStatic& ln_0, TStatic& ln_1):
  TLawDistribListWindow(parent, resourceId, jSet, data, sRoll, ln_0, ln_1)
{
}

void TLawDistribListWindow2::WriteOneCell(const JudgeLawStats& s,
  char* const dest, const size_t destSz)
{
  const char* const methodName = "WriteOneCell";

  if(dest && destSz)
  {
    char surrNumTxt[64];
    const char* const sample = " 1999";

    surrNumTxt[0] = '\0';
    if(s.complexLaws) // <--> s.surrs
      ostrstream(surrNumTxt, sizeof(surrNumTxt))
        << s.complexLaws  // <--> s.surrs
        << ends;
    ENDING_ZERO(surrNumTxt);
    AdjustWidth(surrNumTxt, sizeof(surrNumTxt), sample);
    {
      ostrstream oss (dest, destSz);

      if(s.laws)
        oss << s.laws;
      oss << " |" << surrNumTxt << " |" << ends;
    }
    dest[destSz - 1] = '\0';
    if(strlen(dest) + 1 == destSz)
      error("%s::%s\nThe buffer is too short.",
        typeid(*this).name(), methodName);
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
}  // TLawDistribListWindow2::WriteOneCell

bool TLawDistribListWindow2::InsertOneRow(const JudgeLawStatsArray& jLSA,
  const bool shortNames, const bool withSubjCode)
{
  return TLawDistribListWindow::InsertOneRow(jLSA, shortNames, withSubjCode);
}  // TLawDistribListWindow2::InsertOneRow

void TLawDistribListWindow2::ClosestWidth(char* const txt, const size_t txtSz,
  char* sample, const int align, TListWindow& lw)
{
  const char* const methodName = "TLawDistribListWindow2::ClosestWidth";

  if(txt && txtSz && sample)
  {  // -- 0 --
    const int desiredWidth = lw.GetStringWidth(sample);
    const int originalWidth = lw.GetStringWidth(txt);
    CharPtr ctrl(strlen(txt) + 1);

    ctrl[0] = '\0';
    strConcat(ctrl, txt, ctrl.Size());
    if(originalWidth < desiredWidth)
    {  // -- 1 --
	CharPtr tmp(txtSz);
	const size_t originalLen = strlen(txt);
      int prevDiff = originalWidth - desiredWidth;
      int currDiff = originalWidth - desiredWidth;
      int tmpWidth = 0;
      const char* const filling = " ";

      tmp[0] = '\0';
      while((originalWidth + tmpWidth) < desiredWidth &&
        (originalLen + strlen(tmp) + 1) < txtSz)
      {
        prevDiff = originalWidth + tmpWidth - desiredWidth;
	  strConcat(tmp, filling, tmp.Size());
        tmpWidth = lw.GetStringWidth(tmp);
        currDiff = originalWidth + tmpWidth - desiredWidth;
      }
      if(abs(currDiff) > abs(prevDiff) && strlen(tmp) > 0)
        tmp[strlen(tmp) - 1] = '\0';
      if(align == RIGHT_ALIGN)
	{
	  strConcat(tmp, txt, tmp.Size());
	  txt[0] = '\0';
        strConcat(txt, tmp, txtSz);
      }
      else
        if(align == LEFT_ALIGN)
        {
          strConcat(txt, tmp, txtSz);
        }
	  else
        {  // CENTER_ALIGN or whatever else
          const size_t spaces = strlen(tmp);
          const size_t half = spaces / 2;

          tmp[half] = '\0';
          strConcat(tmp, txt, tmp.Size());
          for(int i = half; i < spaces; i++)
            strConcat(tmp, filling, tmp.Size());
          txt[0] = '\0';
          strConcat(txt, tmp, txtSz);
	  }
    }  // -- 1 --
    if(strstr(txt, ctrl) == NULL)
	error("%s\n%s", methodName, UNSUCCESSFUL_OPERATION);
  }  // -- 0 --
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, methodName);
}  // TLawDistribListWindow2::ClosestWidth

void TLawDistribListWindow2::AdjustWidth(char* const txt, const size_t txtSz,
  const char* const sample)
{
  if(txt && txtSz && sample)
  {
    CharPtr tmpS(strlen(sample) + 1);

    tmpS[0] = '\0';
    strConcat(tmpS, sample, tmpS.Size());
    ClosestWidth(txt, txtSz, tmpS, RIGHT_ALIGN, *this);
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "AdjustWidth");
}  // TLawDistribListWindow2::AdjustWidth

bool TLawDistribListWindow2::InsertColumns()
{
  return TLawDistribListWindow::InsertColumns();
}  // TLawDistribListWindow2::InsertColumns

bool TLawDistribListWindow2::InsertRows()
{
  return TLawDistribListWindow::InsertRows();
}  // TLawDistribListWindow2::InsertRows

// --------- TLawDistribListWindow2 - end ---------

//


