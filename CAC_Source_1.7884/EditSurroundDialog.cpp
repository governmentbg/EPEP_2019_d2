DEFINE_RESPONSE_TABLE1(TEditSurroundDialog, TEditIncomingDialog)
//{{TEditSurroundDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_EDIT_INCOMING_KIND, KindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditSurroundDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSurroundDialog Implementation}}

TEditSurroundDialog::TEditSurroundDialog(TWindow* parent, TSurround *surround, const char *kinds)
:
	TEditIncomingDialog(parent, surround, kinds ? kinds : KIND_SURROUNDS, Default->xColleges, !kinds, IDD_EDIT_SURROUND)
{
	new TLongFace(this, IDC_EDIT_SURROUND_LAWSUIT_NO, "F_LAWSUIT_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_SURROUND_LAWSUIT_YEAR, "F_LAWSUIT_YEAR");
	lawsuitType = new TCharAliasFace(this, IDC_EDIT_SURROUND_LAWSUIT_TYPE, "F_LAWSUIT_TYPE", Type, TYPE_SURROUNDABLES);
	text = new TStringFace(this, IDC_EDIT_SURROUND_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	judge = new TUserAliasFace(this, IDC_EDIT_SURROUND_JUDGE, "F_JUDGE", "", ALIAS_EMPTY | (edit ? 0 : FLAG_ACTIVE));

	if (surround->decisionKind)
	{
		new TCharAliasFace(this, IDC_EDIT_SURROUND_DECISION_KIND, "F_DECISION_KIND", Kind, NULL);
		new TDateFace(this, IDC_EDIT_SURROUND_DECISION_DATE, "F_DECISION_DATE", false);
	}
}

void TEditSurroundDialog::SetupWindow()
{
	TEditIncomingDialog::SetupWindow();

	KindCBNSelChange();

	if (edit)
	{
		// 2007:122 LRQ/LPR: all surround are editable, but not all fields
		college->EnableWindow(strchr(RESULT_CLEANS, Surround()->Status()));
		SetCaption("Редактиране на съпровождащ документ");
	}
	else
	{
		// 2016:067 LPR/TRQ/TRQ: FIX: should be controlled by KindCBN only
		//judge->EnableWindow(true);
		SetCaption("Регистриране на съпровождащ документ");
	}

	// 2010:048; 2010:053 LRQ: also for edit
	set_direction(Surround()->lawsuit, direction);
}

bool TEditSurroundDialog::IsValid()
{
	bool isValid = TEditIncomingDialog::IsValid();

	if (isValid)
	{

		if (kind->C() == KIND_ANOTHER_SURROUND && !any(text))
			isValid = error("Полетата вид документ и текст не си съответстват.");
		else
		{
			const char *beforeWhat = NULL;

			if (date->Date() < Surround()->decisionDate)
				beforeWhat = strchr(KIND_SESSIONS, Surround()->decisionKind) ? "заседанието" : "съдебния акт";

			if (beforeWhat)
				isValid = error("Датата на съпровождащия документ е преди датата на %s.", beforeWhat);
			else
				isValid = CheckDirection();
		}
	}

	return isValid;
}

void TEditSurroundDialog::KindCBNSelChange()
{
#if COURT_TYPE == COURT_AREAL
	if (lawsuitType->C() == TYPE_FIRM_LAW && kind->C() == KIND_CHANGE_SURROUND)
	{
		TLawsuit lawsuit;
		const char *tJudge = INTER;

		judge->SetInter(true);
		judge->SetXCrit(POST_VISIBLE_JUDGES);
		lawsuit.key = Surround()->lawsuit;

		if (lawsuit.Try("F_JUDGE") && strcmp(lawsuit.judge, INTER))
		{
			strcpy(User->ucn, lawsuit.judge);

			if (User->Try() && (User->flags & FLAG_ACTIVE))
				tJudge = lawsuit.judge;
		}

		judge->SetS(tJudge);
	}
	else
#endif  // AREAL
	{
		judge->SetInter(false);
		judge->SetXCrit("");
	}
}
