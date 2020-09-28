DEFINE_RESPONSE_TABLE1(TTimeBookDialog, TFloatDialog)
// --------------
	EV_EN_KILLFOCUS(IDC_TB_MIN_DATE, BClicked),
	EV_CBN_SELCHANGE(IDC_TB_TYPES, BChange),
	EV_BN_CLICKED(IDC_TB_MIN_DATE_UPDN, BClicked),
	EV_BN_CLICKED(IDC_TB_CUMULATIVE, BCumulative),
#if COURT_TYPE == COURT_APPEAL
	EV_BN_CLICKED(IDC_TB_WANT_BOOK, BClickedBook),
	EV_BN_CLICKED(IDC_TB_WANT_BOOK_MNO, BClickedBookMNO),
#endif
// -----------
END_RESPONSE_TABLE;

TTimeBookDialog::TTimeBookDialog(TWindow* parent, TTimeBookGroup *group)
	:TFloatDialog(parent, group, IDD_TIME_BOOK) // - da se 
{
	type = new TCharAliasFace(this, IDC_TB_TYPES, "F_TYPE", Type, QUERY_COLLEGES);
	minDate = new TDateFace(this, IDC_TB_MIN_DATE, "F_MIN_DATE", false);
	new TDateFaceUpDown(this, IDC_TB_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_TB_MAX_DATE, "F_MAX_DATE", false);
	new TDateFaceUpDown(this, IDC_TB_MAX_DATE_UPDN, maxDate);
	new TCharAutoListFace(this, IDC_TB_COMPOSIT, "F_COMPOSITIONS",  CharComposition, QUERY_COMPOSITIONS,
		SIZE_OF_COMPOSITIONS);
	new TLongFace(this, IDC_TB_BEGIN_SERIAL_NO, "F_BEGIN_NO", SIZE_OF_EVENT_NO);
	cumulative = new TCheckFace(this, IDC_TB_CUMULATIVE, "F_CUMULATIVE");
	beginDateLabel = new TStatic(this, IDC_TB_BEGIN_DATE_LABEL);
	beginDate = new TDateFace(this, IDC_TB_BEGIN_DATE, "F_BEGIN_DATE", true);
	book = new TCheckFace(this, IDC_TB_WANT_BOOK, "F_BOOK"); 
	bookMno = new TCheckFace(this, IDC_TB_WANT_BOOK_MNO, "F_BOOK_MNO"); 
	summary = new TCheckFace(this, IDC_TB_WANT_SUMMARY, "F_SUMMARY");
	judgeSummary = new TCheckFace(this, IDC_TB_WANT_JUDGE_SUMMARY, "F_JUDGE_SUMMARY");
	resultSet = new TCheckFace(this, IDC_TB_WANT_RESULTS_SET, "F_RESULT_SET");
	newFormat = new TRadioFace(this, IDC_TB_NEW_FORMAT, "F_NEW_FORMAT");
	rf1 = new TRadioFace(this, IDC_TB_1ST_INS, "F_INSTANCE_1_LAWS");
	rf2 = new TRadioFace(this, IDC_TB_2ND_INS, "F_INSTANCE_2_LAWS");
	rf3 = new TRadioFace(this, IDC_TB_3RD_INS, "F_INSTANCE_3_LAWS");
	rf4 = new TRadioFace(this, IDC_TB_2ND_3RD, "F_INSTANCE_2_3_LAWS");
	new TRadioFace(this, IDC_TB_DAY, "F_PERIOD_DAY");
	new TRadioFace(this, IDC_TB_MORNING, "F_PERIOD_MORNING");
	new TRadioFace(this, IDC_TB_AFTERNOON, "F_PERIOD_AFTERNOON");
	pNoon = new TTimeFace (this, IDC_TB_NOON, "F_TIME", false) ;
	new TRadioFace(this, IDC_TB_ORDER_1, "F_ORDER1");
	new TRadioFace(this, IDC_TB_ORDER_2, "F_ORDER2");
}

bool TTimeBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");			
 		else if (invalid_range(beginDate, minDate))
			isValid = error("Началната дата на натрупване е по-голяма от началната дата."); 
		else if (book->GetCheck() == BF_UNCHECKED && bookMno->GetCheck() == BF_UNCHECKED 
			&& summary->GetCheck() == BF_UNCHECKED && judgeSummary->GetCheck() == BF_UNCHECKED 
			&& resultSet->GetCheck() == BF_UNCHECKED)
			isValid = error("Няма избран елемент от секция \"Да се състави\"");
	}
	return isValid;
}

void TTimeBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_A & REPEAT_BOOK_A_TIME))
		time_book(this, (TTimeBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}

void TTimeBookDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();
	newFormat->SetCheck(BF_CHECKED);
	newFormat->EnableWindow(false);
	pNoon->SetTime(CTime(12,30));
	beginDate->SetDate(CDate(1, Today.month, Today.year));
	book->SetCheck(BF_CHECKED);
	summary->SetCheck(BF_CHECKED);
	judgeSummary->SetCheck(BF_CHECKED);
	resultSet->SetCheck(BF_UNCHECKED);
	bookMno->SetCheck(BF_UNCHECKED);
	bookMno->EnableWindow(false);
	
	if (COURT_LOCK)
	{
		cumulative->SetCheck(BF_UNCHECKED);
		cumulative->ShowWindow(false);
	}
	else
	{
		cumulative->SetCheck(BF_CHECKED);
		cumulative->ShowWindow(true);
	}
	
	BCumulative();
	Standard();
}
void TTimeBookDialog::Standard()
{
#if COURT_TYPE == COURT_AREAL
	rf1->SetCheck(BF_CHECKED);
	rf2->SetCheck(BF_UNCHECKED);
	rf3->SetCheck(BF_UNCHECKED);
	rf4->SetCheck(BF_UNCHECKED);

	switch (type->C())
	{
		case TYPE_PUNISHMENT_LAW:
			rf1->EnableWindow(true);
			rf2->EnableWindow(true);
			rf3->EnableWindow(true);
			rf4->EnableWindow(true);
			break;
		case TYPE_FIRM_LAW:
			rf1->EnableWindow(false);
			rf2->EnableWindow(false);
			rf3->EnableWindow(false);
			rf4->EnableWindow(false);
			break;
		case TYPE_CITIZEN_LAW:
		case TYPE_TRADE_LAW:
			rf1->EnableWindow(true);
			rf2->EnableWindow(true);
			rf3->EnableWindow(false);
			rf4->EnableWindow(false);
			break;
		case TYPE_ADMIN_LAW:
			rf1->EnableWindow(true);
			rf2->EnableWindow(false);
			rf3->EnableWindow(true);
			rf4->EnableWindow(false);
			break;
	}
#endif

#if COURT_TYPE == COURT_ADMIN
	rf1->SetCheck(BF_CHECKED);
	rf1->EnableWindow(true);
	rf2->EnableWindow(false);
	rf3->EnableWindow(true);
	rf4->EnableWindow(false);
#endif

#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_MILITARY 
	#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_MILITARY
		(CourtType == COURT_MILITARY_APPEAL ? rf2 : rf1)->SetCheck(BF_CHECKED);		// 2012:300 TRQ/LPR: FIX
		rf1->EnableWindow(false);
		rf2->EnableWindow(false);
		rf3->EnableWindow(false);
		rf4->EnableWindow(false);
	#else
		if (type->C() == TYPE_PUNISHMENT_LAW)
			bookMno->EnableWindow(true);
		else 
		{
			bookMno->SetCheck(BF_UNCHECKED);
			bookMno->EnableWindow(false);
		}
		rf2->SetCheck(BF_CHECKED);
		rf2->EnableWindow(false);
		rf1->EnableWindow(false);
		rf3->EnableWindow(false);
		rf4->EnableWindow(false);
	#endif
#endif

#if COURT_TYPE == COURT_ADMIN
	rf1->SetCheck(BF_CHECKED);
	rf2->SetCheck(BF_UNCHECKED);
	rf3->SetCheck(BF_UNCHECKED);
	rf4->SetCheck(BF_UNCHECKED);
	rf2->EnableWindow(false);
	rf4->EnableWindow(false);
	if (type->C() == TYPE_CITIZEN_LAW)
	{
		rf1->EnableWindow(false);
		rf3->EnableWindow(false);
	}
	else
	{
		rf1->EnableWindow(true);
		rf3->EnableWindow(true);
	}
#endif
}

void TTimeBookDialog::BClicked()
{
	beginDate->SetDate(CDate(1, minDate->Date().month, minDate->Date().year));
}
void TTimeBookDialog::BChange()
{
	Standard();
}
void TTimeBookDialog::BClickedBook()
{
	bookMno->SetCheck(BF_UNCHECKED);
}
void TTimeBookDialog::BClickedBookMNO()
{
	book->SetCheck(BF_UNCHECKED);
}
void TTimeBookDialog::BCumulative()
{
	if (cumulative->GetCheck() == BF_UNCHECKED)
	{
		beginDateLabel->ShowWindow(false);
		beginDate->ShowWindow(false);
	}
	else
	{
		beginDateLabel->ShowWindow(true);
	        beginDate->ShowWindow(true);
	}
}