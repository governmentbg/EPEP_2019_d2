DEFINE_RESPONSE_TABLE1(TSetSessionBaseDialog, TSessionCompositionDialog)
//{{TSetSessionBaseDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_SESSION_COMPOSITION_JUDGE, JudgeCBNSelChange),
//{{TSetSessionBaseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSetSessionBaseDialog Implementation}}

TSetSessionBaseDialog::TSetSessionBaseDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, const char *kinds,
	bool edit, int resId)
:
	TSessionCompositionDialog(parent, session, lawsuit, edit, resId)
{
	new TCharAliasFace(this, IDC_SET_SESSION_TYPE, "F_TYPE", Type, TYPE_LAWSUITS);
	new TLongFace(this, IDC_SET_SESSION_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_SET_SESSION_YEAR, "F_YEAR");

	date = new TDateFace(this, IDC_SET_SESSION_DATE, "F_DATE");
	kind = new TCharAliasFace(this, IDC_SET_SESSION_KIND, "F_KIND", Kind, kinds);
	end = new TDateFace(this, IDC_SET_SESSION_END, "F_END", false);
	entime = new TTimeFace(this, IDC_SET_SESSION_ENTIME, "F_ENTIME", false);

	// 2014:184; 2015:014 OK -> ctor, session->GetComposition() -> inline
	// 2010:018 LPR: F_JUDGE is now from lawsuit/SESSION_CHANJED
	// 2015:094 +F_PROSECUTOR; 2015:250 +F_SECRETAR; 2017:096 not for edit
	if (!edit)
		session->GetLast("F_JUDGE_X, F_REJUDGE_X, F_PRESIDENT, " JUROR_NAMES_UNQUOTED ", F_PROSECUTOR, F_SECRETAR");
}

bool TSetSessionBaseDialog::IsValid()
{
	bool isValid = TSessionCompositionDialog::IsValid();

	if (isValid)
	{
		bool tEndate = !end->Date().Empty() && end->Date() != date->Date();
		bool tEntime = !entime->Time().Empty();

		// 2007:220 LPR/LRQ: simple new 1-session check
		if (date->Date() < lawsuit->date)
			isValid = error("Датата на насрочване е преди датата на образуване на делото.");
		else if (date->Date() < lawsuit->previousDate)
			isValid = error("Датата на насрочване е преди датата на разпореждане за образуване.");
		else if (tEndate && end->Date() < date->Date())
			isValid = error("Крайната дата е преди датата на насрочване.");
		// 2008:060 FIX: was !entime->Time().Empty()
		else if (tEndate && !tEntime)
			isValid = error("Крайния час е задължителен при заседания продължаващи повече от един ден.");
		// 2008:060 FIX: was missing
		else if (!tEndate && tEntime && entime->Time() < time->Time())
			isValid = error("Крайния час е преди началния.");
	}

	return isValid;
}

void TSetSessionBaseDialog::SetupWindow()
{
	TSessionCompositionDialog::SetupWindow();

	composition->EnableWindow(can_func(RIGHT_RECOMPOS));
	end->EnableWindow(lawsuit->key.type == TYPE_PUNISHMENT_LAW);
	AdjustPresident();
}

void TSetSessionBaseDialog::JudgeCBNSelChange()
{
	AdjustPresident();
}
