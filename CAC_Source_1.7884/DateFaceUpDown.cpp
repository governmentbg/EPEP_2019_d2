DEFINE_RESPONSE_TABLE1(TDateFaceUpDown, TUpDown)
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE;

TDateFaceUpDown::TDateFaceUpDown(TWindow* parent, int resourceId,
  TDateFace *tDateFace) :
    TUpDown(parent, resourceId, NULL),
    dateFace(tDateFace), oldPos(0)
{
}

void TDateFaceUpDown::SetupWindow()
{
  TUpDown::SetupWindow();
  oldPos = 1;
  SetRange(0, 2);
  SetPos(oldPos);
}

void TDateFaceUpDown::EvLButtonDown(uint modKeys, TPoint& point)
{
  TUpDown::EvLButtonDown(modKeys, point);

  const bool up = GetPos() - oldPos > 0;
  const bool dn = GetPos() - oldPos < 0;
  CDate d = dateFace->Date();

  SetPos(oldPos);
  if(!d.Empty())
  {
    if (up && d < DATE_MAX)
    {
	d += 1;
	dateFace->SetDate(d);
    }
    else if (dn && d > DATE_MIN)
    {
	d += -1;
	dateFace->SetDate(d);
    }
    else if (up || dn)
	MessageBeep(0xFFFFFFFF);
  }
}
