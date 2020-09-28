DEFINE_RESPONSE_TABLE1(TAbcBookDialog, TFloatDialog)
//{{TAbcBookDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ABC_BOOK_TYPES, TypeCBNSelChange),
	EV_EN_KILLFOCUS(IDC_ABC_BOOK_YEAR, YearKillFocus),
	EV_BN_CLICKED(IDC_ABC_BOOK_NEW_FORMAT, NewFormatBNClicked),
//{{TAbcBookDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

TAbcBookDialog::TAbcBookDialog(TWindow* parent, TAbcBookGroup *group)
	: TFloatDialog(parent, group, IDD_ABC_BOOK)
{
	type = new TCharAliasFace(this, IDC_ABC_BOOK_TYPES, "F_TYPE", Type, Default->colleges);
	minNo = new TLongFace(this, IDC_ABC_BOOK_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_ABC_BOOK_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_ABC_BOOK_YEAR, "F_YEAR");
	minDate = new TDateFace(this, IDC_ABC_BOOK_MIN_DATE, "F_MIN_DATE", false);
	new TDateFaceUpDown(this, IDC_ABC_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_ABC_BOOK_MAX_DATE, "F_MAX_DATE", false);
	new TDateFaceUpDown(this, IDC_ABC_BOOK_MAX_DATE_UPDN, maxDate);
	minLetter = new TFirmNameFace(this, IDC_ABC_BOOK_MIN_LETTER, "F_MIN_LETTER", SIZE_OF_LETTER);
	maxLetter = new TFirmNameFace(this, IDC_ABC_BOOK_MAX_LETTER, "F_MAX_LETTER", SIZE_OF_LETTER);
	allSides = new TCheckFace(this, IDC_ABC_BOOK_ALL_SIDES, "F_ALL_SIDES");
	composition = new TCheckFace(this, IDC_ABC_BOOK_COMPOSITION, "F_COMPOSITION");
	involvement = new TCheckFace(this, IDC_ABC_BOOK_INVOLVEMENT, "F_INVOLVEMENT");
	newFormat = new TCheckFace(this, IDC_ABC_BOOK_NEW_FORMAT, "F_NEW_FORMAT");
	abv = new TRadioFace(this, IDC_ABC_BOOK_ABC, "F_ABC");
	insol = new TRadioFace(this, IDC_ABC_BOOK_INSOL, "F_INSOL");
	create = new TButton(this, IDC_ABC_BOOK_CREATE);
}

bool TAbcBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (!minNo->L())
			isValid = error("Началният номер е задължителен.");
		else if (!year->L())
			isValid = error("Годината е задължителна.");
		else if (!*minLetter->S() || !*maxLetter->S())
			isValid = error("Символите са задължителни.");
		else if (invalid_range(minNo, maxNo))
			isValid = error("Началният номер е по-голям от крайния.");
		else if (maxDate->IsWindowEnabled() && minDate->Date().Empty())
			isValid = error("Липсва начална дата.");
		else if (maxDate->IsWindowEnabled() && maxDate->Date().Empty())
			isValid = error("Липсва крайна дата.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
		else if (!minDate->Date().Empty() && minDate->Date().year < year->L())
			isValid = error("Началната дата е преди годината на делото.");
		else if (!maxDate->Date().Empty() && maxDate->Date().year < year->L())
			isValid = error("Крайната дата е преди годината на делото.");
		else if (*minLetter->S() > *maxLetter->S())
			isValid = error("Началният символ е по-голям крайния.");
	}

	return isValid;
}

void TAbcBookDialog::Action(bool execute)
{
	if (execute && (REPEAT_BOOKS_A & REPEAT_BOOK_A_ABC))
		abc_book(this, (TAbcBookGroup *) Group());
	else
		TFloatDialog::Action(execute);
}

void TAbcBookDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	minNo->SetL(1);
	year->SetL(Today.year);
	minLetter->SetS("0");
	maxLetter->SetS("Я");
	composition->EnableWindow(PRINT_COMPOSITIONS);
	// 2018-03-26: old format
	//newFormat->SetCheck(BF_CHECKED);
	//UpdateButtons();
	type->SetC(kind2type(Default->kind));
	TypeCBNSelChange();

#if COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
	abv->ShowWindow(false);
	insol->ShowWindow(false);
	create->ShowWindow(false);
#endif
}

void TAbcBookDialog::NewFormatBNClicked()
{
#if COURT_TYPE == COURT_REGIONAL
	if (newFormat->GetCheck() == BF_CHECKED)
	{
		composition->EnableWindow(true);
		composition->SetCheck(BF_UNCHECKED);
	}
	else
	{
		composition->EnableWindow(false);
		composition->SetCheck(BF_CHECKED);
	}
#endif  // REGIONAL
}

void TAbcBookDialog::TypeCBNSelChange()
{
	UpdateButtons();

#if COURT_TYPE == COURT_AREAL
	if (type->C() == TYPE_FIRM_LAW)
	{
		minDate->EnableWindow(FALSE);
		maxDate->EnableWindow(FALSE);
		minDate->SetDate(CDate());
		maxDate->SetDate(CDate());
		involvement->SetCheck(BF_UNCHECKED);
		involvement->EnableWindow(FALSE);
	}
	else
	{
		if (!maxDate->IsWindowEnabled())
		{
			YearKillFocus();
			maxDate->SetDate(Today);
		}

		minDate->EnableWindow(TRUE);
		maxDate->EnableWindow(TRUE);
		involvement->EnableWindow(TRUE);
	}
#endif  // AREAL

	NewFormatBNClicked();
}

void TAbcBookDialog::UpdateButtons()
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	// 2018-03-26: was TYPE_TRADE_OR_CITIZEN_LAWS
	if (strchr(TYPE_TRADEALIKE_LAWS, type->C()))
	{
		abv->EnableWindow(true);
		insol->EnableWindow(true);
	}
	else
	{
		abv->SetCheck(BF_CHECKED);
		insol->SetCheck(BF_UNCHECKED);
		abv->EnableWindow(false);
		insol->EnableWindow(false);
	}
#endif  // APPEAL || AREAL

	if (COURT_LOCK)
	{
		abv->EnableWindow(false);
		insol->EnableWindow(false);
		create->EnableWindow(false);
	}
}

void TAbcBookDialog::YearKillFocus()
{
	minDate->SetDate(CDate(1, 1, year->L()));
}