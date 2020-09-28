TSessionListDialog::TSessionListDialog(TWindow * parent,TPrintSessionGroup * group, int resId)
	: TFloatDialog(parent, group, resId)
{
	lawKinds = new TCharListFace(this, IDC_SESSION_LIST_KIND, "F_KINDS", Kind, Default->xKinds, SIZE_OF_KINDS);
	minDate = new TDateFace(this, IDC_SESSION_LIST_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_SESSION_LIST_MAX_DATE, "F_MAX_DATE", false);
	compositions = new TCharAutoListFace(this, IDC_SESSION_LIST_COMPOSITION, "F_COMPOSITIONS", CharComposition, QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	hall = new TLongAliasFace(this, IDC_SESSION_LIST_HALL, "F_HALL", Hall, NULL, ALIAS_EMPTY);
	whole = new TRadioFace(this, IDC_SESSION_LIST_WHOLE_DAY, "F_WHOLE_DAY");
	morning = new TRadioFace(this, IDC_SESSION_LIST_MORNING, "F_MORNING");
	afternoon = new TRadioFace(this, IDC_SESSION_LIST_AFTERNOON, "F_AFTERNOON");
	time = new TTimeFace (this, IDC_SESSION_LIST_NOON, "F_TIME", false) ;
	withType = new TRadioFace(this, IDC_SESSION_LIST_HEADER_WITH_TYPE, "F_WITH_TYPE");
	withColl = new TRadioFace(this, IDC_SESSION_LIST_HEADER_WITH_COLL, "F_WITH_COLL");
	variant_1 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_1, "F_VAR_1");
	variant_2 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_2, "F_VAR_2");
	variant_3 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_3, "F_VAR_3");
	variant_4 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_4, "F_VAR_4");
	variant_41 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_41, "F_VAR_41");
	variant_5 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_5, "F_VAR_5");
	variant_6 = new TRadioFace(this, IDC_SESSION_LIST_CONTENTS_VAR_6, "F_VAR_6");
	lines = new TLongFace(this, IDC_SESSION_LIST_LINES, "F_LINES", SIZE_OF_EMPTY_LINES);
	new TCheckFace(this, IDC_SESSION_LIST_REORDER, "F_ARL");
}

void TSessionListDialog::Action(bool execute)
{
	msql m;
	PrintSessionGroup()->FormCriteria(m);

	if (execute)
	{
		//PrintSessionGroup()->selCount = compositions->GetSelCount();
		session_print_list(this, str(m), PrintSessionGroup());
	}
	else
		Message(m);
}

bool TSessionListDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!*lawKinds->S())
			isValid = error("Не е избран вид дело.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
		else if (lines->L() > EMPTY_LINES_MAX)
			isValid = error("Максималният брой празни редове е %d.", EMPTY_LINES_MAX);
	}

	return isValid;
}

void TSessionListDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	time->SetTime(CTime(12, 30, 0));
#if COURT_TYPE == COURT_REGIONAL
	variant_4->EnableWindow(false);
	variant_41->EnableWindow(false);
#endif  // REGIONAL
}
