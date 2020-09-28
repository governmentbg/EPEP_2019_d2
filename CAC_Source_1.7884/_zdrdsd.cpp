DEFINE_RESPONSE_TABLE1(TZdravkovaDistributionDialog, T2ChkBoxesDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
END_RESPONSE_TABLE;

TZdravkovaDistributionDialog:: TZdravkovaDistributionDialog(
	TWindow* parent, const char* const title,
	const char * const nameGrp,  // 27.aug.2002
	const char* const name1, const char* const name2,
	const char* const dateLabel, bool& firstChecked, bool& secondChecked,
	CDate& lDate_, bool& wantSubjRanges_, int resId) :
		T2ChkBoxesDialog(parent, title, nameGrp, name1, name2, firstChecked,
			secondChecked, resId),
		pLastDate(NULL), lDate(lDate_), pLDateStatic(NULL),
		dLabelText(dateLabel), pUseSubjRanges(NULL),
    wantSubjRanges(wantSubjRanges_)
{
	pLDateStatic = new TStatic(this, IDC_LAST_DATE_LABEL,
		dLabelText ? strlen(dLabelText) + 1 : 0);
	pLastDate = new TDateFace(this, IDC_LAST_DATE, "LAST_DATE", true);
	new TDateFaceUpDown(this, IDC_LAST_DATE_UPDN, pLastDate);
  pUseSubjRanges = new TCheckBox(this, IDC_ZD_USE_SUBJ_RANGES, NULL);
}  // TZdravkovaDistributionDialog::TZdravkovaDistributionDialog

void TZdravkovaDistributionDialog::SetupWindow()
{
	T2ChkBoxesDialog::SetupWindow();
	if(dLabelText)
		pLDateStatic->SetText(dLabelText);
	pLastDate->SetDate(lDate);
  pUseSubjRanges->SetCheck(wantSubjRanges ? BF_CHECKED : BF_UNCHECKED);
}  // TZdravkovaDistributionDialog::SetupWindow

void TZdravkovaDistributionDialog::OKBNClicked()
{
	cb1Checked = pChkBx1->GetCheck() == BF_CHECKED;
	cb2Checked = pChkBx2->GetCheck() == BF_CHECKED;
	lDate = pLastDate->Date();
  wantSubjRanges = pUseSubjRanges->GetCheck() == BF_CHECKED;
	CmOk () ;
}  // TZdravkovaDistributionDialog::OKBNClicked

//

