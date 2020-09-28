DEFINE_RESPONSE_TABLE1(T2RadiosDialog, TScaleDialog)
	EV_BN_CLICKED(IDOK, OKBNClicked),
END_RESPONSE_TABLE;

T2RadiosDialog::T2RadiosDialog(TWindow* parent, int& choosen,
	const char* const title, const char* const groupTitle,
  const char* const radio1Txt, const char* const radio2Txt,
  const int dflt, int resId) :
  	TScaleDialog(parent, resId),
    choice(choosen), dfltChoice(dflt), caption(title),
    groupText(groupTitle), radio1Text(radio1Txt), radio2Text(radio2Txt)
{
  {
    TGroupBox* pGrpBox =
      new TGroupBox(this, IDC_2R_GROUP);

    cluster.SetGrpBox(pGrpBox);
    cluster.Add(new TRadioButton(this, IDC_2R_RADIO1, pGrpBox),
                IDC_2R_RADIO1);
    cluster.Add(new TRadioButton(this, IDC_2R_RADIO2, pGrpBox),
                IDC_2R_RADIO2);
  }
}  // T2RadiosDialog::T2RadiosDialog

void T2RadiosDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();
  if(caption)
	SetCaption(caption);
	if(groupText)
  	SetDlgItemText(IDC_2R_GROUP, groupText);
	if(radio1Text)
  	SetDlgItemText(IDC_2R_RADIO1, radio1Text);
	if(radio2Text)
  	SetDlgItemText(IDC_2R_RADIO2, radio2Text);
	if(0 <= dfltChoice && dfltChoice <= 1)
		cluster.SetSelection(dfltChoice);
}  // T2RadiosDialog::SetupWindow

void T2RadiosDialog::OKBNClicked()
{
	choice = cluster.GetSelection();
  if(choice >= 0)
  	CmOk();
}  // T2RadiosDialog::OKBNClicked

T3RadiosDialog::T3RadiosDialog(TWindow* parent, int& choosen,
  const char* const title, const char* const groupTitle,
  const char* const radio1Txt, const char* const radio2Txt,
  const char* const radio3Txt, const int dflt, int resId):
  T2RadiosDialog(parent, choosen, title, groupTitle, radio1Txt, radio2Txt,
    dflt, resId),
  radio3Text(radio3Txt)
{
  cluster.Add(new TRadioButton(this, IDC_3R_RADIO3, cluster.GetGrpBox()), IDC_3R_RADIO3);
}

void T3RadiosDialog::SetupWindow()
{
	T2RadiosDialog::SetupWindow();
	if(radio3Text)
	SetDlgItemText(IDC_3R_RADIO3, radio3Text);
}  // T3RadiosDialog::SetupWindow
