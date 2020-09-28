DEFINE_RESPONSE_TABLE1(TEditCompositionDialog, TGroupDialog)
//{{TEditCompositionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditCompositionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditCompositionDialog Implementation}}

TEditCompositionDialog::TEditCompositionDialog(TWindow* parent, TFineComposition *tComposition, int resId)
:
	TGroupDialog(parent, tComposition, resId)
{
	new TCharAliasFace(this, IDC_EDIT_COMPOSITION_COLLEGE, "F_COLLEGE", College, COLLEGE_ALLS);
	name = new TStringFace(this, IDC_EDIT_COMPOSITION_NAME, "F_NAME", SIZE_OF_ALIAS);
	judge = new TUserAliasFace(this, IDC_EDIT_COMPOSITION_JUDGE, "F_JUDGE", POST_VISIBLE_JUDGES,
		FLAG_ACTIVE | USER_INTER | ALIAS_EMPTY);
	secretar = new TUserAliasFace(this, IDC_EDIT_COMPOSITION_SECR, "F_SECRETAR", POST_VISIBLE_SECRETARS,
		FLAG_ACTIVE | ALIAS_EMPTY);

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	judgeX = new TUserAliasFace(this, IDC_EDIT_COMPOSITION_JUDGE_X, "F_JUDGE_X", POST_VISIBLE_JUDGES);
	reJudgeX = new TUserAliasFace(this, IDC_EDIT_COMPOSITION_REJUDGE_X, "F_REJUDGE_X", POST_VISIBLE_JUDGES);
	president = new TUserAliasFace(this, IDC_EDIT_COMPOSITION_PRESIDENT, "F_PRESIDENT", POST_VISIBLE_MATURES);
#endif  // APPEAL || ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_REGIONAL
	judgeX = NULL;
	reJudgeX = NULL;
	president = NULL;
#endif  // REGIONAL
}

void TEditCompositionDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		if (!any(Composition()->judge))
			strcpy(Composition()->judge, EMPTY);

		if (Group()->Update())
			CmOk();
	}
}

void TEditCompositionDialog::SetupWindow()
{
	Composition2Group(Group(), Composition()->college, Composition()->composition, true, true);

	TGroupDialog::SetupWindow();
	name->EnableWindow(COURT_FLAT);
}

bool TEditCompositionDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (isValid)
	{
		// 2007:211 LPR/LRQ: relaxed open res dialog compos check
		if (
			atob(judge) && // 2009:323 LPR: was judge->S() &&
			(atob(president) || atob(judgeX) || atob(reJudgeX)) &&
			strcmp(judge->S(), president->S()) &&
			strcmp(judge->S(), judgeX->S()) &&
			strcmp(judge->S(), reJudgeX->S())
		)
			isValid = error("Докладчик трябва да е един от съдиите.");
	}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

	return isValid;
}
