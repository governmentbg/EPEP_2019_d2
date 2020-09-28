//{{TBook39Dialog Implementation}}

TBook39Dialog::TBook39Dialog(TWindow* parent, TBook39Group *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	types = new TCharAutoListFace(this, IDC_BOOK_39_TYPES, "F_TYPES", Type, group->types, SIZE_OF_TYPES);
	minNo = new TLongFace(this, IDC_BOOK_39_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_BOOK_39_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_BOOK_39_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_BOOK_39_MAX_YEAR, "F_MAX_YEAR");
	judges = new TUserListBox(this, IDC_BOOK_39_JUDGES, POST_VISIBLE_JUDGES);
	compositions = new TCharAutoListFace(this, IDC_BOOK_39_COMPOSITIONS, "F_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	receivers = new TSenderListBox(this, IDC_BOOK_39_RECEIVERS, UCN_RETURN_LOWERS);
	slcReasons = new TSLCReasonListBox(this, IDC_BOOK_39_SLC_REASONS, C2S[SLC_TYPE_CANCELED], SLC_REASON_BOOK);
	minOutRegDate = new TDateFace(this, IDC_BOOK_39_MIN_OUTREG_DATE, "F_MIN_OUTREG_DATE", false);
	maxOutRegDate = new TDateFace(this, IDC_BOOK_39_MAX_OUTREG_DATE, "F_MAX_OUTREG_DATE", false);
	eventText = new TCheckFace(this, IDC_BOOK_39_EVENT_TEXT, "F_EVENT_TEXT");
}

bool TBook39Dialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minOutRegDate, maxOutRegDate))
			isValid = error("Началната дата на връщане в подчинения съд е по-голяма от крайната.");
		else if (minYear->L() == 0 && minOutRegDate->Date().Empty())
			isValid = error("Задължително е попълването на начална година, или на началната дата на връщане.");
	}

	return isValid;
}

void TBook39Dialog::Action(bool)
{
	TBook39Group *group = TYPECHECK_PTR_CAST(TBook39Group, Group());

	// 2015:091 before FormCriteria - for header 
	if (group->minOutRegDate.Empty())
	{
		group->minOutRegDate.day = 1;
		group->minOutRegDate.month = 1;
		group->minOutRegDate.year = group->minYear;
	}

	judges->GetMark(&group->judges);
	receivers->GetMark(&group->receivers);
	slcReasons->GetMark(&group->slcReasons);

	if (!group->slcReasons.Count())
	{
		slcReasons->SelectAll(true);
		slcReasons->GetMark(&group->slcReasons);
	}

	CmOk();
}

void TBook39Dialog::SetupWindow()
{
	TFloatDialog::SetupWindow();
	// from BookDialog
	types->SetCaretIndex(0, false);
	types->SetS("");
}
