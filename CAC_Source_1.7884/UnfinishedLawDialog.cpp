DEFINE_RESPONSE_TABLE1(TUnfinishedLawDialog, TFloatSubjectDialog)
//{{TUnfinishedLawDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_UNFINISHED_LAW_REJUDGE, ReJudgeBNClicked),
//{{TUnfinishedLawDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TUnfinishedLawDialog Implementation}}

TUnfinishedLawDialog::TUnfinishedLawDialog(TWindow* parent, TUnfinishedLawGroup *group, int resId)
:
	TFloatSubjectDialog(parent, group, resId)
{
	kinds = new TCharListFace(this, IDC_UNFINISHED_LAW_KINDS, "F_KINDS", Kind, KIND_LAWSUITS, SIZE_OF_KINDS);
	generalKinds = new TCharListFace(this, IDC_UNFINISHED_LAW_GENERAL_KINDS, "F_GENERAL_KINDS", Kind, KIND_ALL_GENERALS,
		SIZE_OF_KINDS);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	senders = new TSenderListBox(this, IDC_UNFINISHED_LAW_SENDERS, UCN_RETURN_LOWERS);
	sendersLabel = new TStatic(this, IDC_UNFINISHED_LAW_SENDERS_LABEL);
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	finished = new TDateFace(this, IDC_UNFINISHED_LAW_FINISHED, "F_FINISHED");
	compositions = new TCharAutoListFace(this, IDC_UNFINISHED_LAW_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	reJudge = new TCheckFace(this, IDC_UNFINISHED_LAW_REJUDGE, "F_REJUDGE");
	judges = new TUserListBox(this, IDC_UNFINISHED_LAW_JUDGES, POST_VISIBLE_JUDGES);
	new TCheckFace(this, IDC_UNFINISHED_LAW_AGE_FIRST, "F_AGE_FIRST");
	new TRadioFace(this, IDC_UNFINISHED_LAW_AGE_ALLS, "F_AGE_ALLS");
	new TRadioFace(this, IDC_UNFINISHED_LAW_AGE_THREE_OR_LESS, "F_AGE_THREE_OR_LESS");
	new TRadioFace(this, IDC_UNFINISHED_LAW_AGE_THREE_TO_SIX, "F_AGE_THREE_TO_SIX");
	new TRadioFace(this, IDC_UNFINISHED_LAW_AGE_SIX_TO_TWELVE, "F_AGE_SIX_TO_TWELVE");
	new TRadioFace(this, IDC_UNFINISHED_LAW_AGE_MORE_THAN_TWELVE, "F_AGE_MORE_THAN_TWELVE");
	new TRadioFace(this, IDC_UNFINISHED_LAW_BY_TYN, "F_BY_TYN");
	new TRadioFace(this, IDC_UNFINISHED_LAW_BY_TSYN, "F_BY_TSYN");
	new TRadioFace(this, IDC_UNFINISHED_LAW_BY_TKYN, "F_BY_TKYN");
	byJudge = new TRadioFace(this, IDC_UNFINISHED_LAW_BY_JUDGE, "F_BY_JUDGE");
}

void TUnfinishedLawDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(UnfinishedLawGroup()->newSubjects, UnfinishedLawGroup()->oldSubjects);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	senders->GetMark(&UnfinishedLawGroup()->senders);
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	judges->GetMark(&UnfinishedLawGroup()->judges);
	UnfinishedLawGroup()->FormCriteria(m);

	if (execute)
	{
		mstr title;

		UnfinishedLawGroup()->Title(title);
		TUnfinishedLawQuery(this, str(m), str(title), UnfinishedLawGroup()).Execute();
	}
	else
		MessageEx(m);

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
	sendersLabel->SetCaption("П&ървоинстанционни съдилища");
#endif  // APPEAL || ADMIN || MILITARY
}

void TUnfinishedLawDialog::ReJudgeBNClicked()
{
	// 2017:234 LPR: query always displays judge
	//byJudge->SetCaption(reJudge->GetCheck() == BF_CHECKED ? "По съдия при образуване" : "По съдия-докладчик");
}
